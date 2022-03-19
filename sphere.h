

#pragma once

#include "main.h"
#include "singleton.h"
#include "manager.h"
#include "renderer.h"
#include "gameobject.h"

#define UMAX (10)
#define VMAX (5)

class SphereDebugDraw : public GameObject
{

private:
	static ID3D11Buffer*		 m_VertexBuffer;
	static ID3D11Buffer*		 m_IndexBuffer;

	static ID3D11VertexShader*		m_VertexShader;
	static ID3D11PixelShader*		m_PixelShader;
	static ID3D11InputLayout*		m_VertexLayout;

	static VERTEX_3D* m_VertexArray;
	static WORD* m_IndexArray;

	static int m_VertexCount;
	static int m_IndexCount;

	float m_Radius;
	BoundingSphere m_BoundingSphere;
	ContainmentType m_ContainsTest;
	XMFLOAT4 m_Color;

	float m_Lenth;
	bool m_RayTest;
	bool m_RectTest;
	bool m_DrawFlag;

	bool m_Chosen;
	
public:

	static void Load();
	static void Unload();

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetRadius(float r) { m_Radius = r; }
	void SetColor(XMFLOAT4 color) { m_Color = color; }

	bool GetChosen() { return m_Chosen; }
	bool GetRayTest() { return m_RayTest; }
	const BoundingSphere& GetBoundingSphere() { return m_BoundingSphere; }
	const ContainmentType& GetContainsTest() { return m_ContainsTest; }
};

