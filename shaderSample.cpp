
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "model.h"
#include "shaderSample.h"
#include "bullet.h"
#include "meshField.h"
#include "myGui.h"
#include "sphere.h"

#include <string>


void ShaderSample::Init()
{
	Scene* scene = Manager::GetInstance().GetScene();

	m_Model = new Model();
	m_Model->Load("asset\\model\\torus2\\torus.obj");

	m_Position = { 0.0f,3.0f,-3.0f };
	//m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };

	m_Quaternion = XMQuaternionIdentity();


	m_Sphere = scene->AddGameObject<SphereDebugDraw>(1);
	m_Sphere->SetRadius(1.5f);
	m_Sphere->SetColor({ 0.0f,10.0f,0.0f,1.0f });


	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "depthShadowMappingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "depthShadowMappingPS.cso");


}

void ShaderSample::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	//m_Sphere->SetDestroy();
}

void ShaderSample::Update()
{
	//if (Input::GetKeyPress('Q')) m_Rotation.x -= 0.1f;
	//if (Input::GetKeyPress('E')) m_Rotation.x += 0.1f;

	m_Sphere->SetPosition(m_Position);
	

	if (Input::GetKeyPress('A'))
	{
		AddWorldOffsetFromVector(GetRightVector(), -0.1f);
		
		XMVECTOR quat = XMQuaternionIdentity();
		XMFLOAT3 axis = { 0.0f, 0.0f, 1.0f };
		quat = XMQuaternionRotationAxis(XMLoadFloat3(&axis), 0.1f);
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, quat);
		
	
	}

	if (Input::GetKeyPress('D'))
	{
		AddWorldOffsetFromVector(GetRightVector(), 0.1f);

		XMVECTOR quat = XMQuaternionIdentity();
		XMFLOAT3 axis = { 0.0f, 0.0f, -1.0f };
		quat = XMQuaternionRotationAxis(XMLoadFloat3(&axis), 0.1f);
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, quat);

	}

	//D3DXVECTOR3 forward = GetForward();

	if (Input::GetKeyPress('W'))
	{
		AddWorldOffsetFromVector(GetForwardVector(), 0.1f);

		XMVECTOR quat = XMQuaternionIdentity();
		XMFLOAT3 axis = { 1.0f, 0.0f, 0.0f };
		quat = XMQuaternionRotationAxis(XMLoadFloat3(&axis), 0.1f);
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, quat);

	}

	if (Input::GetKeyPress('S'))
	{
		AddWorldOffsetFromVector(GetForwardVector(), -0.1f);
		XMVECTOR quat = XMQuaternionIdentity();
		XMFLOAT3 axis = { -1.0f, 0.0f, 0.0f };
		quat = XMQuaternionRotationAxis(XMLoadFloat3(&axis), 0.1f);
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, quat);

	}

	if (Input::GetKeyPress('Q')) m_Rotation.y -= 0.05f;
	if (Input::GetKeyPress('E')) m_Rotation.y += 0.05f;

	

	/*Scene* scene = Manager::GetInstance().GetScene();

	MeshField* meshField = scene->GetGameObject<MeshField>(1);

	m_Position.y = meshField->GetHeight(m_Position) + 1.2f;*/

}

void ShaderSample::Draw()
{
	Scene* scene = Manager::GetInstance().GetScene();
	MeshField* meshField = scene->GetGameObject<MeshField>(1);
	
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シャドウバッファテクスチャを１番へセット
	ID3D11ShaderResourceView* shadowDepthTexture = Renderer::GetInstance().GetShadowDepthTexture();
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);

	//シェーダ設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	XMMATRIX mat;
	//マトリクス設定
	m_ScaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_RotMtx = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	mat = XMMatrixRotationQuaternion(m_Quaternion);
	m_TransMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_WorldMtx = m_ScaleMtx * mat * m_RotMtx  * m_TransMtx;

	Renderer::GetInstance().SetWorldMatrix(m_WorldMtx);

	m_Model->Draw();

	
}
