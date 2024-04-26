/**
* @file Particle.cpp
*/
#include "Particle.h"
 #include "Engine/Debug.h"
 #include <algorithm>
#include "Engine/Engine.cpp"


// ���_�f�[�^�\����
struct ParticleVertex
{	float x, y; // ���W(XY�̂�)	
	float u, v; // �e�N�X�`�����W	
};

// �摜�\���p�f�[�^���i�[����\����(float*16��=64�o�C�g)
struct ParticleData
 {
	vec4 position; // ���W(w�v�f�͖��g�p)
	vec2 scale;    // �g��k��
	float cos;     // Z����]��cos
	float sin;     // Z����]��sin
	vec4 color;    // �F�ƕs�����x
	vec4 texcoord; // �e�N�X�`�����W
};
/**
* �R���X�g���N�^
*
* @param ep       �G�~�b�^�[�̏������p�����[�^
* @param pp       �p�[�e�B�N���̏������p�����[�^
* @param pManager �Ǘ��I�u�W�F�N�g�̃A�h���X
*/
ParticleEmitter::ParticleEmitter(const ParticleEmitterParameter& ep,
	const ParticleParameter& pp, ParticleManager* pManager) :
	ep(ep), pp(pp), pManager(pManager), interval(1.0f / ep.emissionsPerSecond)
{
	//texture = Texture::Create(ep.imagePath.c_str(), GL_LINEAR);
}


/**
* �G�~�b�^�[�̊Ǘ����ɂ���p�[�e�B�N���̏�Ԃ��X�V����
*
* @param matView    �`��Ɏg�p����r���[�s��
* @param deltaTime  �O��̍X�V����̌o�ߎ���(�b)
*/
void ParticleEmitter::Update(const mat4& matView, float deltaTime)
{
	// �Ǘ��I�u�W�F�N�g�����ݒ�Ȃ牽�����Ȃ�
	if (!pManager) {
		return;

	}

	// ���Ԃ��o�߂�����
	timer += deltaTime;
	if (timer >= ep.duration) {
		if (ep.loop) {
			timer -= ep.duration;
			emissionTimer -= ep.duration;

		}
		else {
			timer = ep.duration;

		}

	}
	// �\�[�g�Ɏg���u���_����̐[�x�l�v���v�Z
	viewDepth = (matView * vec4(ep.position, 1)).z;


	// �p�[�e�B�N���̕��o
	for (; timer - emissionTimer >= interval; emissionTimer += interval) {
		AddParticle();

	}

	// �p�[�e�B�N���̍X�V
	for (auto e : particles) {
		if (e->lifetime <= 0) {
			continue;

		}
		e->lifetime -= deltaTime;

		// ���x���v�Z
		const float t = e->GetLifetimeRatio();
		vec3 velocity = e->velocity.Interporate(t);

		// ���a�����̑��x���v�Z
		velocity += e->radialDirection * e->pp->radial.Interporate(t);

		// �d�͂ɂ������x�Ƒ��x���X�V
		e->gravityAccel += ep.gravity * deltaTime; // �����x
		e->gravitySpeed += e->gravityAccel * deltaTime; // ���x
		velocity.z += e->gravitySpeed;

		// ���W���X�V
		e->position += velocity * deltaTime;

		// �\�[�g�Ɏg���u���_����̐[�x�l�v���v�Z
		e->viewDepth = (matView * vec4(e->position, 1)).z;

	}

	// ����ł���p�[�e�B�N�����폜
	auto itr = std::partition(particles.begin(), particles.end(),
		[](const Particle* p) { return !p->IsDead(); });

	for (auto i = itr; i != particles.end(); ++i) {
		pManager->DeallocateParticle(*i); // �p�[�e�B�N����ԋp

	}
	particles.erase(itr, particles.end());
	
	  // �p�[�e�B�N�����\�[�g
	std::sort(particles.begin(), particles.end(),
		[](const Particle* a, const Particle* b)
				{ return a->viewDepth < b->viewDepth; });


}


