/**
* @file Scene.h
*/
#ifndef SCENE_H_INCLUDED
 #define SCENE_H_INCLUDED
 #include <memory>

// 先行宣言
class Engine;
struct Material;
using MaterialPtr = std::shared_ptr<Material>;

/**
* シーンの基底クラス
*/
class Scene
{
public:	Scene() = default;
	  virtual ~Scene() = default;	
	  virtual bool Initialize(Engine & engine) { return true; }	
	  virtual void Update(Engine & engine, float deltaTime) {}	
	  virtual void Finalize(Engine & engine) {}	

	  MaterialPtr skysphereMaterial; // スカイスフィア用のマテリアル
	  MaterialPtr gateMaterial; // スカイスフィア用のマテリアル

};

using ScenePtr = std::shared_ptr<Scene>; // シーンポインタ型

#endif // SCENE_H_INCLUDED