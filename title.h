#pragma once

#include"scene.h"
#include"collision.h"
#include"sprite2D.h"


class Title : public Scene
{
public:
	enum TitleMode
	{
		TITLE_FADE_IN,
		TITLE_SHOW,
		TITLE_FADE_OUT

	};

private:

	Rect m_RectStart;
	Rect m_RectExit;
	TitleMode m_TitleMode;

	static int m_TextureId[3];

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};