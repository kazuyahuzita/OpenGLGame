/**
* 霧コンポーネント
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
		// タイマーが寿命を超えたら自身を削除
		timer += deltaTime;
		if (timer >= lifespan) {
			GetOwner()->Destroy();

		}
	}

private:
	float timer = 0;    // 経過時間タイマー
	float lifespan = 7; // 寿命(秒)

};
#endif // EFFECT_H_INCLUDED
