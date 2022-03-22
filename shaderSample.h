#pragma once

#include "gameobject.h"

#define ShaderTypeMax (8)

class ShaderSample : public GameObject
{
private:
	class TPSCamera* m_TpsCamera;
	class Model* m_Model;
	class SphereDebugDraw* m_Sphere;

	ID3D11VertexShader*			m_VertexShader[ShaderTypeMax];
	ID3D11PixelShader*			m_PixelShader[ShaderTypeMax];
	ID3D11InputLayout*			m_VertexLayout[ShaderTypeMax];

	XMVECTOR m_Quaternion;
	XMFLOAT4 m_Parameter;

	int m_Type;

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};
