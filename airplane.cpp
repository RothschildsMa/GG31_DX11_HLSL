
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "model.h"
#include "player.h"
#include "bullet.h"
#include "hp.h"
#include "polygon2D.h"
#include "shadow.h"
#include "gamePad.h"
#include "mouse.h"
#include "sphere.h"
#include "camera.h"
#include "explosion.h"
#include "collision.h"
#include "mouse.h"
#include "field.h"
#include "enemy.h"
#include "airplane.h"


ID3D11VertexShader*		Airplane::m_VertexShader = nullptr;
ID3D11PixelShader*		Airplane::m_PixelShader = nullptr;
ID3D11InputLayout*		Airplane::m_VertexLayout = nullptr;

Model* Airplane::m_Model;

void Airplane::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\air00\\air00.obj");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "rimLightingPS.cso");
}

void Airplane::Unload()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Airplane::Init()
{
	Scene* scene = Manager::GetInstance().GetScene();

	m_Position = { 0.0f,1.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.1f,0.1f,0.1f };

	m_ShootFrame = 0;
	m_HpValue = 1.0f;
	m_Time = 0.0f;
	m_IsMoving = false;
	m_IsRotating = false;
	m_IsShooting = false;
	m_ContainsTest = ContainmentType::DISJOINT;

	m_NextTarget = XMVectorZero();
	m_CurrentTarget = XMVectorZero();
	m_TargetDir = XMVectorZero();
	m_Circle = { { 0.0f,0.0f }, 20.0f };

	m_Sphere = scene->AddGameObject<SphereDebugDraw>(1);

	m_Shadow = scene->AddGameObject<Shadow>(1);


	m_Hp = scene->AddGameObject<HP>(1);

	m_Hp->SetTextureTag(HP::Type::PlayerHp, 5, 1, 5, 0, m_HpValue, 0.0f);


	//m_Sphere->SetScale({ 2.0f,2.0f,2.0f });

	m_ShootSE[0] = scene->AddGameObject<Audio>(2);
	m_ShootSE[0]->Load("asset\\audio\\02.wav");
	m_ShootSE[1] = scene->AddGameObject<Audio>(2);
	m_ShootSE[1]->Load("asset\\audio\\03.wav");
}

void Airplane::Uninit()
{
	m_Sphere->SetDestroy();
	m_Shadow->SetDestroy();

}

void Airplane::Update()
{
	Scene* scene = Manager::GetInstance().GetScene();

	Field* field = scene->GetGameObject<Field>(1);

	m_Shadow->SetPosition({ m_Position.x,m_Position.y - 0.99f,m_Position.z });
	m_Sphere->SetPosition(m_Position);
	m_Sphere->SetRadius(1.5f);
	m_Sphere->SetColor({ 0.0f,10.0f,0.0f,1.0f });

	m_Hp->SetPosition({ m_Position.x,m_Position.y + 2.5f,m_Position.z });
	m_Hp->SetHp(m_HpValue);
	m_Hp->SetTextureTag(HP::Type::PlayerHp, 5, 1, 5, 0, m_HpValue,0.0f);

	m_ShootFrame++;

	if (Mouse::GetInstance().OnMouseTrigger(Right) && m_Sphere->GetChosen() && field->GetRayTest())
	{
		m_Time = 0.0f;
		m_IsShooting = false;
		m_IsRotating = true;
		m_IsMoving = true;
		XMFLOAT3 Pos = field->GetIntersection();

		m_NextTarget = XMVectorSet(Pos.x, m_Position.y, Pos.z, 0.0f);

		m_CurrentTarget = XMLoadFloat3(&m_Position);

		m_TargetDir = XMVector3Normalize(XMVectorSubtract(m_NextTarget, m_CurrentTarget));

	}

	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(1);

	for (Enemy* enemy : enemyList)
	{
		if (Mouse::GetInstance().OnMousePress(Right) && m_Sphere->GetChosen())
		{
			float distance = GetDistanceFromVector(m_NextTarget, XMLoadFloat3(&enemy->GetPosition()));
			if (distance <= 3.0f || enemy->GetSphere()->GetRayTest())
			{
				if (GetDistance(m_Position, enemy->GetPosition()) <= 20.5f)
				{
					m_IsMoving = false;
					m_IsShooting = true;

				}
			}
		}

	}

	if (m_IsRotating)
	{
		this->RotateToTarget();

	}
	if (m_IsMoving)
	{
		this->MoveToTarget();

	}

	if (m_IsShooting)
	{
		if (m_ShootFrame >= 12)
		{

			m_Bullet = scene->AddGameObject<Bullet>(1);
			m_Bullet->SetDir(GetForwardVector());
			m_Bullet->SetPosition(m_Position);
			m_ShootSE[0]->Play();

			m_ShootFrame = 0;
		}
	}


	//std::vector<SphereDebugDraw*> spherelist = scene->GetGameObjects<SphereDebugDraw>(1);

	std::vector<Airplane*> airlist = scene->GetGameObjects<Airplane>(1);
	
	for (Airplane* air : airlist)
	{
		if (this == air) continue;
		if (GetDistance(m_Position, air->GetPosition()) < 1.0f)
		{
			m_Position.x += 0.5f;
		}
	}
	

	if (m_HpValue <= -1.0f)
	{
		SetDestroy();
	}

}

void Airplane::Draw()
{
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	m_ScaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_RotMtx = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_TransMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_WorldMtx = m_ScaleMtx * m_RotMtx * m_TransMtx;
	
	Renderer::GetInstance().SetWorldMatrix(m_WorldMtx);


	m_Model->Draw();
}
