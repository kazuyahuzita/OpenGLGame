/**
* @file FramebufferObject.h
*/
#ifndef FRAMEBUFFEROBJECT_H_INCLUDED
 #define FRAMEBUFFEROBJECT_H_INCLUDED
 #include "glad/glad.h"
 #include "Texture.h"

/**
* フレームバッファ・オブジェクト(FBO)
*/
class FramebufferObject
{
public:
	/**
	* テクスチャからFBOを作成するコンストラクタ
	*
	* @param color カラーテクスチャ
	* @param depth 深度テクスチャ
	*/
	FramebufferObject(const TexturePtr& color, const TexturePtr& depth);
	~FramebufferObject();

	// コピーと代入を禁止
	FramebufferObject(const FramebufferObject&) = delete;
	FramebufferObject& operator=(const FramebufferObject&) = delete;

	// 管理番号を取得
	operator GLuint() const { return fbo; }

	// 幅と高さを取得
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	// テクスチャを取得
	const TexturePtr& GetColorTexture() const { return texColor; }
	const TexturePtr& GetDepthTexture() const { return texDepth; }

private:
	GLuint fbo = 0;      // FBOの管理番号
	int width = 0;       // FBOの幅
	int height = 0;      // FBOの高さ
	TexturePtr texColor; // カラーテクスチャ
	TexturePtr texDepth; // 深度テクスチャ
};
using FramebufferObjectPtr = std::shared_ptr<FramebufferObject>;

#endif // FRAMEBUFFEROBJECT_H_INCLUDED