/**
* @file Light.h
*/
#ifndef LIGHT_H_INCLUDED
 #define LIGHT_H_INCLUDED
 #include "Component.h"
 #include "VecMath.h"

/**
* ライト・コンポーネント
* 
* AwakeでEngineからライトインデックスを取得し、
* Updateで自身に設定された値を
* インデックスに対応するライトに反映されます
* 取得したライトインデックスをEngineからデストラクタで呼ばれたら返す
*/
class Light : public Component
{
public:
	Light() = default;
	virtual ~Light();
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	
	//シェーダー側で効率化した部分を、触りやすいように個別の変数にする
	//データ処理を行わないクラスなので「操作しやすさ」を重視
	  // ライトの種類
	enum class Type
	{
		pointLight, // 点光源
		spotLight,  // スポット光源
	};
	Type type = Type::pointLight; // ライトの種類

	vec3 color = { 1, 1, 1 }; // 色
	float intensity = 1; // 明るさ
	float radius = 1;    // ライトが届く最大半径

	
	// スポットライト用のパラメータ
	// 方向はGameObject::rotationから計算
	float coneAngle = radians(30);    // スポットライトが照らす角度
	float falloffAngle = radians(20); // スポットライトの減衰開始角度

private:
	int lightIndex = -1; // ライトインデックス
};

#endif // LIGHT_H_INCLUDED