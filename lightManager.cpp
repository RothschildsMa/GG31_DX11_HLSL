

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "myGui.h"


#include "lightManager.h"

static float Near = 100.0f;
static float Far = 1000.0f;

void LightManager::Init()
{

	m_DirectionLight.Enable = true;
	m_DirectionLight.Position = { -60.0f, 120.0f, -60.0f,1.0f };
	m_DirectionLight.Direction = { 1.0f, -1.0f, 1.0f, 0.0f };
	m_DirectionLight.Direction = XMVector4Normalize(m_DirectionLight.Direction);
	m_DirectionLight.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	m_DirectionLight.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_Focus = { 0.0f, 0.0f, 0.0f };
	m_Up = { 0.0f, 1.0f, 0.0f };

}

void LightManager::Uninit()
{
}

void LightManager::Update()
{

	ImGui::Begin("DirectionLight");
	if (ImGui::CollapsingHeader("Position"))
	{
		ImGui::SliderFloat("PosX", &m_DirectionLight.Position.x, -100.0f, 100.0f);
		ImGui::SliderFloat("PosY", &m_DirectionLight.Position.y, 10.0f, 120.0f);
		ImGui::SliderFloat("PosZ", &m_DirectionLight.Position.z, -100.0f, 100.0f);
	}

	if (ImGui::CollapsingHeader("Projection"))
	{
		ImGui::SliderFloat("Near", &Near, 1.0f, 200.0f);
		ImGui::SliderFloat("Far", &Far, 120.0f, 1000.0f);

	}


	ImGui::End();


	m_DirectionLight.ViewMatrix = XMMatrixLookAtLH(XMLoadFloat4(&m_DirectionLight.Position), XMLoadFloat3(&m_Focus), XMLoadFloat3(&m_Up));
	m_DirectionLight.ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, Near, Far);


}

void LightManager::Draw()
{
	Renderer::GetInstance().SetLight(m_DirectionLight);

	//*******１パス目 シャドウバッファの作成*******
	Renderer::GetInstance().BeginDepth();
	//ライトカメラの行列をセット
	Renderer::GetInstance().SetViewMatrix(m_DirectionLight.ViewMatrix);
	Renderer::GetInstance().SetProjectionMatrix(m_DirectionLight.ProjectionMatrix);



}

void LightManager::Clear()
{
	m_DirectionLight.Enable = false;
	Renderer::GetInstance().SetLight(m_DirectionLight);
}
