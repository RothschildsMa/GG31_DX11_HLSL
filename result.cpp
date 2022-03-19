
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "gamePad.h"
#include "result.h"
#include "title.h"
#include "fade.h"
#include "mouse.h"
#include "airplane.h"

int Resault::m_TextureId[3];
bool Resault::m_Flag = false;

void Resault::Init()
{
	m_TextureId[0] = Texture::GetInstance().SetLoadFile("asset/texture/resault/resaultBG02.png");
	m_TextureId[1] = Texture::GetInstance().SetLoadFile("asset/texture/resault/next.png");
	m_TextureId[2] = Texture::GetInstance().SetLoadFile("asset/texture/resault/resaultBG01.png");

	

	m_Rect = { {800.0f,500.0f},{210.0f,210.0f} };
	m_ResaultMode = Resault::ResaultMode::RESAULT_FADE_IN;
	Fade::GetInstance().Fade_Start(FadeMode::FADE_IN, 0.0f, 0.0f, 0.0f, 60 * 3);

	Texture::GetInstance().Load();
}

void Resault::Uninit()
{
	Scene::Uninit();

	for (int i = 0; i < 3; i++)
	{
		Texture::GetInstance().SetReleaseFile(&m_TextureId[i], 1);;
	}

}

void Resault::Update()
{
	Scene::Update();
	if (Input::GetKeyTrigger(VK_SPACE) || GamePad::GetInstance().JoystickPress(ButtonB))
	{
		Manager::GetInstance().SetScene<Title>();
	}

	switch (m_ResaultMode)
	{
	case RESAULT_FADE_IN:
		if (!Fade::GetInstance().IsFade())
		{
			m_ResaultMode = RESAULT_SHOW;
		}
		break;


	case RESAULT_SHOW:

		if (Collision::PointAndRect(Mouse::GetInstance().GetMousePosition(), m_Rect))
		{
			if (Mouse::GetInstance().OnMousePress(Left))
			{
				m_ResaultMode = RESAULT_FADE_OUT;
				Fade::GetInstance().Fade_Start(FadeMode::FADE_OUT, 0.0f, 0.0f, 0.0f, 60);

			}
		}

		break;


	case RESAULT_FADE_OUT:
		if (!Fade::GetInstance().IsFade())
		{
			//éüÇÃÉVÅ[ÉìÇ÷à⁄çs
			Manager::GetInstance().SetScene<Title>();
		}
		break;

	}
}

void Resault::Draw()
{
	Scene::Draw();
	
	if (m_Flag)
	{
		Sprite2D::GetInstance().SpriteDraw(m_TextureId[2], 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1600, 900);

	}
	else
	{
		Sprite2D::GetInstance().SpriteDraw(m_TextureId[0], 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1600, 900);

	}


	Sprite2D::GetInstance().SpriteDraw(m_TextureId[1], m_Rect.Position.x, m_Rect.Position.y, m_Rect.Size.x, m_Rect.Size.y, 0, 0, 210, 210);



	
}


