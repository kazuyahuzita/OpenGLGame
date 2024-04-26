/*
@file skeletal.vert
*/

#version 450
//シェーダーへの入力
layout(location =0)in vec3 inPosition;
layout(location=1) in vec2 inTexcoord; // テクスチャ座標
layout(location=2) in vec3 inNormal;   // 法線ベクトル
//変数の型の前にuが入っているのは「符号なし整数型」を意味する
//逆はiの「符号あり整数型」
layout(location=3) in uvec4 inJoints;  // 座標変換行列の番号
layout(location=4) in vec4 inWeights;  // 座標変換行列の影響度

//シェーダーからの出力
//数字で関係を取り合っている、関数とかの使い方ではできない（出来るけど使えない）
layout(location=0) out vec3 outPosition; // ワールド座標
layout(location=1) out vec2 outTexcoord; // テクスチャ座標                                                                                                                                                            
layout(location=2) out vec3 outNormal;   // 法線ベクトル
layout(location=3) out vec3 outShadowTexcoord; // シャドウテクスチャ座標

// プログラムからの入力
//親子階層の複雑化を防ぐために、計算式を最適化
//「前の計算結果に次のベクトルをかける」操作を繰り返すことで、
//座標変換に求められることが分かるようにする
//行列にすることで、ロケーション番号の節約、ベクトルよりも行列の方が効率的
//ただし、必要なメモリ領域はベクトル配列と同一
layout(location=0) uniform mat4 transformMatrix;
layout(location=1) uniform mat3 normalMatrix;

// アスペクト比と視野角による拡大率
layout(location=3) uniform vec2 aspectRatioAndScaleFov;

layout(location=4) uniform vec3 cameraPosition; // カメラの位置
layout(location=5) uniform vec2 cameraSinCosY;  // カメラのY軸回転
layout(location=6) uniform vec3 cameraPosition1;  // カメラのX軸回転
layout(location=7) uniform vec2 cameraSinCosX;  // カメラのX軸回転

layout(location=10) uniform mat4 shadowTextureMatrix; // シャドウテクスチャ行列
layout(location=11) uniform float shadowNormalOffset; // 座標を法線方向にずらす量

// 関節データ用のシェーダストレージブロック
//std430:シェーダーストレージブロックのデータ配置方法の指定
//シェーダストレージブロック専用で、データをC言語に近い形でメモリ上配置に
//binding:CPU側でシェーダストレージブロックを指定するときに使う番号を設定
//シェーダストレージブロックでは、C言語の配列と同様に配列のサイズを省略できる
//サイズを省略したら、CPU側からサイズ指定化可
layout(std430, binding=0) buffer JointDataBlock
{
  mat4 jointMatrices[]; // 関節の座標変換行列
};

/**
* ベクトルを回転させる
*
* @param v       回転させるベクトル
* @param sinCosX X軸回転に使用するsinとcosの値
* @param sinCosY Y軸回転に使用するsinとcosの値
*
* @return ベクトルvを、X軸->Y軸の順番で回転させたベクトル
*/
vec3 RotateXY(vec3 v, vec2 sinCosX, vec2 sinCosY)
{
  // X軸回転
  v.zy = vec2(
    v.z * sinCosX.y + v.y * sinCosX.x,
    -v.z * sinCosX.x + v.y * sinCosX.y);

  // Y軸回転
  v.xz = vec2(
    v.x * sinCosY.y + v.z * sinCosY.x,
    -v.x * sinCosY.x + v.z * sinCosY.y);

  return v;
}
void main()
{
    outTexcoord = inTexcoord;
    // ローカル座標系からワールド座標系に変換      
    
    // 関節行列に影響度を掛けて加算合成
    mat4 jointMatrix =
     jointMatrices[inJoints.x] * inWeights.x +
     jointMatrices[inJoints.y] * inWeights.y +
     jointMatrices[inJoints.z] * inWeights.z +
     jointMatrices[inJoints.w] * inWeights.w;
     
    // 関節行列とゲームオブジェクトの座標変換行列を合成
    mat4 modelMatrix = transformMatrix * jointMatrix;

    //ローカル座標からワールド座標に変換
    gl_Position =  modelMatrix  * vec4(inPosition, 1);
    outPosition = gl_Position.xyz;

    // ワールド法線を計算
    mat3 jointNormalMatrix = transpose(inverse(mat3(modelMatrix)));
    outNormal =normalize(jointNormalMatrix * inNormal);

    // シャドウテクスチャ座標を計算
    outShadowTexcoord = outPosition + outNormal * shadowNormalOffset;
    outShadowTexcoord = vec3(shadowTextureMatrix * vec4(outShadowTexcoord, 1));

    // ワールド座標系からビュー座標系に変換
    float cameraSinY = cameraSinCosY.x;
    float cameraCosY = cameraSinCosY.y;
    vec3 pos = gl_Position.xyz - cameraPosition;
    gl_Position.x = pos.x * cameraCosY + pos.z * cameraSinY;
    gl_Position.y = pos.y;
    gl_Position.z = pos.x * -cameraSinY + pos.z * cameraCosY;
    
    vec3 pos1 = gl_Position.xyz - cameraPosition1; 

    float cameraSinX = cameraSinCosX.x;
    float cameraCosX = cameraSinCosX.y;
    gl_Position.x = pos1.x;
    gl_Position.y = pos1.y * cameraCosX  + pos1.z * -cameraSinX;
    gl_Position.z = pos1.y * cameraSinX + pos1.z * cameraCosX;

    gl_Position.xy *= aspectRatioAndScaleFov;

    // 深度値の計算結果が-1～+1になるようなパラメータA, Bを計算
    const float near = 0.35;
    const float far = 1000;
    const float A = -2 * far * near / (far - near);
    const float B = (far + near) / (far - near);
    // 遠近法を有効にする
    gl_Position.w = -gl_Position.z;
    gl_Position.z = -gl_Position.z * B + A; // 深度値を補正
}