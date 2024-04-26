#define _CRT_SECURE_NO_WARNINGS
#include "Mesh.h"
#include "Debug.h"
#include <numeric>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdio.h>



/**
* メッシュを描画する
*/
void Draw(const StaticMesh& mesh, GLuint program, const MaterialList& materials)
{
	// カラーパラメータを取得
	vec4 objectColor;
	bool hasUniformColor = false; // カラー系ユニフォーム変数の有無

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

//描画パラメータを直接描画
/**
* 描画パラメータ配列を描画する
*/
void Draw(const std::vector<DrawParams>&drawParamsList, GLuint program,
	const MaterialList & materials, const vec4 * objectColor)
{
	for (const auto& e : drawParamsList) {
		// マテリアルを設定
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
				//使わないテクスチャイメージスロットにテクスチャを
				// 設定していると、シェーダの効率が低下する場合が
				//なので、テクスチャイメージスロットの１番を未設定に
			if (material.texEmission) {
				const GLuint tex = *material.texEmission;
				glBindTextures(1, 1, &tex);
				
			}
			else {
				
				glBindTextures(1, 1, nullptr); // テクスチャ1を未設定にする
				
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
* MTLファイルを読み込む
*
* @param foldername OBJファイルのあるフォルダ名
* @param filename   MTLファイル名
*
* @return MTLファイルに含まれるマテリアルの配列
*/
std::vector<MaterialPtr> MeshBuffer::LoadMTL(
	const std::string& foldername, const char* filename)
{
	// MTLファイルを開く
	const std::string fullpath = foldername + filename;
	std::ifstream file(fullpath);
	if (!file) {
		LOG_ERROR("%sを開けません", fullpath.c_str());
		return {};

	}

	// MTLファイルを解析する
	std::vector<MaterialPtr> materials;
	MaterialPtr pMaterial;
	while (!file.eof()) {
		std::string line;
		std::getline(file, line);

		// マテリアル定義の読み取りを試みる
		char name[1000] = { 0 };
		if (sscanf(line.data(), " newmtl %999s", name) == 1)
		{
			pMaterial = std::make_shared<Material>();
			pMaterial->name = name;
			materials.push_back(pMaterial);
			continue;

		}

		// マテリアルが定義されていない場合は行を無視する
		if (!pMaterial) {
			continue;

		}

		// 基本色の読み取りを試みる
		if (sscanf(line.data(), " Kd %f %f %f",
			&pMaterial->baseColor.x, &pMaterial->baseColor.y,
			&pMaterial->baseColor.z) == 3) {
			continue;

		}

		// 不透明度の読み取りを試みる
		if (sscanf(line.data(), " d %f", &pMaterial->baseColor.w) == 1) {
			continue;

		}

		// 基本色テクスチャ名の読み取りを試みる
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
		
		    // 発光色の読み取りを試みる
		if (sscanf(line.data(), " Ke %f %f %f",
			&pMaterial->emission.x, &pMaterial->emission.y,
			&pMaterial->emission.z) == 3) {
		continue;
		
		}
		
		    // 発光色テクスチャ名の読み取りを試みる
		if (sscanf(line.data(), " map_Ke %999s", &textureName) == 1) {
			const std::string filename = foldername + textureName;
			if (std::filesystem::exists(filename)) {
				pMaterial->texEmission =
					textureCallback(filename.c_str());

			}
			else {
				LOG_WARNING("%sを開けません", filename.c_str());

			}
			continue;

		} // if map_Ke

	}

	// 読み込んだマテリアルの配列を返す
	return materials;
}

/**
* コンストラクタ
*/
MeshBuffer::MeshBuffer(size_t bufferSize)
{
	// バッファオブジェクトを作成
	buffer = BufferObject::Create(bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

	// VAOを作成
	vao = VertexArrayObject::Create();

	// VBOとIBOをVAOにバインド
	glBindVertexArray(*vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);

	// 頂点アトリビュートを設定
	vao->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
	vao->SetAttribute(1, 2, sizeof(Vertex), offsetof(Vertex, texcoord));
	vao->SetAttribute(2, 3, sizeof(Vertex), offsetof(Vertex, normal));
	
	  // スケルタルメッシュ用のVAOを作成
	vaoSkeletal = VertexArrayObject::Create();
	glBindVertexArray(*vaoSkeletal);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	//position,texcoord,normalはスタティックメッシュと同じ構成
	vaoSkeletal->SetAttribute(0, 3, sizeof(SkeletalVertex),
		offsetof(SkeletalVertex, position));
	vaoSkeletal->SetAttribute(1, 2, sizeof(SkeletalVertex),
		offsetof(SkeletalVertex, texcoord));
	vaoSkeletal->SetAttribute(2, 3, sizeof(SkeletalVertex),
		offsetof(SkeletalVertex, normal));
	glEnableVertexAttribArray(3);

	//glVertexAttribPointerと違い、正規化の必要なし
	//整数値で送ってほしいのに、正規化すると浮動小数点数になってしまうため
	
	glVertexAttribIPointer(3, 4, GL_UNSIGNED_SHORT, sizeof(SkeletalVertex),
		reinterpret_cast<const void*>(offsetof(SkeletalVertex, joints)));
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_UNSIGNED_SHORT, sizeof(SkeletalVertex),
		reinterpret_cast<const void*>(offsetof(SkeletalVertex, joints)));

	// 誤った操作が行われないようにバインドを解除
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	// スタティックメッシュの容量を予約
	meshes.reserve(100);
	// 描画パラメータの容量を予約
	drawParamsList.reserve(100);
}


/**
* OBJファイルを読み込む
*
* @param filename OBJファイル名
*
* @return filenameから作成したメッシュ
*/
StaticMeshPtr MeshBuffer::LoadOBJ(const char* filename)
{
	// 以前に読み込んだファイルなら、作成済みのメッシュを返す
	{
		//meshes配列にはこれまでに読み込んだファイルが記録されているので
		//Find関数でfilenameと同じデータを検索して
		//読み込んだことがあれば、そのまま返す 
		//戻り値を調べれば、以前に読み込んだかどうかが分かる
		auto itr = meshes.find(filename);
		//データの位置が配列の終端と異なっていたら「データがある」ということ
		if (itr != meshes.end()) {
			//itrのfirstは検索キー,secondはデータ本体となっているため
			//secondを渡す
			return itr->second;

		}
	}
	
	// OBJファイルからメッシュデータを作成
	MeshData meshData = CreateMeshDataFromOBJ(filename);
	if (meshData.vertices.empty()) {
		return nullptr; // 読み込み失敗
		
	}
	  // 変換したデータをプリミティブバッファに追加
	AddVertexData(
		meshData.vertices.data(), meshData.vertices.size() * sizeof(Vertex),
		meshData.indices.data(), meshData.indices.size() * sizeof(uint16_t));
	
	  // メッシュを作成
	auto pMesh = std::make_shared<StaticMesh>();

	pMesh->drawParamsList.swap(meshData.drawParamsList);
	pMesh->materials.swap(meshData.materials);

	pMesh->name = filename;
	meshes.emplace(filename, pMesh);

	
	 // AddVertexDataでコピーしたデータの位置を、描画パラメータに反映
	const intptr_t baseIndexOffset =
	reinterpret_cast<intptr_t>(drawParamsList.back().indices);
	const GLint baseVertex = drawParamsList.back().baseVertex;
	for (auto& e : pMesh->drawParamsList) {
		e.baseVertex = baseVertex;
		e.indices = reinterpret_cast<const void*>(
			baseIndexOffset + reinterpret_cast<intptr_t>(e.indices));
		
	}
	LOG("%sを読み込みました(頂点数=%d, インデックス数=%d",
		filename, meshData.vertices.size(), meshData.indices.size());

	// 作成したメッシュを返す
	return pMesh;

}

/**
* OBJファイルを読み込み関節を設定する
*/
SkeletalMeshPtr MeshBuffer::LoadOBJ(
	const char* filename, const Skeleton& skeleton)
{
	// 以前に読み込んだファイルなら、作成済みのメッシュを返す
	{
		auto itr = skeletalMeshes.find(filename);
		if (itr != skeletalMeshes.end()) {
			return itr->second;

		}
	}

	// OBJファイルからメッシュデータを作成
	MeshData meshData = CreateMeshDataFromOBJ(filename);
	if (meshData.vertices.empty()) {
		return nullptr; // 読み込み失敗

	}
	
	  // Vertexの共通部分をSkeletaVertexにコピー
	std::vector<SkeletalVertex> skeletalVertices(meshData.vertices.size());
	for (int i = 0; i < meshData.vertices.size(); ++i) {
		skeletalVertices[i].position = meshData.vertices[i].position;
		skeletalVertices[i].texcoord = meshData.vertices[i].texcoord;
		skeletalVertices[i].normal = meshData.vertices[i].normal;

	}
		
		  // 関節行列の番号と影響度を設定(仮)
	for (auto& vertex : skeletalVertices) {
		std::fill_n(vertex.joints, 4, 0);
		std::fill_n(vertex.weights, 4, 0);
		vertex.joints[0] =
			static_cast<uint16_t>(std::clamp(vertex.position.y, 0.0f, 3.0f));
		vertex.weights[0] = 65535;

	}

	
	// 変換したデータをプリミティブバッファに追加
	//スタティックメッシュ用のVAOとスケルタルメッシュ用のVAOではストライドが異なるため、
	//スタティック用のVAOと同じストライドを指定する

	AddVertexData(
		skeletalVertices.data(), skeletalVertices.size() * sizeof(SkeletalVertex),
		meshData.indices.data(), meshData.indices.size() * sizeof(uint16_t),
		sizeof(SkeletalVertex));
	
	  // スケルタルメッシュを作成
	auto pMesh = std::make_shared<SkeletalMesh>();
	//swap関数は、データを指すポインタ型を交換するだけの	
	pMesh->drawParamsList.swap(meshData.drawParamsList);
	pMesh->materials.swap(meshData.materials);

	pMesh->name = filename;
	skeletalMeshes.emplace(filename, pMesh);
	
	  // AddVertexDataでコピーしたデータの位置を反映
	const intptr_t baseIndexOffset =
	reinterpret_cast<intptr_t>(drawParamsList.back().indices);
	const GLint baseVertex = drawParamsList.back().baseVertex;
	for (auto& e : pMesh->drawParamsList) {
		e.baseVertex = baseVertex;
		e.indices = reinterpret_cast<void*>(
			baseIndexOffset + reinterpret_cast<intptr_t>(e.indices));

	}
	LOG("%sを読み込みました(頂点数=%d, インデックス数=%d)",
		filename, meshData.vertices.size(), meshData.indices.size());

	// 作成したメッシュを返す
	return pMesh;
}

//OBJの読み込みをCreateMesuDataFromOBJメンバ関数に担当させる
/**
* OBJファイルをメッシュデータに変換する
*
* @param filename OBJファイル名
*
* @return filenameから作成したメッシュデータ
*/
MeshBuffer::MeshData MeshBuffer::CreateMeshDataFromOBJ(const char* filename)
 {

	// OBJファイルを開く
	std::ifstream file(filename);
	if (!file) {
		LOG_ERROR("%sを開けません", filename);
		return {}; // 空のオブジェクトを返す

	}

	// フォルダ名を取得する
	std::string foldername(filename);
	{
		const size_t p = foldername.find_last_of("￥￥/");
		if (p != std::string::npos) {
			foldername.resize(p + 1);

		}
	}

	// OBJファイルを解析して、頂点データとインデックスデータを読み込む
	std::vector<vec3> positions;
	std::vector<vec2> texcoords;
	std::vector<vec3> normals;
	struct IndexSet { int v, vt, vn; };
	std::vector<IndexSet> faceIndexSet;

	positions.reserve(20'000);
	texcoords.reserve(20'000);
	normals.reserve(20'000);
	faceIndexSet.reserve(20'000 * 3);


	// マテリアル
	std::vector<MaterialPtr> materials;
	materials.reserve(100);


	// マテリアルの使用範囲
	struct UseMaterial {
		std::string name;   // マテリアル名
		size_t startOffset; // 割り当て範囲の先頭位置

	};
	std::vector<UseMaterial> usemtls;
	usemtls.reserve(100);

	// 仮データを追加(マテリアル指定がないファイル対策)
	usemtls.push_back({ std::string(), 0 });

	while (!file.eof())
	{
		std::string line;
		std::getline(file, line);
		const char* p = line.c_str();

		// 頂点座標の読み取りを試みる
		//改行等を気にするため、空白は消さないように
		vec3 v;
		if (sscanf(p, " v %f %f %f", &v.x, &v.y, &v.z) == 3) {
			positions.push_back(v);
			continue;

		}

		// テクスチャ座標の読み取りを試みる
		vec2 vt;
		if (sscanf(p, " vt %f %f", &vt.x, &vt.y) == 2) {
			texcoords.push_back(vt);
			continue;

		}


		// 法線の読み取りを試みる
		vec3 vn;
		if (sscanf(p, " vn %f %f %f", &vn.x, &vn.y, &vn.z) == 3) {
			normals.push_back(vn);
			continue;

		}
		// インデックスデータの読み取りを試みる
		IndexSet f0, f1, f2;
		int readByte;
		//頂点座標＋テクスチャ座標＋法線
		if (sscanf(p, " f %u/%u/%u %u/%u/%u%n",
			&f0.v, &f0.vt, &f0.vn, &f1.v, &f1.vt, &f1.vn, &readByte) == 6)
		{
			p += readByte; // 読み取り位置を更新
			for (;;)
			{
				//頂点座標＋テクスチャ座標
				if (sscanf(p, " %u/%u/%u%n", &f2.v, &f2.vt, &f2.vn, &readByte) != 3) {
					break;
				}

				p += readByte; // 読み取り位置を更新
				faceIndexSet.push_back(f0);
				faceIndexSet.push_back(f1);
				faceIndexSet.push_back(f2);
				f1 = f2; // 次の三角形のためにデータを移動

			}
			continue;

		}
		//%nが現れるまでに読み取った文字数を引数に格納する
		if (sscanf(p, " f %u/%u %u/%u%n",
			&f0.v, &f0.vt, &f1.v, &f1.vt, &readByte) == 4)
		{
			f0.vn = f1.vn = 0; // 法線なし
			p += readByte; // 読み取り位置を更新
			for (;;) {
				if (sscanf(p, " %u/%u%n", &f2.v, &f2.vt, &readByte) != 2) {
					break;

				}
				f2.vn = 0; // 法線なし
				p += readByte; // 読み取り位置を更新
				faceIndexSet.push_back(f0);
				faceIndexSet.push_back(f1);
				faceIndexSet.push_back(f2);
				f1 = f2; // 次の三角形のためにデータを移動

			}
			continue;

		}

		// MTLファイルの読み取りを試みる
		char mtlFilename[1000];
		if (sscanf(line.data(), " mtllib %999s", mtlFilename) == 1) {
			const auto tmp = LoadMTL(foldername, mtlFilename);
			materials.insert(materials.end(), tmp.begin(), tmp.end());
			continue;

		}


		// 使用マテリアル名の読み取りを試みる
		char mtlName[1000];
		if (sscanf(line.data(), " usemtl %999s", mtlName) == 1) {
			usemtls.push_back({ mtlName, faceIndexSet.size() });
			continue;

		}
	} // while eof


	// 末尾に番兵を追加
	usemtls.push_back({ std::string(), faceIndexSet.size() });
	
	// OBJファイルのf構文とOpenGLの頂点インデックス配列の対応表
	std::unordered_map<uint64_t, uint16_t> indexMap;
	indexMap.reserve(10'000);

	MeshData meshData;
	// 読み込んだデータを、OpenGLで使えるデータに変換
	//======================vertices==========================//
	std::vector<Vertex>& vertices = meshData.vertices;
	vertices.reserve(faceIndexSet.size());
	//====================indices===========================//
	std::vector<uint16_t>& indices = meshData.indices;
	indices.reserve(faceIndexSet.size());

	for (const auto& e : faceIndexSet) {
		// f構文の値を64ビットの「キー」に変換
		const uint64_t key = static_cast<uint64_t>(e.v) +
		(static_cast<uint64_t>(e.vt) << 20) + (static_cast<uint64_t>(e.vn) << 40);
		
		// 対応表からキーに一致するデータを検索
		const auto itr = indexMap.find(key);

		if (itr != indexMap.end()) {
			      // 対応表にあるので既存の頂点インデックスを使う
				indices.push_back(itr->second);
			
		}
		else
		{
			// 対応表にないので新しい頂点データを作成し、頂点配列に追加

			Vertex v;
			v.position = positions[e.v - 1];
			v.texcoord = texcoords[e.vt - 1];
			// 法線が設定されていない場合は0を設定(あとで計算)
			if (e.vn == 0) {
				v.normal = { 0, 0, 0 };

			}
			else {
				v.normal = normals[e.vn - 1];

			}
			vertices.push_back(v);
			
			// 新しい頂点データのインデックスを、頂点インデックス配列に追加
			const uint16_t index = static_cast<uint16_t>(vertices.size() - 1);
			indices.push_back(index);
			
			// キーと頂点インデックスのペアを対応表に追加
			indexMap.emplace(key, index);
			
		}

	}

	// 設定されていない法線を補う
	FillMissingNormals(vertices.data(), vertices.size(),
		indices.data(), indices.size());

	// データの位置を取得
	const void* indexOffset = 0;
	const GLint baseVertex = 0;

	// マテリアルに対応した描画パラメータを作成
	 // 仮データと番兵以外のマテリアルがある場合、仮データを飛ばす
	size_t i = 0;
	if (usemtls.size() > 2) {
		i = 1; // 仮データと番兵以外のマテリアルがある場合、仮データを飛ばす

	}
	for (; i < usemtls.size() - 1; ++i) {
		const UseMaterial& cur = usemtls[i]; // 使用中のマテリアル
		const UseMaterial& next = usemtls[i + 1]; // 次のマテリアル
		if (next.startOffset == cur.startOffset) {
			continue; // インデックスデータがない場合は飛ばす

		}
		// 描画パラメータを作成
		DrawParams params;
		params.mode = GL_TRIANGLES;
		params.count = static_cast<GLsizei>(next.startOffset - cur.startOffset);
		params.indices = indexOffset;
		params.baseVertex = baseVertex;
		params.materialNo = 0; // デフォルト値を設定
		for (int i = 0; i < materials.size(); ++i) {
			if (materials[i]->name == cur.name) {
				params.materialNo = i; // 名前の一致するマテリアルを設定
				break;
			}
		}
		meshData.drawParamsList.push_back(params);

		// インデックスオフセットを変更
		indexOffset = reinterpret_cast<void*>(
			reinterpret_cast<size_t>(indexOffset) + sizeof(uint16_t) * params.count);
	}

	// マテリアル配列が空の場合、デフォルトマテリアルを追加
	if (materials.empty()) {
		meshData.materials.push_back(std::make_shared<Material>());

	}
	else {
		meshData.materials.assign(materials.begin(), materials.end());

	}
	
	return meshData;

}

//既存のプログラムを変更せずに済ませるため
//stride(ストライド)引数はデフォルト値を指定しているの
/**
* 頂点データの追加
*/
void MeshBuffer::AddVertexData(const void* vertices, size_t vertexBytes,
	const uint16_t * indices, size_t indexBytes, size_t stride, GLenum mode)
{
	// 空き容量が足りていることを確認
	// 必要なバイト数が空きバイト数より大きい場合は追加できない
	const size_t totalBytes = vertexBytes + indexBytes;
	const size_t freeBytes = buffer->GetSize() - usedBytes;
	if (totalBytes > freeBytes) {
		LOG_ERROR("容量が足りません(要求サイズ=%d, 使用量/最大容量=%d/%d)",
			vertexBytes + indexBytes, usedBytes, buffer->GetSize());
		return;

	}

	// 頂点データをGPUメモリにコピー
	GLuint tmp[2];
	glCreateBuffers(2, tmp);
	glNamedBufferStorage(tmp[0], vertexBytes, vertices, 0);
	glNamedBufferStorage(tmp[1], indexBytes, indices, 0);
	glCopyNamedBufferSubData(tmp[0], *buffer, 0, usedBytes, vertexBytes);
	glCopyNamedBufferSubData(tmp[1], *buffer, 0, usedBytes + vertexBytes, indexBytes);
	glDeleteBuffers(2, tmp);

	// 追加した図形の描画パラメータを作成
	DrawParams newParams;
	newParams.mode = mode;
	newParams.count = static_cast<GLsizei>(indexBytes / sizeof(uint16_t));
	// インデックスデータの位置(バイト数で指定)は頂点データの直後
	newParams.indices = reinterpret_cast<void*>(usedBytes + vertexBytes);
	// 頂点データの位置は頂点データ数で指定する
	newParams.baseVertex = static_cast<GLint>(usedBytes / stride);
	drawParamsList.push_back(newParams);

	// 次のデータ格納開始位置を計算
	//uint16_tとVertexの最小公倍数によって新しいデータ格納位置を決めているので、
	//SkeletalVertexを加えて、3つの最小公倍数を求めていきます！
	constexpr size_t a = std::lcm(sizeof(SkeletalVertex),
		std::lcm(sizeof(uint16_t), sizeof(Vertex)));
	//uint16_tが2バイト、Vertexが32バイト、SkeletalVertexが48バイト
	//よって最小公倍数は96バイトに

	usedBytes += ((totalBytes + a - 1) / a) * a;
}



/**
* 既存の描画パラメータとテクスチャから新しいスタティックメッシュを作成する
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
* 全ての頂点データを削除
*/
void MeshBuffer::Clear()
 {
	usedBytes = 0;
	meshes.clear();
	drawParamsList.clear();
}

// 欠けている法線を補う
void FillMissingNormals(
	Vertex* vertices, size_t vertexCount,
	const uint16_t* indices, size_t indexCount)
{
	// 法線が設定されていない頂点を見つける
	std::vector<bool> missingNormals(vertexCount, false);
	for (int i = 0; i < vertexCount; ++i) {
		    // 法線の長さが0の場合を「設定されていない」とみなす
			const vec3 & n = vertices[i].normal;
		if (n.x == 0 && n.y == 0 && n.z == 0) {
			missingNormals[i] = true;
			
		}
		
	}
		// 法線を計算
	for (int i = 0; i < indexCount; i += 3) {
		// 面を構成する2辺a, bを求める
		 //外積によって法線を求めるため要素を出す
		const int i0 = indices[i + 0];
		const int i1 = indices[i + 1];
		const int i2 = indices[i + 2];
		//要素をはめる
		const vec3& v0 = vertices[i0].position;
		const vec3& v1 = vertices[i1].position;
		const vec3& v2 = vertices[i2].position;
		//外積を出して、法線をだす
		const vec3 a = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
		const vec3 b = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

		// 外積によってaとbに垂直なベクトル(法線)を求める
		const float cx = a.y * b.z - a.z * b.y;
		const float cy = a.z * b.x - a.x * b.z;
		const float cz = a.x * b.y - a.y * b.x;

		// 法線を正規化して単位ベクトルにする
		const float l = sqrt(cx * cx + cy * cy + cz * cz);
		const vec3 normal = { cx / l, cy / l, cz / l };

		
	 // 法線が設定されていない頂点にだけ法線を加算
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
		// 法線を正規化
	 for (int i = 0; i < vertexCount; ++i) {
		if (missingNormals[i]) {
			vec3 & n = vertices[i].normal;
			const float l = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
			n = { n.x / l, n.y / l, n.z / l };
			
		}
		
	} // for i
}

// スタティックメッシュを取得
StaticMeshPtr MeshBuffer::GetStaticMesh(const char* name) const
{
	auto itr = meshes.find(name);
	if (itr != meshes.end()) {
		return itr->second;
		
	}
	 return nullptr;
}

//===================================================================================//
// スケルタルメッシュを取得
SkeletalMeshPtr MeshBuffer::GetSkeletalMesh(const char* name) const
{
	auto itr = skeletalMeshes.find(name);
	if (itr != skeletalMeshes.end()) {
		return itr->second;
		
	}
	 return nullptr;
}
//====================================================================================//