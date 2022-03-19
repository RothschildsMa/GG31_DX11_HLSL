#pragma once

#include "gameobject.h"
#include "collision.h"
#include "audio.h"
#include "sphere.h"

class Player :  public GameObject
{
protected:
	class Shadow* m_Shadow;
	class SphereDebugDraw* m_Sphere;
	class Audio* m_ShootSE[2];
	class Bullet* m_Bullet;

	XMVECTOR m_NextTarget;
	XMVECTOR m_CurrentTarget;
	XMVECTOR m_TargetDir;

	Circle m_Circle;
	ContainmentType m_ContainsTest;
	int m_ShootFrame;
	float m_HpValue;
	float m_Time;
	bool m_IsMoving;
	bool m_IsRotating;
	bool m_IsShooting;

public:
	static void Load();
	static void Unload();

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SubHp(float value);
	void RotateToTarget();
	void MoveToTarget();
	
	SphereDebugDraw* GetSphere() { return m_Sphere; }
	const ContainmentType& GetContainsTest() { return m_ContainsTest; }
};
