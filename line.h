

#pragma once
#include "main.h"
#include "singleton.h"
#include "manager.h"
#include "renderer.h"
#include "gameobject.h"

class LineDebugDraw : public GameObject
{
private:
	static ID3D11Buffer*		 m_VertexBuffer;

	XMFLOAT3 m_Start;
	XMFLOAT3 m_End;

	float m_Lenth;

public:

	static void Load();
	static void Unload();

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetEnd(XMFLOAT3 end) { m_End = end; }

};

