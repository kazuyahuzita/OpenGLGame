#define _CRT_SECURE_NO_WARNINGS
#include "Mesh.h"
#include "Debug.h"
#include <numeric>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdio.h>



/**
* ���b�V����`�悷��
*/
void Draw(const StaticMesh& mesh, GLuint program, const MaterialList& materials)
{
	// �J���[�p�����[�^���擾
	vec4 objectColor;
	bool hasUniformColor = false; // �J���[�n���j�t�H�[���ϐ��̗L��

	if (program) {
		hasUniformColor = glGetUniformLocation(program, "color") >= 0;
		if (hasUniformColor) {

			glGetUniformfv(program, 100, &objectColor.x);
		}//if loc
	}

	if (hasUniformColor) {
		Draw(mesh.drawParamsList, program, materials, &objectColor);
		
	}
	else {
		Draw(mesh.drawParamsList, program, materials, nullptr);
		
	}
	
}

//�`��p�����[�^�𒼐ڕ`��
/**
* �`��p�����[�^�z���`�悷��
*/
void Draw(const std::vector<DrawParams>&drawParamsList, GLuint program,
	const MaterialList & materials, const vec4 * objectColor)
{
	for (const auto& e : drawParamsList) {
		// �}�e���A����ݒ�
		if (e.materialNo >= 0 && e.materialNo < materials.size()) {
			const Material & material = *materials[e.materialNo];
			if (objectColor) {
				const vec4 color = *objectColor * material.baseColor;
				
				glProgramUniform4fv(program, 100, 1, &color.x);
				glProgramUniform4f(program, 101,
					material.emission.x, material.emission.y, material.emission.z,
					static_cast<bool>(material.texEmission));

			}
			if (material.texBaseColor) {
				const GLuint tex = *material.texBaseColor;
				glBindTextures(0, 1, &tex);

			}
				//�g��Ȃ��e�N�X�`���C���[�W�X���b�g�Ƀe�N�X�`����
				// �ݒ肵�Ă���ƁA�V�F�[�_�̌������ቺ����ꍇ��
				//�Ȃ̂ŁA�e�N�X�`���C���[�W�X���b�g�̂P�Ԃ𖢐ݒ��
			if (material.texEmission) {
				const GLuint tex = *material.texEmission;
				glBindTextures(1, 1, &tex);
				
			}
			else {
				
				glBindTextures(1, 1, nullptr); // �e�N�X�`��1�𖢐ݒ�ɂ���
				
			}
		}
		glDrawElementsBaseVertex(
			e.mode,
			e.count,
			GL_UNSIGNED_SHORT,
			e.indices,
			e.baseVertex);

	}
}

