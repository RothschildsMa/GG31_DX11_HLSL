#pragma once

#include "gameobject.h"


class ShaderSample :  public GameObject
{
private:

	class Model* m_Model;
	class SphereDebugDraw* m_Sphere;


	ID3D11VertexShader*			m_VertexShader;
	ID3D11PixelShader*			m_PixelShader;
	ID3D11InputLayout*			m_VertexLayout;

	XMVECTOR m_Quaternion;

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};
