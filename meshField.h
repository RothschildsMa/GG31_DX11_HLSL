#pragma once

#include "gameobject.h"

class MeshField : public GameObject
{
private:

	ID3D11Buffer*				m_VertexBuffer = nullptr;
	ID3D11Buffer*				m_IndexBuffer = nullptr;
	ID3D11ShaderResourceView*   m_Texture = nullptr;
	ID3D11ShaderResourceView*   m_NormalTexture = nullptr;

	VERTEX_3D                   m_Vertex[21][21];

	ID3D11VertexShader*			m_VertexShader = nullptr;
	ID3D11PixelShader*			m_PixelShader;
	ID3D11InputLayout*			m_VertexLayout = nullptr;

	LIGHT m_Light;
	int m_Type;

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	float GetHeight(XMFLOAT3 Position);
	void SetType(int type) { m_Type = type; }

};
