#ifndef MOVESTAGESCENE_H_INCLUDED
#define MOVESTAGESCENE_H_INCLUDED
#include "Engine/Component.h"
#include "Engine/VecMath.h"
#include "Engine/BoxCollider.h"
#include "Engine/Engine.h"

#include <functional>
#include<random>

/**
* Playerが触れたらそのヘッダーのisStage1が反応し、AthleticStage1Sceneに飛ばす
*/
class MoveStage1Scene : public Component
{
public:
	MoveStage1Scene() = default;
	virtual ~MoveStage1Scene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveStage1SceneState
	{
		Inside,		// 当たってきた
		Outside,	// 出ていった
	};

	MoveStage1SceneState GetState() const { return ms1sState; }

	void SetState(MoveStage1SceneState _state) {
		ms1sState = _state;
	}

private:
	MoveStage1SceneState ms1sState = MoveStage1SceneState::Outside;
};

#endif // MOVESTAGESCENE_H_INCLUDED