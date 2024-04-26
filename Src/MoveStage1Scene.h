#ifndef MOVESTAGE1SCENE_H_INCLUDED
#define MOVESTAGE1SCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* Stage1Sceneに飛ばす判定を出すオブジェクト
* playerが触れたらシーンに飛ばす判定は任せている
* @param MoveStage1SceneはInsideとOutside
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

#endif // MOVESTAGE2SCENE_H_INCLUDED