/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
 #define TEXTURE_H_INCLUDED
 #include "glad/glad.h"
 #include <string>
 #include <memory>

// 先行宣言
class Texture;
using TexturePtr = std::shared_ptr<Texture>;

/**
* テクスチャ管理クラス
*/
class Texture
{
	//指定したクラスをプライベートメンバで呼び出す
	friend class Engine;
public:
	// コピーと代入を禁止

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// 管理番号を取得

	operator GLuint() const { return id; }

	// 幅と高さを取得
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }


	// 画像のアスペクト比を取得
	float GetAspectRatio() const {
		return static_cast<float>(width) / static_cast<float>(height);


	}

private:
	//プライベートに置くことで、
	//Textureクラス自身とEngineクラス以外は
	//Textureオブジェクトを作れなくなる
	explicit Texture(const char* filename);

	/**
	* 空のテクスチャを作成するコンストラクタ
	*
	* @param name      テクスチャ識別用の名前
	* @param width     テクスチャの幅(ピクセル数)
	* @param height    テクスチャの高さ(ピクセル数)
	* @param gpuFormat データ形式
	* @param wrapMode  ラップモード
	*/
	Texture(const char* name, int width, int height,
		GLenum gpuFormat, GLenum wrapMode = GL_CLAMP_TO_EDGE);

	~Texture();

	std::string name; // テクスチャ名(主にデバッグ用)
	GLuint id = 0;    // オブジェクト管理番号
	int width = 0;    // テクスチャの幅
	int height = 0;   // テクスチャの高さ
};

#endif // TEXTURE_H_INCLUDED