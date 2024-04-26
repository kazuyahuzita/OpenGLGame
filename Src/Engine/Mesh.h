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
// 先行宣言
class MeshBuffer;
using MeshBufferPtr = std::shared_ptr<MeshBuffer>;


/**
* 頂点データ形式
*/
struct Vertex
{
	vec3 position; // 頂点座標
	vec2 texcoord; // テクスチャ座標
	vec4 texcoord4; // テクスチャ座標
	vec3 normal;	//法線ベクトル
};

/**
* マテリアル
*/
struct Material
{
	std::string name = "<Default>";  // マテリアル名
	vec4 baseColor = { 1, 1, 1, 1 }; // 基本色+アルファ
	vec3 emission = { 0, 0, 0 };     // 発光色
	TexturePtr texBaseColor;         // 基本色テクスチャ
	TexturePtr texEmission;          // 発光色テクスチャ
};
using MaterialPtr = std::shared_ptr<Material>;
using MaterialList = std::vector<MaterialPtr>; // マテリアル配列型

/**
* 描画パラメータ
*/
struct DrawParams
{
	GLenum mode = GL_TRIANGLES; // プリミティブの種類
	GLsizei count = 0;          // 描画するインデックス数	
	const void* indices = 0;    // 描画開始インデックスのバイトオフセット	
	GLint baseVertex = 0;       // インデックス0となる頂点配列内の位置	
	int materialNo = -1;        // マテリアルインデックス
};


/**
* 3Dモデル
*/
struct StaticMesh
{
	std::string name;    // メッシュ名	
	std::vector<DrawParams> drawParamsList; // 描画パラメータ	
	MaterialList materials; // 共有マテリアル配列
};
using StaticMeshPtr = std::shared_ptr<StaticMesh>;

void Draw(const StaticMesh& mesh, GLuint program, const MaterialList& materials);
//スケルタルメッシュでもできるように
//描画パラメータ配列を処理するプログラムを分離し
//どちらのメッシュでも描画できるようにする
void Draw(const std::vector<DrawParams>&drawParamsList, GLuint program,
	const MaterialList & materials, const vec4 * objectColor);

//何番目の座標変換行列を使うかというデータを持つ頂点データ型の追加
/**
* 関節付き頂点データ形式
*/
struct SkeletalVertex
{
	vec3 position; // 頂点座標
	vec2 texcoord; // テクスチャ座標
	vec3 normal;   // 法線ベクトル
	//複数の関節からの影響を再現するために変化している
	uint16_t joints[4];  // 影響を受ける関節の番号
	uint16_t weights[4]; // 各関節の影響度
};

/**
* 関節付きメッシュ
*/
struct SkeletalMesh
{
	std::string name;    // メッシュ名	
	std::vector<DrawParams> drawParamsList; // 描画パラメータ	
	MaterialList materials; // 共有マテリアル	
};
using SkeletalMeshPtr = std::shared_ptr<SkeletalMesh>;

// 共有マテリアル配列を複製する
inline MaterialList CloneMaterialList(const StaticMeshPtr& original)
{
	MaterialList clone(original->materials.size());
	for (int i = 0; i < clone.size(); ++i) {
		clone[i] = std::make_shared<Material>(*original->materials[i]);

	}
	return clone;
}
/**
* 頂点データを管理するクラス
*/
class MeshBuffer
{
public:
	/**
	* メッシュバッファを作成する
	*
	* @param bufferSize 格納できる頂点データのサイズ(バイト数)
	*/
	static MeshBufferPtr Create(size_t bufferSize)
	{
		return std::make_shared<MeshBuffer>(bufferSize);
	}

	// コンストラクタ・デストラクタ
	MeshBuffer(size_t bufferSize);
	~MeshBuffer() = default;

	// コピーと代入を禁止
	MeshBuffer(const MeshBuffer&) = delete;
	MeshBuffer& operator=(const MeshBuffer&) = delete;

	
	/**
	* テクスチャ作成コールバック型
	*
	* 引数に指定されたファイル名からテクスチャを作成する型
	*/
	using TextureCallback = std::function<TexturePtr(const char*)>;

	
	/**
	* テクスチャ作成コールバックを設定する
	*
	* @param callback 設定するコールバックオブジェクト
	*/
	void SetTextureCallback(const TextureCallback& callback)
	{
		textureCallback = callback;
	}
	/**
	* OBJファイルを読み込む
	*
	* @param filename OBJファイル名
	*
	* @return filenameから作成したメッシュ
	*/

