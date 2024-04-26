/**
* @file Particle.h
*/
#ifndef PARTICLE_H_INCLUDED
 #define PARTICLE_H_INCLUDED
 #include "glad/glad.h"
 #include "Engine/Texture.h"
 #include "Engine/Mesh.h"
 #include "Engine/VecMath.h"
 #include <vector>
 #include <memory>
 #include <random>

// ��s�錾
class Particle;
class ParticleEmitter;
using ParticleEmitterPtr = std::shared_ptr<ParticleEmitter>;
class ParticleManager;
using ParticleManagerPtr = std::shared_ptr<ParticleManager>;
class VertexArrayObject;
using VertexArrayObjectPtr = std::shared_ptr<VertexArrayObject>;
/**
* �l�̕�ԕ��@
*/
enum class InterporationType
 {	
	linear,    // ���`���		
	easeIn,    // ���X�ɉ���		
	easeOut,   // ���X�Ɍ���		
	easeInOut, // ���X�ɉ������A���X�Ɍ�������		
	none,      // ��Ԃ��Ȃ�		
};

//
/**
* �l���Ԃ���@�\�����N���X
*/
template<typename T>
class Interporator
{
public:
	Interporator() = default;
	~Interporator() = default;
	explicit Interporator(const T& start) : start(start), end(start) {}
	Interporator(const T& start, const T& end) : start(start), end(end) {}

	// �l��ݒ肷��
	void Set(const T& start, const T& end) {
		this->start = start;
		this->end = end;

	}

	// ����t(�͈�0.0�`1.0)�ŕ�Ԃ����l��Ԃ�
	T Interporate(float t) const {
		switch (type) {
		default:
		case InterporationType::linear: return start + t * (end - start);
		case InterporationType::none:   return start;

		}

	}

	T start; // ��ԊJ�n���̒l
	T end;   // ��ԏI�����̒l
	InterporationType type = InterporationType::linear; // ��ԕ��@
};
using Interporator1f = Interporator<float>;
using Interporator2f = Interporator<vec2>;
using Interporator3f = Interporator<vec3>;
using Interporator4f = Interporator<vec4>;

/**
* �p�[�e�B�N���̓�����`����p�����[�^
*/
struct ParticleParameter
 {
	float lifetime = 1;           // ��������
	vec4 tc = { 0, 0, 1, 1 }; // �e�N�X�`�����W
	Interporator1f radial = Interporator1f(10);             // ���a�����̑��x
	Interporator3f velocity = Interporator3f({ 0, 0, 0 });    // ���x
	Interporator2f scale = Interporator2f({ 1, 1 });       // �傫��
	Interporator1f rotation = Interporator1f(0);              // ��]
	Interporator4f color = Interporator4f({ 1, 1, 1, 1 }); // �F�ƕs�����x
};


/**
* �G�~�b�^�[�̌`��
*/
enum class ParticleEmitterShape
{
	sphere, // ��
	box,    // ������
};


/**
* �p�[�e�B�N���E�G�~�b�^�[�̃p�����[�^�[
*/
struct ParticleEmitterParameter
{
	std::string name;      // �G�~�b�^�[��
	float gravity = -9.8f; // �d��

	// �摜�ƃu�����h�W��
	std::string imagePath;          // �e�N�X�`���E�t�@�C����
	vec2 tiles = { 1, 1 }; // �e�N�X�`���̏c�Ɖ��̕�����
	GLenum srcFactor = GL_SRC_ALPHA;           // �u�����h�W��
	GLenum dstFactor = GL_ONE_MINUS_SRC_ALPHA; // �u�����h�W��

	// ���o���Ԃƕ��o�p�x
	float duration = 1.0f; // �p�[�e�B�N������o����b��
	bool loop = false; // duration�o�ߌ�̓���(true=���[�v, false=��~)
	float emissionsPerSecond = 10; // �b������̃p�[�e�B�N�����o��

	// ���o�͈͂̌`��
	//�`��𖳖����p�̂Ƃ��Ē�`
	//���p�̂̃����o���L���Ȃ̂���shape�����o�ϐ��Ŏw��
	vec3 position = { 0, 0, 0 }; // ���W
	vec3 rotation = { 0, 0, 0 }; // ��]
	ParticleEmitterShape shape = ParticleEmitterShape::sphere; // �`��
	float randomizeDirection = 0; // ���o�����̗�����(0=�Ȃ� 1=���S�����_��)
	union {
		struct Sphere {
			float radius = 1;    // ���̔��a
			float thickness = 1; // ���k�̌���(0=�Ŕ� 1=���S�܂ł�������)

		} sphere;
		struct Box {
			vec3 size = { 1, 1, 1 }; // �����̂̃T�C�Y

		} box;

	};


	// �R���X�g���N�^�E�f�X�g���N�^
	ParticleEmitterParameter() : sphere({ 1, 1 }) {}
	~ParticleEmitterParameter() = default;

	// ���o�͈͂ɋ����w�肷��
	void SetSphereShape(float radius, float thickness) {
		shape = ParticleEmitterShape::sphere;
		sphere.radius = radius;
		sphere.thickness = thickness;

	}

	// ���o�͈͂ɒ����̂��w�肷��
	void SetBoxShape(const vec3& size) {
		shape = ParticleEmitterShape::box;
		box.size = size;

	}
};


