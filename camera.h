#pragma once

#include"gameobject.h"

class Camera : public GameObject
{
private:
	XMFLOAT3 m_Target;
	XMMATRIX m_ViewMatrix;
	XMMATRIX m_ProjectionMatrix;
	XMVECTOR m_Eye;
	XMVECTOR m_Focus;
	XMVECTOR m_Up;

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	XMMATRIX GetViewMatrix() { return m_ViewMatrix; }
	XMMATRIX GetProjectionMatrix() { return m_ProjectionMatrix; }

};