/*!
 * @brief	繧ｹ繝励Λ繧､繝医す繧ｧ繝ｼ繝繝ｼ縲?
 */


cbuffer cb : register(b0){
    float4x4 mvp;//スプライトの頂点を画面に描くための座標変換。
    float4 mulColor;//画像の色に掛ける調整色。
};

//定数バッファ
//structみたいにデータを並べれる
//registerはシェーダーがデータを受け取る場所を指定するためのもの
cbuffer HeatGaugeParams : register(b1)
{
    float fillRatio; //ゲージの表示割合。0で空、1でマックス。
};

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain( PSInput In ) : SV_Target0
{
    float2 arcCenter = float2(0.5f, 1.0f); //仮の中心。画像の下辺の真ん中。
    float2 direction = In.uv - arcCenter; //中心から、いま描く点への向き。
	
	//UVの縦と横を揃えるために*2をしている。こうすることでゲージとスピードが一定になった
	//左からゲージをスタートさせたいから、-を掛けている
    float angle = atan2(-direction.y, -direction.x * 2.0f);
    float arcRatio = angle / 3.14159265f; //半円の角度（πラジアン）で割る。
    clip(fillRatio - arcRatio); //arcRatio > fillRatioなら、この点の描画をスキップ
	
	return colorTexture.Sample(Sampler, In.uv) * mulColor;
}
float4 PSMainGamma( PSInput In ) : SV_Target0
{
	float4 color = colorTexture.Sample(Sampler, In.uv) * mulColor;
	color.xyz =  pow(max( color.xyz, 0.0001f ), 1.0f/2.2f);
	return color;
}