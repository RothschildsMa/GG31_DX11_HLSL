
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "mouse.h"
#include "mousePicker.h"

void MousePicker::Init()
{
	m_TextureId = Texture::GetInstance().SetLoadFile("asset/texture/cursor_default.png");
	Texture::GetInstance().Load();

}

void MousePicker::Uninit()
{
	Texture::GetInstance().SetReleaseFile(&m_TextureId, 1);

	

}



void MousePicker::Update()
{

	XMFLOAT2 pos = Mouse::GetInstance().GetMousePosition();
	m_Position = { pos.x,pos.y };

}

void MousePicker::Draw()
{
	Sprite2D::GetInstance().SpriteDraw(m_TextureId, m_Position.x, m_Position.y, 40.0f, 40.0f, 96, 0, 48, 48);


}

