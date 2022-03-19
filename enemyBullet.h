#pragma once

#include "gameobject.h"
#include "explosion.h"

class EnemyBullet : public GameObject
{
private:

	static Model* m_Model;

	static ID3D11VertexShader*			m_VertexShader;
	static ID3D11PixelShader*			m_PixelShader;
	static ID3D11InputLayout*			m_VertexLayout;

	class Explosion* m_Explosion;
	XMFLOAT3 m_Direction;
	int m_LifeFrame;

public:
	static void Load();
	static void Unload();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetDirection(const XMFLOAT3& direction) { m_Direction = direction; }

};