/**
* MTL�t�@�C����ǂݍ���
*
* @param foldername OBJ�t�@�C���̂���t�H���_��
* @param filename   MTL�t�@�C����
*
* @return MTL�t�@�C���Ɋ܂܂��}�e���A���̔z��
*/
std::vector<MaterialPtr> MeshBuffer::LoadMTL(
	const std::string& foldername, const char* filename)
{
	// MTL�t�@�C�����J��
	const std::string fullpath = foldername + filename;
	std::ifstream file(fullpath);
	if (!file) {
		LOG_ERROR("%s���J���܂���", fullpath.c_str());
		return {};

	}

	// MTL�t�@�C������͂���
	std::vector<MaterialPtr> materials;
	MaterialPtr pMaterial;
	while (!file.eof()) {
		std::string line;
		std::getline(file, line);

		// �}�e���A����`�̓ǂݎ������݂�
		char name[1000] = { 0 };
		if (sscanf(line.data(), " newmtl %999s", name) == 1)
		{
			pMaterial = std::make_shared<Material>();
			pMaterial->name = name;
			materials.push_back(pMaterial);
			continue;

		}

		// �}�e���A������`����Ă��Ȃ��ꍇ�͍s�𖳎�����
		if (!pMaterial) {
			continue;

		}

		// ��{�F�̓ǂݎ������݂�
		if (sscanf(line.data(), " Kd %f %f %f",
			&pMaterial->baseColor.x, &pMaterial->baseColor.y,
			&pMaterial->baseColor.z) == 3) {
			continue;

		}

		// �s�����x�̓ǂݎ������݂�
		if (sscanf(line.data(), " d %f", &pMaterial->baseColor.w) == 1) {
			continue;

		}

		// ��{�F�e�N�X�`�����̓ǂݎ������݂�
		char textureName[1000] = { 0 };
		if (sscanf(line.data(), " map_Kd %999s", &textureName) == 1) {
			pMaterial->texBaseColor =
				textureCallback((foldername + textureName).c_str());
			continue;

		}
		//const std::string filename = foldername + textureName;
		//if (std::filesystem::exists(filename)) {
		//	pMaterial->texBaseColor = 
		//		textureCallback(filename.c_str());
		//}
		
		    // �����F�̓ǂݎ������݂�
		if (sscanf(line.data(), " Ke %f %f %f",
			&pMaterial->emission.x, &pMaterial->emission.y,
			&pMaterial->emission.z) == 3) {
		continue;
		
		}
		
		    // �����F�e�N�X�`�����̓ǂݎ������݂�
		if (sscanf(line.data(), " map_Ke %999s", &textureName) == 1) {
			const std::string filename = foldername + textureName;
			if (std::filesystem::exists(filename)) {
				pMaterial->texEmission =
					textureCallback(filename.c_str());

			}
			else {
				LOG_WARNING("%s���J���܂���", filename.c_str());

			}
			continue;

		} // if map_Ke

	}

	// �ǂݍ��񂾃}�e���A���̔z���Ԃ�
	return materials;
}

/**
* �R���X�g���N�^
*/
MeshBuffer::MeshBuffer(size_t bufferSize)
{
	// �o�b�t�@�I�u�W�F�N�g���쐬
	buffer = BufferObject::Create(bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

	// VAO���쐬
	vao = VertexArrayObject::Create();

	// VBO��IBO��VAO�Ƀo�C���h
	glBindVertexArray(*vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);

	// ���_�A�g���r���[�g��ݒ�
	vao->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
	vao->SetAttribute(1, 2, sizeof(Vertex), offsetof(Vertex, texcoord));
	vao->SetAttribute(2, 3, sizeof(Vertex), offsetof(Vertex, normal));
	
	  // �X�P���^�����b�V���p��VAO���쐬
	vaoSkeletal = VertexArrayObject::Create();
	glBindVertexArray(*vaoSkeletal);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	//position,texcoord,normal�̓X�^�e�B�b�N���b�V���Ɠ����\��
	vaoSkeletal->SetAttribute(0, 3, sizeof(SkeletalVertex),
		offsetof(SkeletalVertex, position));
	vaoSkeletal->SetAttribute(1, 2, sizeof(SkeletalVertex),
		offsetof(SkeletalVertex, texcoord));
	vaoSkeletal->SetAttribute(2, 3, sizeof(SkeletalVertex),
		offsetof(SkeletalVertex, normal));
	glEnableVertexAttribArray(3);

	//glVertexAttribPointer�ƈႢ�A���K���̕K�v�Ȃ�
	//�����l�ő����Ăق����̂ɁA���K������ƕ��������_���ɂȂ��Ă��܂�����
	
	glVertexAttribIPointer(3, 4, GL_UNSIGNED_SHORT, sizeof(SkeletalVertex),
		reinterpret_cast<const void*>(offsetof(SkeletalVertex, joints)));
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_UNSIGNED_SHORT, sizeof(SkeletalVertex),
		reinterpret_cast<const void*>(offsetof(SkeletalVertex, joints)));

	// ��������삪�s���Ȃ��悤�Ƀo�C���h������
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	// �X�^�e�B�b�N���b�V���̗e�ʂ�\��
	meshes.reserve(100);
	// �`��p�����[�^�̗e�ʂ�\��
	drawParamsList.reserve(100);
}


