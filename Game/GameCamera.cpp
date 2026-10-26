#include "stdafx.h"
#include "GameCamera.h"
#include"player.h"


GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	m_toCameraPosition.Set(100.0f, 500.0f, -400.0f);

	m_player = FindGO<Player>("player");

	// カメラを半径50の球として扱い、壁や床との間に余裕を持たせる。
	// この値を大きくすると地形から離れる距離も大きくなる。
	m_cameraCollisionSolver.Init(50.0f);

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(30000.0f);
	return true;
}

void GameCamera::Update()
{
	Vector3 target = m_player->GetPosition();

	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y軸周りの回転
	Quaternion qRot;//Y軸にはQuaternionを使う。回転を表すクラス。
	qRot.SetRotationDeg(Vector3::AxisY, 3.0f * x);
	qRot.Apply(m_toCameraPosition);


	//X軸周りの回転。
	Vector3 axisX;//X軸にはVector3を使う。回転軸を表すクラス。
	axisX.Cross(Vector3::AxisY, m_toCameraPosition);
	axisX.Normalize();

	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 150.0f;

	Vector3 toCameraPosOld = m_toCameraPosition;
	Vector3 toPosDir = m_toCameraPosition;
	toPosDir.Normalize();
	if (toPosDir.y < -0.12f) {
		//カメラが上向きすぎ。
		m_toCameraPosition = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//カメラが下向きすぎ。
		m_toCameraPosition = toCameraPosOld;
	}//toCameraPosOldで直前のカメラの位置を保存しておいて、カメラが上向きすぎたり下向きすぎたりしたら、直前の位置に戻す。

		//視点を計算する。
	Vector3 pos = target + m_toCameraPosition;//m_toCameraPosition分座標をずらして三人称視点に

	//メインカメラに注視点と視点を設定する。
	g_camera3D->SetTarget(target);
	// 注視点から予定位置まで球を動かし、途中の地形にぶつかるか調べる。
	// ぶつかる場合は壁の手前へ補正し、ぶつからなければ予定位置を使う。
	Vector3 correctedPosition = pos;
	m_cameraCollisionSolver.Execute(correctedPosition, pos, target);
	// m_toCameraPositionは変更しないので、障害物がなくなると元の距離に戻る。
	g_camera3D->SetPosition(correctedPosition);

	//カメラの更新。
	g_camera3D->Update();
}
