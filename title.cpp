
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "game.h"
#include "input.h"
#include "mouse.h"
#include "gamePad.h"
#include "title.h"
#include "fade.h"

int Title::m_TextureId[3];

void Title::Init()
{
	
	m_TextureId[0] = Texture::GetInstance().SetLoadFile("asset/texture/title/titleBG.png");
	m_TextureId[1] = Texture::GetInstance().SetLoadFile("asset/texture/title/start.png");
	m_TextureId[2] = Texture::GetInstance().SetLoadFile("asset/texture/exit.png");


	m_RectStart = { {680.0f,400.0f},{310.0f,100.0f} };
	m_RectExit = { {680.0f,600.0f},{310.0f,100.0f} };

	m_TitleMode = TitleMode::TITLE_FADE_OUT;
	//Fade::GetInstance().Fade_Start(FadeMode::FADE_IN, 0.0f, 0.0f, 0.0f, 60 * 3);

	Texture::GetInstance().Load();
}

void Title::Uninit()
{
	Scene::Uninit();

	for (int i = 0; i < 3; i++)
	{
		Texture::GetInstance().SetReleaseFile(&m_TextureId[i], 1);

	}

	
}

void Title::Update()
{
	Scene::Update();

	switch (m_TitleMode)
	{
	case TITLE_FADE_IN:
		if (!Fade::GetInstance().IsFade())
		{
			m_TitleMode = TITLE_SHOW;
		}
		break;

	case TITLE_SHOW:

		if (Collision::PointAndRect(Mouse::GetInstance().GetMousePosition(), m_RectStart))
		{
			m_RectStart = { {660.0f,380.0f},{340.0f,130.0f} };


			if (Mouse::GetInstance().OnMousePress(Left))
			{
				m_TitleMode = TITLE_FADE_OUT;
				Fade::GetInstance().Fade_Start(FadeMode::FADE_OUT, 0.0f, 0.0f, 0.0f, 60);
			}
		}
		else
		{
			m_RectStart = { {680.0f,400.0f},{310.0f,100.0f} };	
		}

		if (Collision::PointAndRect(Mouse::GetInstance().GetMousePosition(), m_RectExit))
		{
			
			m_RectExit = { {660.0f,580.0f},{340.0f,130.0f} };

			if (Mouse::GetInstance().OnMousePress(Left))
			{
				exit(1);
			}
		}
		else
		{
			m_RectExit = { {680.0f,600.0f},{310.0f,100.0f} };
		}

		if (Input::GetKeyTrigger(VK_SPACE) || GamePad::GetInstance().JoystickPress(ButtonB))
		{
			m_TitleMode = TITLE_FADE_OUT;
			Fade::GetInstance().Fade_Start(FadeMode::FADE_OUT, 0.0f, 0.0f, 0.0f, 60);
		}
		break;

	case TITLE_FADE_OUT:
		if (!Fade::GetInstance().IsFade())
		{
			//ŽŸ‚ÌƒV[ƒ“‚ÖˆÚs
			Manager::GetInstance().SetScene<Game>();
		}
		break;
	}

	

}

void Title::Draw()
{
	Scene::Draw();

	Sprite2D::GetInstance().SpriteDraw(m_TextureId[0], 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1600, 900);
	Sprite2D::GetInstance().SpriteDraw(m_TextureId[1], m_RectStart.Position.x, m_RectStart.Position.y, m_RectStart.Size.x, m_RectStart.Size.y, 0, 0, 410, 121);
	Sprite2D::GetInstance().SpriteDraw(m_TextureId[2], m_RectExit.Position.x, m_RectExit.Position.y, m_RectExit.Size.x, m_RectExit.Size.y, 0, 0, 410, 121);

}