/**
* �p�[�e�B�N��
*/
class Particle
 {
public:
	friend ParticleManager;
	friend ParticleEmitter;
	
	Particle() = default;
	~Particle() = default;
	
		  // �������Ԃ̔䗦
		float GetLifetimeRatio() const {
		const float t = 1 - lifetime / pp->lifetime;
		return std::clamp(t, 0.0f, 1.0f);
		
	}
	
		  // true=���S, false=����
		bool IsDead() const { return lifetime <= 0; }
	
private:
	  // �ω����Ȃ��p�����[�^
	const ParticleParameter * pp;          // ��{�p�����[�^
	vec3 radialDirection = { 1, 0, 0 }; // ���S����̌���
	Interporator3f velocity = Interporator3f({ 0, 0, 0 }); // ���x
	vec4 tc = { 0, 0, 1, 1 };         // �e�N�X�`�����W
	
	  // �ω�����p�����[�^
	float lifetime = 0;                   // ��������
	vec3 position = { 0, 0, 0 }; // ���W
	float gravityAccel = 0;               // �d�͂ɂ������x�̍��v
	float gravitySpeed = 0;               // �d�͂ɂ�鑬�x�̍��v
	//�������`������鎞�́A�u�������O�v�̏��ŕ`�悷�邽��
	//viewDepth�������ă\�[�g���܂�
	float viewDepth = 0;                  // ���_����̐[�x�l
};


/**
* �p�[�e�B�N�����o�N���X
*/
class ParticleEmitter
{
public:
	friend ParticleManager;

	ParticleEmitter(const ParticleEmitterParameter& ep,
		const ParticleParameter& pp, ParticleManager* pManager);
	~ParticleEmitter() = default;
	ParticleEmitter(const ParticleEmitter&) = delete;
	ParticleEmitter& operator=(const ParticleEmitter&) = delete;

	void Update(const mat4& matView, float deltatime);
	void Die() { ep.loop = false; ep.duration = 0; }
	bool IsDead() const { return !ep.loop && timer >= ep.duration && particles.empty(); }

	// �p�����[�^�̎擾�E�ݒ�
	ParticleEmitterParameter& GetEmitterParameter() { return ep; }
	const ParticleEmitterParameter& GetEmitterParameter() const { return ep; }
	ParticleParameter& GetParticleParameter() { return pp; }
	const ParticleParameter& GetParticleParameter() const { return pp; }

private:
	void AddParticle();

	ParticleEmitterParameter ep; // �G�~�b�^�[�̃p�����[�^�[
	ParticleParameter pp;        // �p�[�e�B�N���̃p�����[�^�[
	ParticleManager* pManager = nullptr; // �Ǘ��N���X�̃A�h���X

	TexturePtr texture;      // �p�[�e�B�N���p�e�N�X�`��
	float interval = 0;      // �p�[�e�B�N���̔����Ԋu(�b)
	float timer = 0;         // ���o�ߎ���(�b)
	float emissionTimer = 0; // �p�[�e�B�N�������^�C�}�[(�b)
	float viewDepth = 0;     // ���_����̐[�x�l

	std::vector<Particle*> particles; // �p�[�e�B�N�����X�g
	GLsizei dataCount = 0; // �`�悷��p�[�e�B�N����
	size_t dataOffset = 0; // �`��̊�ƂȂ钸�_�̃I�t�Z�b�g
};


/**
* �p�[�e�B�N���Ǘ��N���X
*/
class ParticleManager
 {
public:
	static ParticleManagerPtr Create(size_t maxParticleCount);
	
	explicit ParticleManager(size_t maxParticleCount);
	~ParticleManager();
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager & operator=(const ParticleManager&) = delete;
	
	void Update(const mat4 & matView, float deltatime);
	void UpdateSSBO();
	void Draw(const mat4 & matProj, const mat4 & matView);
	
	  // �G�~�b�^�[�̊Ǘ�
	ParticleEmitterPtr AddEmitter(
		const ParticleEmitterParameter & ep, const ParticleParameter & pp);
	ParticleEmitterPtr FindEmitter(const std::string & name) const;
	void RemoveEmitter(const ParticleEmitterPtr&);
	void RemoveEmitterAll();
	
	  // �p�[�e�B�N���̊Ǘ�
	Particle * AllocateParticle();
	void DeallocateParticle(Particle * p);
	
		  // �����̐���
		int RandomInt(int min, int max) {
		return std::uniform_int_distribution<>(min, max)(randomEngine);
		
	}
	 float RandomFloat(float min, float max) {
		return std::uniform_real_distribution<float>(min, max)(randomEngine);
		
	}
	
private:
	std::vector<ParticleEmitterPtr> emitters; // �G�~�b�^�[�̃��X�g
	//�p�[�e�B�N���͈�x�ɑ�ʂɁA��������̂ŃX�}�[�g�|�C���^
	//���g���͔̂�������
	//����́A�p�[�e�B�N���V�X�e���̓����ŊǗ����s����̂�
	//�X�}�[�g�|�C���^�Ȃ��ł��A��r�I�ȒP�ɏ����ł��A���������ɂȂ���
	std::vector<Particle> particles; // �p�[�e�B�N���̃��X�g
	std::vector<Particle*> freeParticles; // ���g�p�p�[�e�B�N���̃��X�g
	
	size_t maxParticleCount = 0; // �����\�ȍő�p�[�e�B�N����
	BufferObjectPtr vbo;
	BufferObjectPtr ibo;
	VertexArrayObjectPtr vao;
	GLuint vsParticle = 0;			// ���_�V�F�[�_�̊Ǘ��ԍ�
	GLuint fsParticle = 0;			// �t���O�����g�V�F�[�_�̊Ǘ��ԍ�
	GLuint prog3D = 0;		// �V�F�[�_�v���O�����̊Ǘ��ԍ�
	BufferObjectPtr ssbo;
	MeshBufferPtr meshBuffer;   // �}�`�f�[�^�Ǘ��I�u�W�F�N�g
	std::mt19937 randomEngine;
};

#endif // PARTICLE_H_INCLUDED