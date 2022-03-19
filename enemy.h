#pragma once

#include "gameobject.h"
#include "collision.h"
#include "sphere.h"
#include "audio.h"

class Enemy : public GameObject
{
private:

	static Model* m_Model;
	class EnemyBullet* m_bullet;
	class HP* m_Hp;
	class SphereDebugDraw* m_SphereDebug;
	class Audio* m_Test;

	static ID3D11VertexShader*		m_VertexShader;
	static ID3D11PixelShader*		m_PixelShader;
	static ID3D11InputLayout*		m_VertexLayout;

	XMFLOAT3 m_Target;
	
	XMFLOAT3 m_ForwardVector;
	

	int m_ShootFrame;
	float m_HpValue;

public:

	static void Load();
	static void Unload();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	
	void SubHp(float value) { m_HpValue -= value; }
	
	SphereDebugDraw* GetSphere() { return m_SphereDebug; }
};

