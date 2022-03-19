#pragma once

#include "gameobject.h"

class Field : public GameObject
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*   m_Texture = NULL;
	ID3D11ShaderResourceView*   m_NormalTexture = NULL;
	

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	BoundingBox m_Box;
	XMFLOAT3 m_Intersection;
	
	bool m_RayTest;
	float m_Lenth;


public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	const XMFLOAT3& GetIntersection() { return m_Intersection; }
	const bool& GetRayTest() { return m_RayTest; }
};
