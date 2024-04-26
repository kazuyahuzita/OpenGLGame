#ifndef MOVESTAGE2SCENE_H_INCLUDED
#define MOVESTAGE2SCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* Stage2Sceneに飛ばす判定を出すオブジェクト
* playerが触れたらシーンに飛ばす判定は任せている
* @param MoveStage2SceneはInsideとOutside
*/
class MoveStage2Scene : public Component
{
public:
	MoveStage2Scene() = default;
	virtual ~MoveStage2Scene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveStage2SceneState
	{
		Inside,		// 当たってきた
		Outside,	// 出ていった
	};

	MoveStage2SceneState GetState() const { return ms2sState; }

	void SetState(MoveStage2SceneState _state) {
		ms2sState = _state;
	}

private:
	MoveStage2SceneState ms2sState = MoveStage2SceneState::Outside;
};

#endif // MOVESTAGE2SCENE_H_INCLUDED