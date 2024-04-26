#ifndef MOVESTAGE3SCENE_H_INCLUDED
#define MOVESTAGE3SCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* Stage3Sceneに飛ばす判定を出すオブジェクト
* playerが触れたらシーンに飛ばす判定は任せている
* @param MoveStage3SceneはInsideとOutside
*/
class MoveStage3Scene : public Component
{
public:
	MoveStage3Scene() = default;
	virtual ~MoveStage3Scene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveStage3SceneState
	{
		Inside,		// 当たってきた
		Outside,	// 出ていった
	};

	MoveStage3SceneState GetState() const { return ms3sState; }

	void SetState(MoveStage3SceneState _state) {
		ms3sState = _state;
	}

private:
	MoveStage3SceneState ms3sState = MoveStage3SceneState::Outside;
};

#endif // MOVESTAGE2SCENE_H_INCLUDED