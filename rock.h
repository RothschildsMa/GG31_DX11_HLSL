#pragma once

#include "gameobject.h"

class Rock : public GameObject
{
private:

	class Model* m_Model;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};
