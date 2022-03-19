#pragma once

#include "scene.h"
#include "sprite2D.h"
#include"collision.h"

class Resault : public Scene
{
public:
	enum ResaultMode
	{
		RESAULT_FADE_IN,
		RESAULT_SHOW,
		RESAULT_FADE_OUT
	};

private:
	static int m_TextureId[3];
	Rect m_Rect;
	ResaultMode m_ResaultMode;

	static bool m_Flag;
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	static void SetFlag(bool flag) { m_Flag = flag; }
};