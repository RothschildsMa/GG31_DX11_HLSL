

#pragma once


#include "main.h"
#include "singleton.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"


class MouseEvent : public Singleton<MouseEvent>
{
private:
	ID3D11Buffer*		 m_VertexBuffer;

	ID3D11VertexShader*			 m_VertexShader;
	ID3D11PixelShader*			 m_PixelShader;
	ID3D11InputLayout*			 m_VertexLayout;

	XMFLOAT2 m_Start;
	XMFLOAT2 m_End;

	Rect m_Rect;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	const Rect& GetRect() { return m_Rect; }

};
