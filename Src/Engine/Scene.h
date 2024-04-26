/**
* @file Scene.h
*/
#ifndef SCENE_H_INCLUDED
 #define SCENE_H_INCLUDED
 #include <memory>

// ��s�錾
class Engine;
struct Material;
using MaterialPtr = std::shared_ptr<Material>;

/**
* �V�[���̊��N���X
*/
class Scene
{
public:	Scene() = default;
	  virtual ~Scene() = default;	
	  virtual bool Initialize(Engine & engine) { return true; }	
	  virtual void Update(Engine & engine, float deltaTime) {}	
	  virtual void Finalize(Engine & engine) {}	

	  MaterialPtr skysphereMaterial; // �X�J�C�X�t�B�A�p�̃}�e���A��
	  MaterialPtr gateMaterial; // �X�J�C�X�t�B�A�p�̃}�e���A��

};

using ScenePtr = std::shared_ptr<Scene>; // �V�[���|�C���^�^

#endif // SCENE_H_INCLUDED