/**
* OBJ�t�@�C����ǂݍ���
*
* @param filename OBJ�t�@�C����
*
* @return filename����쐬�������b�V��
*/
StaticMeshPtr MeshBuffer::LoadOBJ(const char* filename)
{
	// �ȑO�ɓǂݍ��񂾃t�@�C���Ȃ�A�쐬�ς݂̃��b�V����Ԃ�
	{
		//meshes�z��ɂ͂���܂łɓǂݍ��񂾃t�@�C�����L�^����Ă���̂�
		//Find�֐���filename�Ɠ����f�[�^����������
		//�ǂݍ��񂾂��Ƃ�����΁A���̂܂ܕԂ� 
		//�߂�l�𒲂ׂ�΁A�ȑO�ɓǂݍ��񂾂��ǂ�����������
		auto itr = meshes.find(filename);
		//�f�[�^�̈ʒu���z��̏I�[�ƈقȂ��Ă�����u�f�[�^������v�Ƃ�������
		if (itr != meshes.end()) {
			//itr��first�͌����L�[,second�̓f�[�^�{�̂ƂȂ��Ă��邽��
			//second��n��
			return itr->second;

		}
	}
	
	// OBJ�t�@�C�����烁�b�V���f�[�^���쐬
	MeshData meshData = CreateMeshDataFromOBJ(filename);
	if (meshData.vertices.empty()) {
		return nullptr; // �ǂݍ��ݎ��s
		
	}
	  // �ϊ������f�[�^���v���~�e�B�u�o�b�t�@�ɒǉ�
	AddVertexData(
		meshData.vertices.data(), meshData.vertices.size() * sizeof(Vertex),
		meshData.indices.data(), meshData.indices.size() * sizeof(uint16_t));
	
	  // ���b�V�����쐬
	auto pMesh = std::make_shared<StaticMesh>();

	pMesh->drawParamsList.swap(meshData.drawParamsList);
	pMesh->materials.swap(meshData.materials);

	pMesh->name = filename;
	meshes.emplace(filename, pMesh);

	
	 // AddVertexData�ŃR�s�[�����f�[�^�̈ʒu���A�`��p�����[�^�ɔ��f
	const intptr_t baseIndexOffset =
	reinterpret_cast<intptr_t>(drawParamsList.back().indices);
	const GLint baseVertex = drawParamsList.back().baseVertex;
	for (auto& e : pMesh->drawParamsList) {
		e.baseVertex = baseVertex;
		e.indices = reinterpret_cast<const void*>(
			baseIndexOffset + reinterpret_cast<intptr_t>(e.indices));
		
	}
	LOG("%s��ǂݍ��݂܂���(���_��=%d, �C���f�b�N�X��=%d",
		filename, meshData.vertices.size(), meshData.indices.size());

	// �쐬�������b�V����Ԃ�
	return pMesh;

}

