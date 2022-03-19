
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"


Model* Bullet::m_Model;//再度宣言

ID3D11VertexShader*		Bullet::m_VertexShader = NULL;
ID3D11PixelShader*		Bullet::m_PixelShader = NULL;
ID3D11InputLayout*		Bullet::m_VertexLayout = NULL;



void Bullet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\playerbullet\\playerbullet.obj");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
	
}

void Bullet::Unload()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Bullet::Init()
{
	m_Position = { 0.0f,1.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.1f,0.1f,0.1f };

	m_LifeFrame = 0;
	
}

void Bullet::Uninit()
{
	Scene* scene = Manager::GetInstance().GetScene();
	m_Explosion = scene->AddGameObject<Explosion>(1);
	m_Explosion->SetTextureTag(Explosion::TYPE::TYPE_EXP02, 30, 5, 6);
	m_Explosion->SetPosition(m_Position);
}

void Bullet::Update() 
{
	m_LifeFrame++;
	Scene* scene = Manager::GetInstance().GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(1);

	AddWorldOffsetFromVector(m_Dir,0.6f);
	
	m_Rotation.z += 0.1f;

		
	for (Enemy* enemy : enemyList)
	{
		XMFLOAT3 enemyPosition = enemy->GetPosition();


		if (GetDistance(m_Position, enemyPosition) < 2.0f)
		{
			enemy->SubHp(0.4f);
			SetDestroy();
			return;
		}
	}

	if (m_LifeFrame > 300)
	{
		SetDestroy();
		return;
	}

}

void Bullet::Draw()
{
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	XMVECTOR forward = m_Dir * 10000;
	XMFLOAT3 look;
	XMStoreFloat3(&look, forward);
	//マトリクス設定
	m_ScaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_RotMtx = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_LookAtMtx = LookAtMatrixAxisFix(m_Position, look, { 0.0f, 1.0f, 0.0f });
	m_TransMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_WorldMtx = m_ScaleMtx * m_RotMtx * m_LookAtMtx * m_TransMtx;

	Renderer::GetInstance().SetWorldMatrix(m_WorldMtx);

	m_Model->Draw();
}
