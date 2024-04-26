#ifndef UILAYOUT_H_INCLUDED
 #define UILAYOUT_H_INCLUDED
 #include "Component.h"
 #include "VecMath.h"

/**
* UIを画面空間で配置するためのコンポーネント
*/
class UILayout : public Component
{
public:
	UILayout() = default;
	virtual ~UILayout() = default;
	
	void Start() override;
	void Update(float deltaTime) override;
	
	vec3 basePosition = { 0, 0, 0 }; // カメラから見た座標
	vec3 scale = { 1, 1, 1 };    // 物体の拡大率
	vec4 color = { 1, 1, 1, 1 }; // 物体の色

};

#endif UILAYOUT_H_INCLUDED