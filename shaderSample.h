#pragma once

#include "gameobject.h"


class ShaderSample : public GameObject
{
private:

	class Model* m_Model;
	class SphereDebugDraw* m_Sphere;

	ID3D11VertexShader*			m_VertexShader[7];
	ID3D11PixelShader*			m_PixelShader[7];
	ID3D11InputLayout*			m_VertexLayout[7];

	XMVECTOR m_Quaternion;
	XMFLOAT4 m_Parameter;

	int m_Type;

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};
