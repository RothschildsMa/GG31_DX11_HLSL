
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "fade.h"
#include "sprite2D.h"

void Fade::Fade_Start(FadeMode mode, float r, float g, float b, int frame)
{
	m_FadeMode = mode;
	m_Color = { r, g, b, 0.0f };
	m_Prop = 0.0f;
	m_FadeFrame = frame;
	m_FadeStartFrame = m_FrameCount;

}


void Fade::Init()
{
	m_FadeMode = FadeMode::FADE_NONE;
	m_FadeFrame = 0;
	m_FrameCount = 0;
	m_FadeStartFrame = 0;
	m_Prop = 0.0f;
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_TextureId = Texture::GetInstance().SetLoadFile("asset/texture/fade.png");
	Texture::GetInstance().Load();
}

void Fade::Uninit()
{
	Texture::GetInstance().SetReleaseFile(&m_TextureId, 1);
}

void Fade::Update()
{

	m_FrameCount++;

	//フェードしてなかったら何もすることはない
	if (m_FadeMode == FadeMode::FADE_NONE) return;

	//現在のフェードフレーム数を算出
	int frame = m_FrameCount - m_FadeStartFrame;

	//最終フェードフレーム数から0～1の進行割合を算出する
	m_Prop = (float)frame / m_FadeFrame;

	//フェードの割合が100%超えていたら、終了
	if (m_Prop >= 1.0f)
	{
		m_FadeMode = FadeMode::FADE_NONE;
		m_Prop = 1.0f;
	}

}

void Fade::Draw()
{

	if (m_FadeMode != FADE_NONE)
	{
		m_Color.w = m_FadeMode == FADE_IN ? 1.0f - m_Prop : m_Prop;
	}
	//ほぼ透明なので描画しない
	if (m_Color.w <= 0.0001f) return;

	Sprite2D::GetInstance().SetColor(m_Color);

	Sprite2D::GetInstance().SpriteDraw(m_TextureId, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 8, 8);

	Sprite2D::GetInstance().SetColor({1.0f,1.0f,1.0f,1.0f});
}