/**
* �p�[�e�B�N����ǉ�����
*/
void ParticleEmitter::AddParticle()
{
	// �p�[�e�B�N�����m�ۂł��Ȃ���Ή������Ȃ�
	Particle* particle = pManager->AllocateParticle();
	if (!particle) {
		return;

	}

	
	  // ���[�J�����o���W���v�Z
	vec4 position = { 0, 0, 0, 1 };
	switch (ep.shape) {
	case ParticleEmitterShape::sphere: {
		const float cosTheta = pManager->RandomFloat(-1, 1);
		const float sinTheta = std::sqrt(1 - cosTheta * cosTheta);
		const float phi = pManager->RandomFloat(0, 2 * pi);
		// ���̑̐ς̌���4/3��r^3����A���a���Ƃ̕��o�p�x�͔��a��3�捪�ŋ��߂���
		// thickness��3�悷�邱�ƂŁA3�捪��������Ƃ��ŏ��l��thickness�ƈ�v����
		const float t = ep.sphere.thickness;
		const float r0 = pManager->RandomFloat(1 - t * t * t, 1);
		//�O�捪�֐���cbrt�ɂ���āA���S����̋���r��ό`���Ă���
		const float r = std::cbrt(r0) * ep.sphere.radius;
		position.x = r * sinTheta * std::cos(phi);
		position.y = r * sinTheta * std::sin(phi);
		position.z = r * cosTheta;
		break;
		}
	case ParticleEmitterShape::box:
		position.x = pManager->RandomFloat(-ep.box.size.x, ep.box.size.x);
		position.y = pManager->RandomFloat(-ep.box.size.y, ep.box.size.y);
		position.z = pManager->RandomFloat(-ep.box.size.z, ep.box.size.z);
		break;
		
	}
	
	  // Z->X->Y�̏��ŉ�]��K�p���A���o�����֌�����s����v�Z
	const mat4 matRot = RotateY(ep.rotation.y) *
	RotateX(ep.rotation.x) * RotateZ(ep.rotation.z);
	
	// �p�[�e�B�N���̃����o�ϐ���������
	particle->pp = &pp;
	particle->lifetime = pp.lifetime;
	particle->position = ep.position + vec3(matRot * position);
	particle->radialDirection = normalize(particle->position - ep.position);
	particle->gravityAccel = 0;
	particle->gravitySpeed = 0;
	particle->viewDepth = 0;

	
	  // ���������_���W����0�łȂ���΁A�ړ������Ƀ����_����������
	if (ep.randomizeDirection) {
	    // �ړ������������_��������s����v�Z
		const float randomRange = pi * ep.randomizeDirection;
	const mat4 matRandom =
		RotateY(pManager->RandomFloat(-randomRange, randomRange)) *
		RotateX(pManager->RandomFloat(-randomRange, randomRange)) *
		RotateZ(pManager->RandomFloat(-randomRange, randomRange));
		matRot * matRandom; // ��]�ƃ����_��������
	
		    // ���a�����������_����
		particle->radialDirection = vec3(matRandom * vec4(particle->radialDirection, 1));
	
	}
	// ���x�x�N�g������]
	particle->velocity.start = vec3(matRot * vec4(pp.velocity.start, 1));
	particle->velocity.end = vec3(matRot * vec4(pp.velocity.end, 1));

		
	//  // �^�C������1��葽���ꍇ�A�����_���Ƀ^�C����I������
	//if (ep.tiles.x > 1 || ep.tiles.y > 1)
	//{
	//const vec2 invTiles = 1.0f / ep.tiles;
	//const int u = pManager->RandomInt(0, static_cast<int>(ep.tiles.x) - 1);
	//const int v = pManager->RandomInt(0, static_cast<int>(ep.tiles.y) - 1);
	//particle->tc.u = static_cast<float>(u) * invTiles.x;
	//particle->tc.v = static_cast<float>(v) * invTiles.y;
	//particle->tc.sx = invTiles.x;
	//particle->tc.sy = invTiles.y;
	//
	//}
	// �G�~�b�^�[�̊Ǘ����X�g�Ƀp�[�e�B�N����ǉ�
	particles.push_back(particle);

}


/**
* �p�[�e�B�N���Ǘ��I�u�W�F�N�g���쐬����
*
* @param maxParticleCount �\���\�ȃp�[�e�B�N���̍ő吔
*/
ParticleManagerPtr ParticleManager::Create(size_t maxParticleCount)
 {
	return std::make_shared<ParticleManager>(maxParticleCount);
}


/**
* �R���X�g���N�^
*
* @param maxParticleCount �\���\�ȃp�[�e�B�N���̍ő吔
*/
ParticleManager::ParticleManager(size_t maxParticleCount) :
	maxParticleCount(maxParticleCount)
{


	const ParticleVertex vertexData[] = {
	{-0.5f,-0.5f, 0, 1},
	{ 0.5f,-0.5f, 1, 1},
	{ 0.5f, 0.5f, 1, 0},
	{-0.5f, 0.5f, 0, 0},
	};
	vbo = BufferObject::Create(sizeof(vertexData), vertexData);

	const GLushort indexData[] = { 0, 1, 2, 2, 3, 0 };
	ibo = BufferObject::Create(sizeof(indexData), indexData);

	glBindVertexArray(*meshBuffer->GetVAO());
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex),
		(const void*)offsetof(ParticleVertex, u));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// �V�F�[�_��ǂݍ���ŃR���p�C��
	vsParticle = CompileShader(GL_VERTEX_SHADER, "Res/standard.vert");
	fsParticle = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");

	glProgramUniform1f(maxParticleCount * sizeof(ParticleData),
		GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

	// �K�v�ȃ������̈���m��
	emitters.reserve(100);
	particles.resize(maxParticleCount);

	// ���g�p�p�[�e�B�N�����X�g���쐬
	freeParticles.resize(maxParticleCount);
	auto itr = particles.end();
	for (auto& e : freeParticles) {
		--itr;
		e = &*itr;

	}

	// ������������
	std::random_device rd;
	randomEngine.seed(rd());
}


/**
* �f�X�g���N�^
*/
ParticleManager::~ParticleManager()
{
	// ���ׂẴG�~�b�^�[����}�l�[�W���̃A�h���X����������
	for (auto& e : emitters) {
		e->pManager = nullptr;

	}
}


/**
* �p�[�e�B�N���̏�Ԃ��X�V����
*
* @param  matView �`��Ɏg�p����r���[�s��
* @param deltaTime  �O��̍X�V����̌o�ߎ���(�b)
*/
void ParticleManager::Update(const mat4& matView, float deltaTime)
{
	if (emitters.empty()) {
		return;

	}

	// �G�~�b�^�[�̍X�V
	for (auto& e : emitters) {
		e->Update(matView, deltaTime);

	}

	// ����ł���G�~�b�^�[���폜
	auto itr = std::stable_partition(emitters.begin(), emitters.end(),
		[](const ParticleEmitterPtr& e) { return !e->IsDead(); });
	for (auto i = itr; i != emitters.end(); ++i) { (*i)->pManager = nullptr; }
	emitters.erase(itr, emitters.end());

	// �G�~�b�^�[���\�[�g
	std::stable_sort(emitters.begin(), emitters.end(),
		[](const ParticleEmitterPtr& a, const ParticleEmitterPtr& b) {
			return a->viewDepth < b->viewDepth; });
}