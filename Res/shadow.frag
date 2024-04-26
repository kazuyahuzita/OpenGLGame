/**
* @file shadow.frag
*/
#version 450

// シェーダへの入力
layout(location=1) in vec2 inTexcoord; // テクスチャ座標

// テクスチャサンプラ
layout(binding=0) uniform sampler2D texColor;

// プログラムからの入力
layout(location=102) uniform float alphaCutoff; // フラグメントを破棄する境界値

void main()
{
  vec4 c = texture(texColor, inTexcoord);

  // カットオフ値が指定されている場合、アルファがその値未満の場合は描画をキャンセル
  if (alphaCutoff > 0) {
    if (c.a < alphaCutoff) {
      discard; // フラグメントを破棄(キャンセル)
    }
  }
}