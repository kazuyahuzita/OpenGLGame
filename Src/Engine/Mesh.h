/**
* @file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "glad/glad.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "Texture.h"
#include "VecMath.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
// ��s�錾
class MeshBuffer;
using MeshBufferPtr = std::shared_ptr<MeshBuffer>;


/**
* ���_�f�[�^�`��
*/
struct Vertex
{
	vec3 position; // ���_���W
	vec2 texcoord; // �e�N�X�`�����W
	vec4 texcoord4; // �e�N�X�`�����W
	vec3 normal;	//�@���x�N�g��
};

/**
* �}�e���A��
*/
struct Material
{
	std::string name = "<Default>";  // �}�e���A����
	vec4 baseColor = { 1, 1, 1, 1 }; // ��{�F+�A���t�@
	vec3 emission = { 0, 0, 0 };     // �����F
	TexturePtr texBaseColor;         // ��{�F�e�N�X�`��
	TexturePtr texEmission;          // �����F�e�N�X�`��
};
using MaterialPtr = std::shared_ptr<Material>;
using MaterialList = std::vector<MaterialPtr>; // �}�e���A���z��^

/**
* �`��p�����[�^
*/
struct DrawParams
{
	GLenum mode = GL_TRIANGLES; // �v���~�e�B�u�̎��
	GLsizei count = 0;          // �`�悷��C���f�b�N�X��	
	const void* indices = 0;    // �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g	
	GLint baseVertex = 0;       // �C���f�b�N�X0�ƂȂ钸�_�z����̈ʒu	
	int materialNo = -1;        // �}�e���A���C���f�b�N�X
};


/**
* 3D���f��
*/
struct StaticMesh
{
	std::string name;    // ���b�V����	
	std::vector<DrawParams> drawParamsList; // �`��p�����[�^	
	MaterialList materials; // ���L�}�e���A���z��
};
using StaticMeshPtr = std::shared_ptr<StaticMesh>;

void Draw(const StaticMesh& mesh, GLuint program, const MaterialList& materials);
//�X�P���^�����b�V���ł��ł���悤��
//�`��p�����[�^�z�����������v���O�����𕪗���
//�ǂ���̃��b�V���ł��`��ł���悤�ɂ���
void Draw(const std::vector<DrawParams>&drawParamsList, GLuint program,
	const MaterialList & materials, const vec4 * objectColor);

//���Ԗڂ̍��W�ϊ��s����g�����Ƃ����f�[�^�������_�f�[�^�^�̒ǉ�
/**
* �֐ߕt�����_�f�[�^�`��
*/
struct SkeletalVertex
{
	vec3 position; // ���_���W
	vec2 texcoord; // �e�N�X�`�����W
	vec3 normal;   // �@���x�N�g��
	//�����̊֐߂���̉e�����Č����邽�߂ɕω����Ă���
	uint16_t joints[4];  // �e�����󂯂�֐߂̔ԍ�
	uint16_t weights[4]; // �e�֐߂̉e���x
};

/**
* �֐ߕt�����b�V��
*/
struct SkeletalMesh
{
	std::string name;    // ���b�V����	
	std::vector<DrawParams> drawParamsList; // �`��p�����[�^	
	MaterialList materials; // ���L�}�e���A��	
};
using SkeletalMeshPtr = std::shared_ptr<SkeletalMesh>;

// ���L�}�e���A���z��𕡐�����
inline MaterialList CloneMaterialList(const StaticMeshPtr& original)
{
	MaterialList clone(original->materials.size());
	for (int i = 0; i < clone.size(); ++i) {
		clone[i] = std::make_shared<Material>(*original->materials[i]);

	}
	return clone;
}
/**
* ���_�f�[�^���Ǘ�����N���X
*/
class MeshBuffer
{
public:
	/**
	* ���b�V���o�b�t�@���쐬����
	*
	* @param bufferSize �i�[�ł��钸�_�f�[�^�̃T�C�Y(�o�C�g��)
	*/
	static MeshBufferPtr Create(size_t bufferSize)
	{
		return std::make_shared<MeshBuffer>(bufferSize);
	}

	// �R���X�g���N�^�E�f�X�g���N�^
	MeshBuffer(size_t bufferSize);
	~MeshBuffer() = default;

	// �R�s�[�Ƒ�����֎~
	MeshBuffer(const MeshBuffer&) = delete;
	MeshBuffer& operator=(const MeshBuffer&) = delete;

	
	/**
	* �e�N�X�`���쐬�R�[���o�b�N�^
	*
	* �����Ɏw�肳�ꂽ�t�@�C��������e�N�X�`�����쐬����^
	*/
	using TextureCallback = std::function<TexturePtr(const char*)>;

	
	/**
	* �e�N�X�`���쐬�R�[���o�b�N��ݒ肷��
	*
	* @param callback �ݒ肷��R�[���o�b�N�I�u�W�F�N�g
	*/
	void SetTextureCallback(const TextureCallback& callback)
	{
		textureCallback = callback;
	}
	/**
	* OBJ�t�@�C����ǂݍ���
	*
	* @param filename OBJ�t�@�C����
	*
	* @return filename����쐬�������b�V��
	*/

