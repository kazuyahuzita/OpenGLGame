/**
* @file cloud.frag
*/
#version 450

// シェーダへの入力
layout(location=0) in vec3 inPosition; // ワールド座標
layout(location=1) in vec2 inTexcoord; // テクスチャ座標

// テクスチャサンプラ
layout(binding=0) uniform sampler2D texDensity;

// 出力する色データ
out vec4 outColor;

// プログラムからの入力
layout(location=100) uniform vec3 color; // 物体の色

void main()
{
	float density = texture(texDensity, inTexcoord).r;
	outColor = vec4(color, density);
}