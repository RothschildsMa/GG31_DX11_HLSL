#pragma once

#include"gameobject.h"

class Camera : public GameObject
{
protected:
	XMFLOAT3 m_Target;
	XMMATRIX m_ViewMatrix;
	XMMATRIX m_ProjectionMatrix;
	XMVECTOR m_Eye;
	XMVECTOR m_Focus;
	XMVECTOR m_Up;

public:

	virtual void Init();
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw();

	XMMATRIX GetViewMatrix() { return m_ViewMatrix; }
	XMMATRIX GetProjectionMatrix() { return m_ProjectionMatrix; }

	
};

class DefaultCamera : public Camera
{

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;


};

class TPSCamera : public Camera
{
private:
	XMFLOAT3 m_Stick;
	XMVECTOR m_Direction;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetCameraStick(XMFLOAT3 Target, XMFLOAT3 Stick, XMVECTOR Direction) 
	{
		m_Target = Target; 
		m_Stick = Stick;
		m_Direction = Direction;
	}
};
