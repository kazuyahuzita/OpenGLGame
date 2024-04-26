#ifndef UILAYOUT_H_INCLUDED
 #define UILAYOUT_H_INCLUDED
 #include "Component.h"
 #include "VecMath.h"

/**
* UI����ʋ�ԂŔz�u���邽�߂̃R���|�[�l���g
*/
class UILayout : public Component
{
public:
	UILayout() = default;
	virtual ~UILayout() = default;
	
	void Start() override;
	void Update(float deltaTime) override;
	
	vec3 basePosition = { 0, 0, 0 }; // �J�������猩�����W
	vec3 scale = { 1, 1, 1 };    // ���̂̊g�嗦
	vec4 color = { 1, 1, 1, 1 }; // ���̂̐F

};

#endif UILAYOUT_H_INCLUDED