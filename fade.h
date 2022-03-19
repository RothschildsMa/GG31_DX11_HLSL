/*=====================================================================================================

	画面フェードの制御[fade.h]
																author:Ma
																date:2020/08/10
-------------------------------------------------------------------------------------------------------

=======================================================================================================*/
#pragma once

#include "singleton.h"

enum FadeMode
{
	FADE_NONE,
	FADE_IN,
	FADE_OUT
};

class Fade : public Singleton<Fade>
{
private:

	int m_FadeFrame;
	int m_FadeStartFrame;
	FadeMode m_FadeMode;
	int m_FrameCount;
	float m_Prop;
	XMFLOAT4 m_Color;

	int m_TextureId;

public:

	bool IsFade() { return m_FadeMode != FADE_NONE; }

	void Fade_Start(FadeMode mode, float r, float g, float b, int frame);

	void Init();
	void Uninit();
	void Update();
	void Draw();

};




