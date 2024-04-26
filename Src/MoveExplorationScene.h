#ifndef MOVEEXPLORATIONSCENE_H_INCLUDED
#define MOVEEXPLORATIONSCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* ExplorationSceneに飛ばす判定を出すオブジェクト
* playerが触れたらシーンに飛ばす判定は任せている
* @param MoveExplorationSceneStateはInsideとOutside
*/
class MoveExplorationScene : public Component
{
public:
	MoveExplorationScene() = default;
	virtual ~MoveExplorationScene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveExplorationSceneState
	{
		Inside,		// 当たってきた
		Outside,	// 出ていった
	};

	MoveExplorationSceneState GetState() const { return mesState; }

	void SetState(MoveExplorationSceneState _state) {
		mesState = _state;
	}

private:
	MoveExplorationSceneState mesState = MoveExplorationSceneState::Outside;
};

#endif // MOVEEXPLORATIONSCENE_H_INCLUDED