	StaticMeshPtr LoadOBJ(const char* filename);

	// 関節データ
	struct Skeleton
	{

	};
	
	/**
	* OBJファイルを読み込み関節を設定する
	*
	* @param filename OBJファイル名
	* @param skeleton 関節データ
	*
	* @return filenameから作成した関節付きメッシュ
	*/
	SkeletalMeshPtr LoadOBJ(
			const char* filename, const Skeleton & skeleton);
	/**
	* 頂点データの追加
	*
	* @param vertices    GPUメモリにコピーする頂点データ配列
	* @param vertexBytes verticesのバイト数
	* @param indices     GPUメモリにコピーするインデックスデータ配列
	* @param indexBytes  indicesのバイト数
	* @param stride      頂点データの間隔(バイト数)
	* @param mode        プリミティブの種類
	*/
	void AddVertexData(const void* vertices, size_t vertexBytes,
		const uint16_t * indices, size_t indexBytes,
		size_t stride = sizeof(Vertex), GLenum mode = GL_TRIANGLES);
	
	/**
	* 既存の描画パラメータとテクスチャから新しいスタティックメッシュを作成する
	*
	* @param name         メッシュ名
	* @param params       メッシュの元になる描画パラメータ
	* @param texBaseColor メッシュに設定するベースカラーテクスチャ
	*
	* @return 作成したスタティックメッシュ
	*/
	StaticMeshPtr CreateStaticMesh(const char* name,
		const DrawParams & params, const TexturePtr & texBaseColor);


	// 全ての頂点データを削除
	void Clear();

	/**
	* スタティックメッシュの取得
	*
	* @param name スタティックメッシュの名前
	*
	* @return 名前がnameと一致するスタティックメッシュ
	*/
	StaticMeshPtr GetStaticMesh(const char* name) const;

	/**
	* スケルタルメッシュの取得
	*
	* @param name スケルタルメッシュの名前
	*
	* @return 名前がnameと一致するスケルタルメッシュ
	*/
	SkeletalMeshPtr GetSkeletalMesh(const char* name) const;
	// 描画パラメータの取得
	const DrawParams& GetDrawParams(size_t index) const
	{
		return drawParamsList[index];
	}

	// 描画パラメータの数を取得
	size_t GetDrawParamsCount() const { return drawParamsList.size(); }

	// VAOの取得
	VertexArrayObjectPtr GetVAO() const { return vao; }
	VertexArrayObjectPtr GetVAOSkeletal() const { return vaoSkeletal; }

private:
	std::vector<MaterialPtr> LoadMTL(
		const std::string & foldername, const char* filename);

	
	// メッシュ構築用の中間データ
	struct MeshData
	{
		std::vector<Vertex> vertices;  // 頂点データ
		std::vector<uint16_t> indices; // インデックスデータ
		std::vector<DrawParams> drawParamsList; // 描画パラメータ配列
		MaterialList materials;                 // マテリアル配列

	};
	MeshData CreateMeshDataFromOBJ(const char* filename);


	std::unordered_map<std::string, StaticMeshPtr> meshes;

	std::unordered_map<std::string, SkeletalMeshPtr> skeletalMeshes;

	std::vector<DrawParams> drawParamsList; // 描画パラメータ配列
	VertexArrayObjectPtr vao; // 頂点アトリビュート配列
	VertexArrayObjectPtr vaoSkeletal; // スケルタルメッシュ用の頂点配列
	BufferObjectPtr buffer;   // 頂点データおよびインデックスデータ
	size_t usedBytes = 0;     // バッファの使用済み容量(バイト)
	TextureCallback textureCallback; // テクスチャ作成コールバック
};


/**
* 欠けている法線を補う
*
* @param vertices    頂点配列
* @param vertexCount 頂点配列の要素数
* @param indices     インデックス配列
* @param indexCount  インデックス配列の要素数
*/
void FillMissingNormals(
	Vertex * vertices, size_t vertexCount,
	const uint16_t * indices, size_t indexCount);

#endif // MESH_H_INCLUDED