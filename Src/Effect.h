/**
* ���R���|�[�l���g
*/
#ifndef EFFECT_H_INCLUDED
#define EFFECT_H_INCLUDED
#include "Engine/Billboard.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/GameObject.h"
#include "Engine/Random.h"

class Effect : public Component
{
	friend class MistGenerator;
public:
	Effect() = default;
	virtual ~Effect() = default;

	virtual void Update(float deltaTime) override
	{
		// �^�C�}�[�������𒴂����玩�g���폜
		timer += deltaTime;
		if (timer >= lifespan) {
			GetOwner()->Destroy();

		}
	}

private:
	float timer = 0;    // �o�ߎ��ԃ^�C�}�[
	float lifespan = 7; // ����(�b)

};
#endif // EFFECT_H_INCLUDED