/**
* OBJ�t�@�C����ǂݍ��݊֐߂�ݒ肷��
*/
SkeletalMeshPtr MeshBuffer::LoadOBJ(
	const char* filename, const Skeleton& skeleton)
{
	// �ȑO�ɓǂݍ��񂾃t�@�C���Ȃ�A�쐬�ς݂̃��b�V����Ԃ�
	{
		auto itr = skeletalMeshes.find(filename);
		if (itr != skeletalMeshes.end()) {
			return itr->second;

		}
	}

	// OBJ�t�@�C�����烁�b�V���f�[�^���쐬
	MeshData meshData = CreateMeshDataFromOBJ(filename);
	if (meshData.vertices.empty()) {
		return nullptr; // �ǂݍ��ݎ��s

	}
	
	  // Vertex�̋��ʕ�����SkeletaVertex�ɃR�s�[
	std::vector<SkeletalVertex> skeletalVertices(meshData.vertices.size());
	for (int i = 0; i < meshData.vertices.size(); ++i) {
		skeletalVertices[i].position = meshData.vertices[i].position;
		skeletalVertices[i].texcoord = meshData.vertices[i].texcoord;
		skeletalVertices[i].normal = meshData.vertices[i].normal;

	}
		
		  // �֐ߍs��̔ԍ��Ɖe���x��ݒ�(��)
	for (auto& vertex : skeletalVertices) {
		std::fill_n(vertex.joints, 4, 0);
		std::fill_n(vertex.weights, 4, 0);
		vertex.joints[0] =
			static_cast<uint16_t>(std::clamp(vertex.position.y, 0.0f, 3.0f));
		vertex.weights[0] = 65535;

	}

	
	// �ϊ������f�[�^���v���~�e�B�u�o�b�t�@�ɒǉ�
	//�X�^�e�B�b�N���b�V���p��VAO�ƃX�P���^�����b�V���p��VAO�ł̓X�g���C�h���قȂ邽�߁A
	//�X�^�e�B�b�N�p��VAO�Ɠ����X�g���C�h���w�肷��

	AddVertexData(
		skeletalVertices.data(), skeletalVertices.size() * sizeof(SkeletalVertex),
		meshData.indices.data(), meshData.indices.size() * sizeof(uint16_t),
		sizeof(SkeletalVertex));
	
	  // �X�P���^�����b�V�����쐬
	auto pMesh = std::make_shared<SkeletalMesh>();
	//swap�֐��́A�f�[�^���w���|�C���^�^���������邾����	
	pMesh->drawParamsList.swap(meshData.drawParamsList);
	pMesh->materials.swap(meshData.materials);

	pMesh->name = filename;
	skeletalMeshes.emplace(filename, pMesh);
	
	  // AddVertexData�ŃR�s�[�����f�[�^�̈ʒu�𔽉f
	const intptr_t baseIndexOffset =
	reinterpret_cast<intptr_t>(drawParamsList.back().indices);
	const GLint baseVertex = drawParamsList.back().baseVertex;
	for (auto& e : pMesh->drawParamsList) {
		e.baseVertex = baseVertex;
		e.indices = reinterpret_cast<void*>(
			baseIndexOffset + reinterpret_cast<intptr_t>(e.indices));

	}
	LOG("%s��ǂݍ��݂܂���(���_��=%d, �C���f�b�N�X��=%d)",
		filename, meshData.vertices.size(), meshData.indices.size());

	// �쐬�������b�V����Ԃ�
	return pMesh;
}

