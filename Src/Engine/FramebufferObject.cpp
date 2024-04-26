/**
* @file FramebufferObject.cpp
*/
#include "FramebufferObject.h"
 #include "Debug.h"

/**
* 既存のテクスチャからFBOを作成するコンストラクタ
* 
* @param 作成するFBOの数
* @param 作成したFBOのIDを格納する変数アドレス
*/
FramebufferObject::FramebufferObject(
	const TexturePtr& color, const TexturePtr& depth)
	: texColor(color), texDepth(depth)
{
	glCreateFramebuffers(1, &fbo);

	// カラーバッファを設定
	if (color) {
		//必要に応じてカラーテクスチャや深度テクスチャを割り当て
		glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, *color, 0);
		width = texColor->GetWidth();
		height = texColor->GetHeight();

	}
	else {

		//GL_NONE(ジーエル・ノン): カラーを描画しないv
		//GL_COLOR_ATTACHIMENT0～GL_COLOR_ATTACHIMENT31: カラーバッファ
		//カラーは描画しない
		glNamedFramebufferDrawBuffer(fbo, GL_NONE);

	}

	// 深度バッファを設定
	if (depth) {

		glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, *depth, 0);
		width = texDepth->GetWidth();
		height = texDepth->GetHeight();

	}

	// FBOのエラーチェック
	if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glDeleteFramebuffers(1, &fbo);
		fbo = 0;
		texColor.reset();
		texDepth.reset();
		LOG_ERROR("FBOの作成に失敗");
		return;

	}

	// 作成成功
	LOG("FBO(width=%d, height=%d)を作成", width, height);

}

/**
* デストラクタ
*/
FramebufferObject::~FramebufferObject()
{
	glDeleteFramebuffers(1, &fbo);
}