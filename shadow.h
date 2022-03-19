#pragma once

#include "gameobject.h"

class Shadow : public GameObject
{
private:

	static ID3D11Buffer*				m_VertexBuffer;
	static ID3D11ShaderResourceView*   m_Texture;
	
	static ID3D11VertexShader*			m_VertexShader;
	static ID3D11PixelShader*			m_PixelShader;
	static ID3D11InputLayout*			m_VertexLayout;

public:
	static void Load();
	static void Unload();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};
