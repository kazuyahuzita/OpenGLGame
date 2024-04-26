/**
* @file unlit.frag
*/
#version 450

// シェーダへの入力
layout(location=0) in vec3 inPosition; // ワールド座標
layout(location=1) in vec2 inTexcoord; // テクスチャ座標

// テクスチャサンプラ
layout(binding=0) uniform sampler2D texColor;
layout(binding=1) uniform sampler2D texEmission;

// 出力する色データ
out vec4 outColor;

// プログラムからの入力
layout(location=100) uniform vec4 color; // 物体の色
layout(location=101) uniform vec4 emission; // 物体の放射色

void main()
{
  vec4 c = texture(texColor, inTexcoord);

  // テクスチャのガンマ補正を解除
  const float crtGamma = 2.2;
  c.rgb = pow(c.rgb, vec3(2.2));

  outColor = c * color;
  if (emission.w > 0) 
    {
      outColor.rgb += texture(texEmission, inTexcoord).rgb * emission.rgb;
    } else {
      outColor.rgb += emission.rgb;
    }

  // ガンマ補正を行う
  outColor.rgb = pow(outColor.rgb, vec3(1 / crtGamma));
}