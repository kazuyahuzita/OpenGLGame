/*
@file standard.frag
*/
#version 450


//// シェーダへの入力
layout(location=0) in vec3 inPosition; // ワールド座標
layout(location=1) in vec2 inTexcoord; // テクスチャ座標
layout(location=2) in vec3 inNormal;   // 法線ベクトル
layout(location=3) in vec3 inShadowTexcoord; // シャドウテクスチャ座標

// テクスチャサンプラ
//sampler2Dは
//テクスチャから読み取った値をそのまま返す
layout(binding=0) uniform sampler2D texColor;
layout(binding=1) uniform sampler2D texEmission;

//sampler2DShadowは
//テクスチャから読み取った値と引数で受け取ったZ座標を比較して、その結果を返す
layout(binding=2) uniform sampler2DShadow texShadow; //影テクスチャ用, 影用の深度テクスチャ
//sampler2Dに「Z座標と比較する」という処理を追加したサンプラ
// プログラムからの入力
layout(location=100) uniform vec4 color; // 物体の色
layout(location=101) uniform vec4 emission; // 物体の発光色
layout(location=102) uniform float alphaCutoff; // フラグメントを破棄する境界値
// 環境光
layout(location=107) uniform vec3 ambientLight;

// 平行光源
struct DirectionalLight
{
  vec3 color;     // 色と明るさ
  vec3 direction; // 光の向き
};
layout(location=108) uniform DirectionalLight directionalLight;
 
// 点光源パラメータ
struct Light
{
	vec4 colorAndFalloffAngle[16]; // 色と明るさ, 減衰開始角度
	vec4 positionAndRadius[16]; // 座標と半径
	vec4 directionAndConeAngle[16]; // 向き, 最大照射角度 
};
layout(location=110) uniform int lightCount; // ライトの個数
layout(location=111) uniform Light light;
// 出力する色データ
out vec4 outColor;

vec3 CalcLight(in vec4 colorAndFalloffAngle,
in vec4 positionAndRadius,
in vec4 directionAndConeAngle,
in vec3 normal)
{
		vec3 direction = positionAndRadius.xyz - inPosition;

		// 光源までの距離
		float sqrDistance = dot(direction, direction);
		float dist = sqrt(sqrDistance);

		// 方向を正規化して長さを1にする
		direction = normalize(direction);

		// ランベルトの余弦則を使って明るさを計算
		float theta = max(dot(direction, normal), 0);

		// ランバート反射による反射光のエネルギー量を入射光と等しくするためにπで割る
		float illuminance = theta / 3.14159265;

		//「スポットライトの方向ベクトル」と「スポットライトからフラグメントへ向かうベクトル」
		//のなす角を求め、「最大照射角度」と比較する
		//directionは「フラグメントからライトへ向かうベクトル」であるため、向きを逆にしている

		// 照射角度が0より大きければスポットライトとみなす
		const float coneAngle = directionAndConeAngle.w;
			if (coneAngle > 0)
			{
					 // 「ライトからフラグメントへ向かうベクトル」と「スポットライトのベクトル」のなす角を計算
					 // 角度がconeAngle以上なら範囲外
					 // acos関数はコサインの値をラジアンに戻す関数
					 float angle = acos(dot(-direction, directionAndConeAngle.xyz));
					 if (angle >= coneAngle) 
					 {
					   return vec3(0); // 照射範囲外
					 }

				// 最大照射角度のとき0, 減衰開始角度のとき1になるように補間
				const float falloffAngle = colorAndFalloffAngle.w;
				const float a = min((coneAngle - angle) / (coneAngle - falloffAngle), 1);
				illuminance *= a;

			} // if coneAngle

		 // ライトの最大距離を制限
		 const float radius = positionAndRadius.w;
		 const float smoothFactor = clamp(1 - pow(dist / radius, 4), 0, 1);
		 illuminance *= smoothFactor * smoothFactor;

		 // 逆2乗の法則によって明るさを減衰させる
		 // 0除算が起きないように1を足している
		 illuminance /= sqrDistance + 1;
		 // 拡散光の明るさを計算
		 return colorAndFalloffAngle.xyz * illuminance;
}

void main()
{
	 vec4 c = texture(texColor, inTexcoord);
	 if (alphaCutoff > 0)
	 {
		// カットオフ値が指定されている場合、アルファがその値未満の場合は描画をキャンセル
		if (c.a < alphaCutoff) {
		  discard; // フラグメントを破棄(キャンセル)
		}
		   c.a = 1; // 破棄されなかったら不透明にする
	 }

	 // テクスチャのガンマ補正を解除
	 const float crtGamma = 2.2; // CRTモニターのガンマ値
	 c.rgb = pow(c.rgb, vec3(2.2));
	 
	 outColor = c * color;
	 
	 // 線形補間によって長さが1ではなくなっているので、正規化して長さを1に復元する
	vec3 normal = normalize(inNormal);


     vec3 diffuse = vec3(0); // 拡散光の明るさの合計
	 //平行光源にも対応できるようにするために法線の正規化をなくす
	
    for (int i = 0; i < 12; i++)
	{
		if (i >= lightCount) {
			break;
		}

	//for文の添え字を使うと、別の数字が入ってしまい
	//中身がいかれてしまうので、switch文によるべた書きな指定を繰り返すことで
	//処理を行うことに

	//マクロ定義は一文に済ませる考え方があるので出来るだけ一文に
	//もし、別の行にしたいときは￥を使う
	//￥：改行しても一文扱いする

#define CALC_LIGHT(i) \
	diffuse += CalcLight( \
		light.colorAndFalloffAngle[i], \
		light.positionAndRadius[i], \
		light.directionAndConeAngle[i], \
		normal);

	switch (i) {
		case 0: CALC_LIGHT(0); break;
		case 1: CALC_LIGHT(1); break;
		case 2: CALC_LIGHT(2); break;
		case 3: CALC_LIGHT(3); break;
		case 4: CALC_LIGHT(4); break;
		case 5: CALC_LIGHT(5); break;
		case 6: CALC_LIGHT(6); break;
		case 7: CALC_LIGHT(7); break;
		case 8: CALC_LIGHT(8); break;
		case 9: CALC_LIGHT(9); break;
		case 10: CALC_LIGHT(10); break;
		case 11: CALC_LIGHT(11); break;
		}
	} // for lightCount
	
	
	// 影を計算
	//フラグメントより光源側に物体が存在する場合は0
	//存在しない場合は１が返される
	float shadow = texture(texShadow, inShadowTexcoord).r;

	// 平行光源の明るさを計算
	//光線の向きを入れているdirection変数に
	//フラグメントから見た光の向きを入れる必要があるので、向きを逆に
	float theta = max(dot(-directionalLight.direction, normal), 0);
	float illuminance = theta / 3.14159265;
	diffuse += directionalLight.color * illuminance * shadow;

	// アンビエントライトの明るさを計算
	diffuse += ambientLight;


	// 拡散光の影響を反映
	// お互いの当たる光を足すことでよりリアルの光描写を作っている
	outColor.rgb *= diffuse ;
 
	// 発光色を反映
	//emissionのwは「エミッションテクスチャの有無」を判定
	//

	if (emission.w > 0) 
	{
	  outColor.rgb += texture(texEmission, inTexcoord).rgb * emission.rgb;
	 }else{
	  outColor.rgb += emission.rgb;
	}
	// ガンマ補正を行う
	outColor.rgb = pow(outColor.rgb, vec3(1 / 2.2));
}