//OBJ�̓ǂݍ��݂�CreateMesuDataFromOBJ�����o�֐��ɒS��������
/**
* OBJ�t�@�C�������b�V���f�[�^�ɕϊ�����
*
* @param filename OBJ�t�@�C����
*
* @return filename����쐬�������b�V���f�[�^
*/
MeshBuffer::MeshData MeshBuffer::CreateMeshDataFromOBJ(const char* filename)
 {

	// OBJ�t�@�C�����J��
	std::ifstream file(filename);
	if (!file) {
		LOG_ERROR("%s���J���܂���", filename);
		return {}; // ��̃I�u�W�F�N�g��Ԃ�

	}

	// �t�H���_�����擾����
	std::string foldername(filename);
	{
		const size_t p = foldername.find_last_of("����/");
		if (p != std::string::npos) {
			foldername.resize(p + 1);

		}
	}

	// OBJ�t�@�C������͂��āA���_�f�[�^�ƃC���f�b�N�X�f�[�^��ǂݍ���
	std::vector<vec3> positions;
	std::vector<vec2> texcoords;
	std::vector<vec3> normals;
	struct IndexSet { int v, vt, vn; };
	std::vector<IndexSet> faceIndexSet;

	positions.reserve(20'000);
	texcoords.reserve(20'000);
	normals.reserve(20'000);
	faceIndexSet.reserve(20'000 * 3);


	// �}�e���A��
	std::vector<MaterialPtr> materials;
	materials.reserve(100);


	// �}�e���A���̎g�p�͈�
	struct UseMaterial {
		std::string name;   // �}�e���A����
		size_t startOffset; // ���蓖�Ĕ͈͂̐擪�ʒu

	};
	std::vector<UseMaterial> usemtls;
	usemtls.reserve(100);

	// ���f�[�^��ǉ�(�}�e���A���w�肪�Ȃ��t�@�C���΍�)
	usemtls.push_back({ std::string(), 0 });

	while (!file.eof())
	{
		std::string line;
		std::getline(file, line);
		const char* p = line.c_str();

		// ���_���W�̓ǂݎ������݂�
		//���s�����C�ɂ��邽�߁A�󔒂͏����Ȃ��悤��
		vec3 v;
		if (sscanf(p, " v %f %f %f", &v.x, &v.y, &v.z) == 3) {
			positions.push_back(v);
			continue;

		}

		// �e�N�X�`�����W�̓ǂݎ������݂�
		vec2 vt;
		if (sscanf(p, " vt %f %f", &vt.x, &vt.y) == 2) {
			texcoords.push_back(vt);
			continue;

		}


		// �@���̓ǂݎ������݂�
		vec3 vn;
		if (sscanf(p, " vn %f %f %f", &vn.x, &vn.y, &vn.z) == 3) {
			normals.push_back(vn);
			continue;

		}
		// �C���f�b�N�X�f�[�^�̓ǂݎ������݂�
		IndexSet f0, f1, f2;
		int readByte;
		//���_���W�{�e�N�X�`�����W�{�@��
		if (sscanf(p, " f %u/%u/%u %u/%u/%u%n",
			&f0.v, &f0.vt, &f0.vn, &f1.v, &f1.vt, &f1.vn, &readByte) == 6)
		{
			p += readByte; // �ǂݎ��ʒu���X�V
			for (;;)
			{
				//���_���W�{�e�N�X�`�����W
				if (sscanf(p, " %u/%u/%u%n", &f2.v, &f2.vt, &f2.vn, &readByte) != 3) {
					break;
				}

				p += readByte; // �ǂݎ��ʒu���X�V
				faceIndexSet.push_back(f0);
				faceIndexSet.push_back(f1);
				faceIndexSet.push_back(f2);
				f1 = f2; // ���̎O�p�`�̂��߂Ƀf�[�^���ړ�

			}
			continue;

		}
		//%n�������܂łɓǂݎ�����������������Ɋi�[����
		if (sscanf(p, " f %u/%u %u/%u%n",
			&f0.v, &f0.vt, &f1.v, &f1.vt, &readByte) == 4)
		{
			f0.vn = f1.vn = 0; // �@���Ȃ�
			p += readByte; // �ǂݎ��ʒu���X�V
			for (;;) {
				if (sscanf(p, " %u/%u%n", &f2.v, &f2.vt, &readByte) != 2) {
					break;

				}
				f2.vn = 0; // �@���Ȃ�
				p += readByte; // �ǂݎ��ʒu���X�V
				faceIndexSet.push_back(f0);
				faceIndexSet.push_back(f1);
				faceIndexSet.push_back(f2);
				f1 = f2; // ���̎O�p�`�̂��߂Ƀf�[�^���ړ�

			}
			continue;

		}

		// MTL�t�@�C���̓ǂݎ������݂�
		char mtlFilename[1000];
		if (sscanf(line.data(), " mtllib %999s", mtlFilename) == 1) {
			const auto tmp = LoadMTL(foldername, mtlFilename);
			materials.insert(materials.end(), tmp.begin(), tmp.end());
			continue;

		}


		// �g�p�}�e���A�����̓ǂݎ������݂�
		char mtlName[1000];
		if (sscanf(line.data(), " usemtl %999s", mtlName) == 1) {
			usemtls.push_back({ mtlName, faceIndexSet.size() });
			continue;

		}
	} // while eof


	// �����ɔԕ���ǉ�
	usemtls.push_back({ std::string(), faceIndexSet.size() });
	
	// OBJ�t�@�C����f�\����OpenGL�̒��_�C���f�b�N�X�z��̑Ή��\
	std::unordered_map<uint64_t, uint16_t> indexMap;
	indexMap.reserve(10'000);

	MeshData meshData;
	// �ǂݍ��񂾃f�[�^���AOpenGL�Ŏg����f�[�^�ɕϊ�
	//======================vertices==========================//
	std::vector<Vertex>& vertices = meshData.vertices;
	vertices.reserve(faceIndexSet.size());
	//====================indices===========================//
	std::vector<uint16_t>& indices = meshData.indices;
	indices.reserve(faceIndexSet.size());

	for (const auto& e : faceIndexSet) {
		// f�\���̒l��64�r�b�g�́u�L�[�v�ɕϊ�
		const uint64_t key = static_cast<uint64_t>(e.v) +
		(static_cast<uint64_t>(e.vt) << 20) + (static_cast<uint64_t>(e.vn) << 40);
		
		// �Ή��\����L�[�Ɉ�v����f�[�^������
		const auto itr = indexMap.find(key);

		if (itr != indexMap.end()) {
			      // �Ή��\�ɂ���̂Ŋ����̒��_�C���f�b�N�X���g��
				indices.push_back(itr->second);
			
		}
		else
		{
			// �Ή��\�ɂȂ��̂ŐV�������_�f�[�^���쐬���A���_�z��ɒǉ�

			Vertex v;
			v.position = positions[e.v - 1];
			v.texcoord = texcoords[e.vt - 1];
			// �@�����ݒ肳��Ă��Ȃ��ꍇ��0��ݒ�(���ƂŌv�Z)
			if (e.vn == 0) {
				v.normal = { 0, 0, 0 };

			}
			else {
				v.normal = normals[e.vn - 1];

			}
			vertices.push_back(v);
			
			// �V�������_�f�[�^�̃C���f�b�N�X���A���_�C���f�b�N�X�z��ɒǉ�
			const uint16_t index = static_cast<uint16_t>(vertices.size() - 1);
			indices.push_back(index);
			
			// �L�[�ƒ��_�C���f�b�N�X�̃y�A��Ή��\�ɒǉ�
			indexMap.emplace(key, index);
			
		}

	}

	// �ݒ肳��Ă��Ȃ��@����₤
	FillMissingNormals(vertices.data(), vertices.size(),
		indices.data(), indices.size());

	// �f�[�^�̈ʒu���擾
	const void* indexOffset = 0;
	const GLint baseVertex = 0;

	// �}�e���A���ɑΉ������`��p�����[�^���쐬
	 // ���f�[�^�Ɣԕ��ȊO�̃}�e���A��������ꍇ�A���f�[�^���΂�
	size_t i = 0;
	if (usemtls.size() > 2) {
		i = 1; // ���f�[�^�Ɣԕ��ȊO�̃}�e���A��������ꍇ�A���f�[�^���΂�

	}
	for (; i < usemtls.size() - 1; ++i) {
		const UseMaterial& cur = usemtls[i]; // �g�p���̃}�e���A��
		const UseMaterial& next = usemtls[i + 1]; // ���̃}�e���A��
		if (next.startOffset == cur.startOffset) {
			continue; // �C���f�b�N�X�f�[�^���Ȃ��ꍇ�͔�΂�

		}
		// �`��p�����[�^���쐬
		DrawParams params;
		params.mode = GL_TRIANGLES;
		params.count = static_cast<GLsizei>(next.startOffset - cur.startOffset);
		params.indices = indexOffset;
		params.baseVertex = baseVertex;
		params.materialNo = 0; // �f�t�H���g�l��ݒ�
		for (int i = 0; i < materials.size(); ++i) {
			if (materials[i]->name == cur.name) {
				params.materialNo = i; // ���O�̈�v����}�e���A����ݒ�
				break;
			}
		}
		meshData.drawParamsList.push_back(params);

		// �C���f�b�N�X�I�t�Z�b�g��ύX
		indexOffset = reinterpret_cast<void*>(
			reinterpret_cast<size_t>(indexOffset) + sizeof(uint16_t) * params.count);
	}

	// �}�e���A���z�񂪋�̏ꍇ�A�f�t�H���g�}�e���A����ǉ�
	if (materials.empty()) {
		meshData.materials.push_back(std::make_shared<Material>());

	}
	else {
		meshData.materials.assign(materials.begin(), materials.end());

	}
	
	return meshData;

}

//�����̃v���O������ύX�����ɍς܂��邽��
//stride(�X�g���C�h)�����̓f�t�H���g�l���w�肵�Ă����
/**
* ���_�f�[�^�̒ǉ�
*/
void MeshBuffer::AddVertexData(const void* vertices, size_t vertexBytes,
	const uint16_t * indices, size_t indexBytes, size_t stride, GLenum mode)
{
	// �󂫗e�ʂ�����Ă��邱�Ƃ��m�F
	// �K�v�ȃo�C�g�����󂫃o�C�g�����傫���ꍇ�͒ǉ��ł��Ȃ�
	const size_t totalBytes = vertexBytes + indexBytes;
	const size_t freeBytes = buffer->GetSize() - usedBytes;
	if (totalBytes > freeBytes) {
		LOG_ERROR("�e�ʂ�����܂���(�v���T�C�Y=%d, �g�p��/�ő�e��=%d/%d)",
			vertexBytes + indexBytes, usedBytes, buffer->GetSize());
		return;

	}

	// ���_�f�[�^��GPU�������ɃR�s�[
	GLuint tmp[2];
	glCreateBuffers(2, tmp);
	glNamedBufferStorage(tmp[0], vertexBytes, vertices, 0);
	glNamedBufferStorage(tmp[1], indexBytes, indices, 0);
	glCopyNamedBufferSubData(tmp[0], *buffer, 0, usedBytes, vertexBytes);
	glCopyNamedBufferSubData(tmp[1], *buffer, 0, usedBytes + vertexBytes, indexBytes);
	glDeleteBuffers(2, tmp);

	// �ǉ������}�`�̕`��p�����[�^���쐬
	DrawParams newParams;
	newParams.mode = mode;
	newParams.count = static_cast<GLsizei>(indexBytes / sizeof(uint16_t));
	// �C���f�b�N�X�f�[�^�̈ʒu(�o�C�g���Ŏw��)�͒��_�f�[�^�̒���
	newParams.indices = reinterpret_cast<void*>(usedBytes + vertexBytes);
	// ���_�f�[�^�̈ʒu�͒��_�f�[�^���Ŏw�肷��
	newParams.baseVertex = static_cast<GLint>(usedBytes / stride);
	drawParamsList.push_back(newParams);

	// ���̃f�[�^�i�[�J�n�ʒu���v�Z
	//uint16_t��Vertex�̍ŏ����{���ɂ���ĐV�����f�[�^�i�[�ʒu�����߂Ă���̂ŁA
	//SkeletalVertex�������āA3�̍ŏ����{�������߂Ă����܂��I
	constexpr size_t a = std::lcm(sizeof(SkeletalVertex),
		std::lcm(sizeof(uint16_t), sizeof(Vertex)));
	//uint16_t��2�o�C�g�AVertex��32�o�C�g�ASkeletalVertex��48�o�C�g
	//����čŏ����{����96�o�C�g��

	usedBytes += ((totalBytes + a - 1) / a) * a;
}



/**
* �����̕`��p�����[�^�ƃe�N�X�`������V�����X�^�e�B�b�N���b�V�����쐬����
*/
StaticMeshPtr MeshBuffer::CreateStaticMesh(const char* name,
	const DrawParams& params, const TexturePtr& texBaseColor)
{
	auto p = std::make_shared<StaticMesh>();
	p->name = name;
	p->drawParamsList.push_back(params);
	p->drawParamsList[0].materialNo = 0;
	p->materials.push_back(std::make_shared<Material>());
	if (texBaseColor) {
		p->materials[0]->texBaseColor = texBaseColor;

	}
	meshes.emplace(name, p);
	return p;
}

/**
* �S�Ă̒��_�f�[�^���폜
*/
void MeshBuffer::Clear()
 {
	usedBytes = 0;
	meshes.clear();
	drawParamsList.clear();
}

// �����Ă���@����₤
void FillMissingNormals(
	Vertex* vertices, size_t vertexCount,
	const uint16_t* indices, size_t indexCount)
{
	// �@�����ݒ肳��Ă��Ȃ����_��������
	std::vector<bool> missingNormals(vertexCount, false);
	for (int i = 0; i < vertexCount; ++i) {
		    // �@���̒�����0�̏ꍇ���u�ݒ肳��Ă��Ȃ��v�Ƃ݂Ȃ�
			const vec3 & n = vertices[i].normal;
		if (n.x == 0 && n.y == 0 && n.z == 0) {
			missingNormals[i] = true;
			
		}
		
	}
		// �@�����v�Z
	for (int i = 0; i < indexCount; i += 3) {
		// �ʂ��\������2��a, b�����߂�
		 //�O�ςɂ���Ė@�������߂邽�ߗv�f���o��
		const int i0 = indices[i + 0];
		const int i1 = indices[i + 1];
		const int i2 = indices[i + 2];
		//�v�f���͂߂�
		const vec3& v0 = vertices[i0].position;
		const vec3& v1 = vertices[i1].position;
		const vec3& v2 = vertices[i2].position;
		//�O�ς��o���āA�@��������
		const vec3 a = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
		const vec3 b = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

		// �O�ςɂ����a��b�ɐ����ȃx�N�g��(�@��)�����߂�
		const float cx = a.y * b.z - a.z * b.y;
		const float cy = a.z * b.x - a.x * b.z;
		const float cz = a.x * b.y - a.y * b.x;

		// �@���𐳋K�����ĒP�ʃx�N�g���ɂ���
		const float l = sqrt(cx * cx + cy * cy + cz * cz);
		const vec3 normal = { cx / l, cy / l, cz / l };

		
	 // �@�����ݒ肳��Ă��Ȃ����_�ɂ����@�������Z
		if (missingNormals[i0]) {
		vertices[i0].normal += normal;
		
		}
		 if (missingNormals[i1]) {
			vertices[i1].normal += normal;
			
		}
		 if (missingNormals[i2]) {
			vertices[i2].normal += normal;
			
		}
	}
		// �@���𐳋K��
	 for (int i = 0; i < vertexCount; ++i) {
		if (missingNormals[i]) {
			vec3 & n = vertices[i].normal;
			const float l = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
			n = { n.x / l, n.y / l, n.z / l };
			
		}
		
	} // for i
}

// �X�^�e�B�b�N���b�V�����擾
StaticMeshPtr MeshBuffer::GetStaticMesh(const char* name) const
{
	auto itr = meshes.find(name);
	if (itr != meshes.end()) {
		return itr->second;
		
	}
	 return nullptr;
}

//===================================================================================//
// �X�P���^�����b�V�����擾
SkeletalMeshPtr MeshBuffer::GetSkeletalMesh(const char* name) const
{
	auto itr = skeletalMeshes.find(name);
	if (itr != skeletalMeshes.end()) {
		return itr->second;
		
	}
	 return nullptr;
}
//====================================================================================//