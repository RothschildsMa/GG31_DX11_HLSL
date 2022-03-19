

#include "manager.h"
#include "scene.h"
#include "spotLight.h"
#include "shaderSample.h"

void SpotLight::Init()
{
	m_Light = {};
	m_Light.Enable = true;
	m_Light.Direction = { 0.0f,-1.0f,0.0f,0.0f };
	m_Light.Direction = XMVector4Normalize(m_Light.Direction);
	m_Light.Ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_Light.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Light.Position = { 0.0f,3.0f,0.0f };
	m_Light.Angle = { 3.14159f / 180.0f * 5.0f, 1.0f, 1.0f, 1.0f };

	

	m_Position = { 0.0f,0.0f,0.0f };
}

void SpotLight::Uninit()
{
}

void SpotLight::Update()
{
	Scene* scene = Manager::GetInstance().GetScene();
	ShaderSample* sample = scene->GetGameObject<ShaderSample>(1);

	m_Position = sample->GetPosition();
	
	m_Light.Position = { m_Position.x, m_Position.y + 10.0f, m_Position.z };

}

void SpotLight::Draw()
{
	Renderer::GetInstance().SetLight(m_Light);
}
