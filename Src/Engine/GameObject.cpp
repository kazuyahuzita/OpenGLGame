/**
* @file GameObject.cpp
*/
#include "GameObject.h"
 #include <algorithm>


/**
* デストラクタ
*/
GameObject::~GameObject()
{
	// 死ぬ前に親子関係を解除する
	SetParent(nullptr);
	for (auto child : children) 
	{
		child->parent = nullptr;

	}
}
/**
* ゲームオブジェクトから削除予定のコンポーネントを削除する
*/
void GameObject::RemoveDestroyedComponent()
{
	if (components.empty()) {
		return; // コンポーネントを持っていなければ何もしない

	}

	// 破棄予定の有無でコンポーネントを分ける
	const auto iter = std::stable_partition(
		components.begin(), components.end(),
		[](const auto& p) { return !p->IsDestroyed(); });

	// 破棄予定のコンポーネントを別の配列に移動
	std::vector<ComponentPtr> destroyList(
		std::move_iterator(iter),
		std::move_iterator(components.end()));

	// 配列から移動済みコンポーネントを削除
	components.erase(iter, components.end());
	
	  // コライダーはcollidersにも登録されているので、両方から削除する必要がる
	const auto iter2 = std::remove_if(
		colliders.begin(), 
		colliders.end(),
		[](const auto& p) {
			return p->IsDestroyed(); 
		}
	);
	colliders.erase(iter2, colliders.end());


	// 破棄予定のコンポーネントのOnDestoryを実行
	for (auto& e : destroyList) {
		e->OnDestroy();

	}

	// ここで実際にコンポーネントが削除される(destoyListの寿命が終わるため)
}

/**
* スタートイベント
*/
void GameObject::Start()
{
	for (auto& e : components)
	{
		if (!e->isStarted)
		{
			e->Start();
			e->isStarted = true;
		}
	}
}

/**
* 更新イベント
*
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void GameObject::Update(float deltaTime)
{
	for (auto& e : components)
	{
		e->Update(deltaTime);
	}
	 RemoveDestroyedComponent();
}

/**
* 衝突イベント
*
* @param self  衝突したコンポーネント(自分)
* @param other 衝突したコンポーネント(相手)
*/
void GameObject::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
{
	for (auto& e : components)
	{
		e->OnCollision(self, other);
	}
}

/**
* 削除イベント
*/
void GameObject::OnDestroy()
{
	for (auto& e : components) {
		e->OnDestroy();

	}
}
//メンバ関数を生ポインタとシェアードポインタの両方持っているのは
// プログラムを書きやすくしているため

// 親オブジェクトを設定する
void GameObject::SetParent(GameObject* parent)
{
	// 同じ親を指定された場合は何もしない
	if (parent == this->parent) {
		return;

	}

	// 別の親がある場合、その親との関係を解除する
	if (this->parent) {
		// 自分の位置を検索
		auto& c = this->parent->children;
		auto itr = std::find(c.begin(), c.end(), this);
		if (itr != c.end()) {
			c.erase(itr); // 配列から自分を削除

		}

	}

	// 新たな親子関係を設定
	if (parent) {
		parent->children.push_back(this);

	}
	this->parent = parent;
}
//シェアードポインタ版のメンバ関数の目的は、
// 「getを呼び出す手間を省いてプログラムを書きやすくすること」
// 親オブジェクトを設定する
void GameObject::SetParent(const GameObjectPtr& parent)
{
	SetParent(parent.get());
}