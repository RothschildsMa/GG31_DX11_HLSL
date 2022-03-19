
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "enemy.h"
#include "scene.h"
#include "player.h"
#include "enemyBullet.h"
#include "hp.h"
#include "mouse.h"
#include "mouseEvent.h"
#include "mousePicker.h"
#include "collision.h"
#include "camera.h"
#include "airplane.h"


Model* Enemy::m_Model;

ID3D11VertexShader*		Enemy::m_VertexShader = NULL;
ID3D11PixelShader*		Enemy::m_PixelShader = NULL;
ID3D11InputLayout*		Enemy::m_VertexLayout = NULL;

void Enemy::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\enemy\\enemy01.obj");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
	
}

void Enemy::Unload()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Enemy::Init()
{
	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.1f,0.1f,0.1f };
	m_HpValue = 1.0f;
	
	Scene* scene = Manager::GetInstance().GetScene();
	m_SphereDebug = scene->AddGameObject<SphereDebugDraw>(1);
	m_Hp = scene->AddGameObject<HP>(1);
	m_Hp->SetTextureTag(HP::Type::EnemyHp, 5, 1, 5, 0, m_HpValue, 0.0f);
	//m_Hp->SetType(m_Hp->EnemyHp);
	
	m_Test = scene->AddGameObject<Audio>(2);
	m_Test->Load("asset\\audio\\test.wav");

	
}

void Enemy::Uninit()
{

}

void Enemy::Update()
{
	m_Hp->SetPosition({ m_Position.x,m_Position.y + 2.0f,m_Position.z });
	m_Hp->SetHp(m_HpValue);
	m_Hp->SetTextureTag(HP::Type::EnemyHp, 5, 1, 5, 0, m_HpValue,0.0f);
	m_ShootFrame++;
	Scene* scene = Manager::GetInstance().GetScene();
	Player* player = scene->GetGameObject<Player>(1);
	Camera* camera = scene->GetGameObject<Camera>(0);

	

	if (m_ShootFrame >= 120)
	{
		m_ShootFrame = 0;
		XMFLOAT3 bulletPosition;
		XMStoreFloat3(&bulletPosition, XMVectorAdd(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_ForwardVector) * 2.0f));
		m_bullet = scene->AddGameObject<EnemyBullet>(1);
		m_bullet->SetPosition({ bulletPosition.x,bulletPosition.y + 1.3f,bulletPosition.z });
		m_bullet->SetDirection(m_ForwardVector);

	}

	if (m_HpValue <= -1.0f)
	{
		SetDestroy();
		m_Hp->SetDestroy();
		m_SphereDebug->SetDestroy();
	}

	m_SphereDebug->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
	m_SphereDebug->SetRadius(1.5f);
	m_SphereDebug->SetColor({ 10.0f,0.0f,0.0f,1.0f });

}

void Enemy::Draw()
{
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	Player* player = Manager::GetInstance().GetScene()->GetGameObject<Airplane>(1);
	XMFLOAT3 playerPosition = player->GetPosition();

	//マトリクス設定
	m_ScaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_RotMtx = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_TransMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	
	m_LookAtMtx = LookAtMatrixAxisFix(m_Position, playerPosition, { 0.0f, 1.0f, 0.0f });
	
	m_WorldMtx = m_ScaleMtx * m_RotMtx * m_LookAtMtx * m_TransMtx;
	Renderer::GetInstance().SetWorldMatrix(m_WorldMtx);

	XMFLOAT4X4 mtxLook;
	XMStoreFloat4x4(&mtxLook, m_LookAtMtx);
	m_ForwardVector = { mtxLook._31,mtxLook._32,mtxLook._33 };

	m_Model->Draw();
}
