

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "rock.h"

void Rock::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\skybox\\skybox.obj");
	m_Position = { 0.0f,1.0f,-3.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 50.0f,50.0f,50.0f };
	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");


}

void Rock::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Rock::Update()
{

}

void Rock::Draw()
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
