

#pragma once


#include "main.h"
#include "singleton.h"
#include "manager.h"
#include "renderer.h"


class DebugDraw : public Singleton<DebugDraw>
{
private:
	ID3D11Buffer*		 m_VertexBuffer;

	ID3D11VertexShader*			 m_VertexShader;
	ID3D11PixelShader*			 m_PixelShader;
	ID3D11InputLayout*			 m_VertexLayout;

	XMFLOAT2 m_Start;
	XMFLOAT2 m_End;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

};
