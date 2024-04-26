#ifndef MOVEHORRORSCENE_H_INCLUDED
#define MOVEHORRORSCENE_H_INCLUDED
#include "Engine/BoxCollider.h"
#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/VecMath.h"

/**
* HorrorSceneに飛ばす判定を出すオブジェクト
* playerが触れたらシーンに飛ばす判定は任せている
* @param MoveHorrorSceneStateはInsideとOutside
*/
class MoveHorrorScene : public Component
{
public:
	MoveHorrorScene() = default;
	virtual ~MoveHorrorScene() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

	enum class MoveHorrorSceneState
	{
		Inside,		// 当たってきた
		Outside,	// 出ていった
	};

	MoveHorrorSceneState GetState() const { return mhsState; }

	void SetState(MoveHorrorSceneState _state) {
		mhsState = _state;
	}

private:
	MoveHorrorSceneState mhsState = MoveHorrorSceneState::Outside;
};

#endif // MOVEHORRORSCENE_H_INCLUDED