	StaticMeshPtr LoadOBJ(const char* filename);

	// �֐߃f�[�^
	struct Skeleton
	{

	};
	
	/**
	* OBJ�t�@�C����ǂݍ��݊֐߂�ݒ肷��
	*
	* @param filename OBJ�t�@�C����
	* @param skeleton �֐߃f�[�^
	*
	* @return filename����쐬�����֐ߕt�����b�V��
	*/
	SkeletalMeshPtr LoadOBJ(
			const char* filename, const Skeleton & skeleton);
	/**
	* ���_�f�[�^�̒ǉ�
	*
	* @param vertices    GPU�������ɃR�s�[���钸�_�f�[�^�z��
	* @param vertexBytes vertices�̃o�C�g��
	* @param indices     GPU�������ɃR�s�[����C���f�b�N�X�f�[�^�z��
	* @param indexBytes  indices�̃o�C�g��
	* @param stride      ���_�f�[�^�̊Ԋu(�o�C�g��)
	* @param mode        �v���~�e�B�u�̎��
	*/
	void AddVertexData(const void* vertices, size_t vertexBytes,
		const uint16_t * indices, size_t indexBytes,
		size_t stride = sizeof(Vertex), GLenum mode = GL_TRIANGLES);
	
	/**
	* �����̕`��p�����[�^�ƃe�N�X�`������V�����X�^�e�B�b�N���b�V�����쐬����
	*
	* @param name         ���b�V����
	* @param params       ���b�V���̌��ɂȂ�`��p�����[�^
	* @param texBaseColor ���b�V���ɐݒ肷��x�[�X�J���[�e�N�X�`��
	*
	* @return �쐬�����X�^�e�B�b�N���b�V��
	*/
	StaticMeshPtr CreateStaticMesh(const char* name,
		const DrawParams & params, const TexturePtr & texBaseColor);


	// �S�Ă̒��_�f�[�^���폜
	void Clear();

	/**
	* �X�^�e�B�b�N���b�V���̎擾
	*
	* @param name �X�^�e�B�b�N���b�V���̖��O
	*
	* @return ���O��name�ƈ�v����X�^�e�B�b�N���b�V��
	*/
	StaticMeshPtr GetStaticMesh(const char* name) const;

	/**
	* �X�P���^�����b�V���̎擾
	*
	* @param name �X�P���^�����b�V���̖��O
	*
	* @return ���O��name�ƈ�v����X�P���^�����b�V��
	*/
	SkeletalMeshPtr GetSkeletalMesh(const char* name) const;
	// �`��p�����[�^�̎擾
	const DrawParams& GetDrawParams(size_t index) const
	{
		return drawParamsList[index];
	}

	// �`��p�����[�^�̐����擾
	size_t GetDrawParamsCount() const { return drawParamsList.size(); }

	// VAO�̎擾
	VertexArrayObjectPtr GetVAO() const { return vao; }
	VertexArrayObjectPtr GetVAOSkeletal() const { return vaoSkeletal; }

private:
	std::vector<MaterialPtr> LoadMTL(
		const std::string & foldername, const char* filename);

	
	// ���b�V���\�z�p�̒��ԃf�[�^
	struct MeshData
	{
		std::vector<Vertex> vertices;  // ���_�f�[�^
		std::vector<uint16_t> indices; // �C���f�b�N�X�f�[�^
		std::vector<DrawParams> drawParamsList; // �`��p�����[�^�z��
		MaterialList materials;                 // �}�e���A���z��

	};
	MeshData CreateMeshDataFromOBJ(const char* filename);


	std::unordered_map<std::string, StaticMeshPtr> meshes;

	std::unordered_map<std::string, SkeletalMeshPtr> skeletalMeshes;

	std::vector<DrawParams> drawParamsList; // �`��p�����[�^�z��
	VertexArrayObjectPtr vao; // ���_�A�g���r���[�g�z��
	VertexArrayObjectPtr vaoSkeletal; // �X�P���^�����b�V���p�̒��_�z��
	BufferObjectPtr buffer;   // ���_�f�[�^����уC���f�b�N�X�f�[�^
	size_t usedBytes = 0;     // �o�b�t�@�̎g�p�ςݗe��(�o�C�g)
	TextureCallback textureCallback; // �e�N�X�`���쐬�R�[���o�b�N
};


/**
* �����Ă���@����₤
*
* @param vertices    ���_�z��
* @param vertexCount ���_�z��̗v�f��
* @param indices     �C���f�b�N�X�z��
* @param indexCount  �C���f�b�N�X�z��̗v�f��
*/
void FillMissingNormals(
	Vertex * vertices, size_t vertexCount,
	const uint16_t * indices, size_t indexCount);

#endif // MESH_H_INCLUDED