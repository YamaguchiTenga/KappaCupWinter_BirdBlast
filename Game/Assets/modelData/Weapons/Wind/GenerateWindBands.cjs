// AI生成：風の帯3本と尾2本の立体モデル。実行すると同じフォルダへTKMを書き出す。
const fs = require('node:fs');
const path = require('node:path');
const vertices = [], indices = [];
function strip(segments, layers, makeVertex) {
    const start = vertices.length;
    for (let i = 0; i <= segments; ++i) {
        for (let j = 0; j <= layers; ++j) vertices.push(makeVertex(i / segments, j / layers));
    }
    for (let i = 0; i < segments; ++i) for (let j = 0; j < layers; ++j) {
        const a = start + i * (layers + 1) + j, b = a + layers + 1;
        indices.push(a, b, a + 1, a + 1, b, b + 1);
    }
}
for (let band = 0; band < 3; ++band) {
    const radius = [88, 102, 84][band], height = [-24, 8, 41][band];
    strip(72, 4, (u, v) => {
        const angle = u * Math.PI * 1.35 + band * 2.1;
        const taper = Math.sin(u * Math.PI) * 0.8 + 0.2;
        return [Math.cos(angle) * radius, height + Math.sin(angle) * 16 + (v - 0.5) * 12 * taper,
            Math.sin(angle) * radius, 0, 1, 0, band + u * 0.9999, v];
    });
}
for (let tail = 0; tail < 2; ++tail) {
    strip(32, 4, (u, v) => {
        const side = tail * 2 - 1;
        return [side * (50 + u * 15) + Math.sin(u * 5) * 5,
            8 + tail * 22 + u * 8 + (v - 0.5) * 10 * (1 - u),
            -35 - u * 150, 0, 1, 0, 3 + tail + u * 0.9999, v];
    });
}
const chunks = [];
function write(n, fn) { const b = Buffer.alloc(n); fn?.(b); chunks.push(b); }
write(4, b => { b[0] = 100; b.writeUInt16LE(1, 2); });
write(12, b => { b.writeUInt32LE(1); b.writeUInt32LE(vertices.length, 4); b[8] = 2; });
write(20); // Five empty texture paths; the shader supplies the colours.
for (const vertex of vertices) write(56, b => {
    vertex.forEach((value, i) => b.writeFloatLE(value, i * 4)); b.fill(255, 48);
});
write(4, b => b.writeUInt32LE(indices.length / 3));
write(indices.length * 2, b => indices.forEach((index, i) => b.writeUInt16LE(index + 1, i * 2)));
fs.writeFileSync(path.join(__dirname, 'WindBands.tkm'), Buffer.concat(chunks));
console.log(JSON.stringify({ vertices: vertices.length, triangles: indices.length / 3 }));
