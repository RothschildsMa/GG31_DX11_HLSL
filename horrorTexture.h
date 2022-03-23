#pragma once

#include"gameobject.h"

class HorrorTexture : public GameObject
{
private:

	ID3D11Buffer*				 m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	 m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	XMFLOAT4 m_Parameter;

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;


};