#include"main.h"
#include"manager.h"
#include"renderer.h"
#include"camera.h"
#include"input.h"
#include"player.h"
#include"skybox.h"
#include"scene.h"
#include"shaderSample.h"


void DefaultCamera::Init()
{
	m_Position = { 0.0f,40.0f,20.0f };
	m_Target = { 0.0f,0.0f,0.0f };
}

void DefaultCamera::Uninit()
{

}

void DefaultCamera::Update()
{


}

void DefaultCamera::Draw()
{
	Camera::Draw();
}

void TPSCamera::Init()
{
	Camera::Init();
}

void TPSCamera::Uninit()
{
	
}

void TPSCamera::Update()
{
	/*Skybox* s = Manager::GetInstance().GetScene()->GetGameObject<Skybox>(1);

	if (Input::GetKeyPress('A')) m_Target.x -= 0.3f;
	if (Input::GetKeyPress('D')) m_Target.x += 0.3f;
	if (Input::GetKeyPress('W')) m_Target.z += 0.3f;
	if (Input::GetKeyPress('S')) m_Target.z -= 0.3f;

	//XMVECTOR height = { 0.0f, 5.0f, 0.0f,0.0f };
	//トップビュー
	//XMStoreFloat3(&m_Position,XMVectorAdd(XMLoadFloat3(&m_Target), player->GetUpVector() * 10.0f));

	//サードパーソンビュー
	XMVECTOR springlength = XMVectorSubtract(XMLoadFloat3(&m_Target), s->GetForwardVector() * 20.0f);
	//XMVECTOR springlength = { 0.0f,0.0f,0.0f,0.0f };
	XMFLOAT3 height = { 0.0f,30.0f,0.0f };
	XMStoreFloat3(&m_Position, XMVectorAdd(springlength, XMLoadFloat3(&height)));*/

	//ShaderSample* sample = Manager::GetInstance().GetScene()->GetGameObject<ShaderSample>(1);
	//m_Target = sample->GetPosition();

	//トップビュー
	//m_Position = m_Target + D3DXVECTOR3(0.0f, 5.0f, -8.0f);

	//XMFLOAT3 spring = { 0.0f,5.0f,0.0f };

	//サードパーソンビュー
	//XMVECTOR forward = sample->GetForwardVector();
	XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Target) - m_Direction * 10.0f + XMLoadFloat3(&m_Stick));

	//if (Input::GetKeyPress('F')) m_Target.x -= 0.3f;
	//if (Input::GetKeyPress('G')) m_Target.x += 0.3f;

}

void TPSCamera::Draw()
{
	Camera::Draw();
	
}

void Camera::Init()
{
	m_Position = { 0.0f,0.0f,1.0f };
	m_Target = { 0.0f,0.0f,0.0f };
}

void Camera::Draw()
{
	XMFLOAT3 pos = { 0.0f,3.0f,10.0f };
	//ビューマトリクス設定
	m_Eye = XMLoadFloat3(&m_Position);
	m_Focus = XMLoadFloat3(&m_Target);
	m_Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_ViewMatrix = XMMatrixLookAtLH(m_Eye, m_Focus, m_Up);

	Renderer::GetInstance().SetViewMatrix(m_ViewMatrix);


	//プロジェクションマトリクス設定
	float Fov = 1.0f;
	float Aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	float NearZ = 1.0f;
	float FarZ = 1000.0f;

	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(Fov, Aspect, NearZ, FarZ);

	Renderer::GetInstance().SetProjectionMatrix(m_ProjectionMatrix);
	Renderer::GetInstance().SetCameraPosition(m_Position);

}
