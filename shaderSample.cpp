
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

static const char* buff = "";
static bool flag[7];
static bool fieldFlag[2];

void ShaderSample::Init()
{
	Scene* scene = Manager::GetInstance().GetScene();

	m_Model = new Model();
	m_Model->Load("asset\\model\\torus2\\torus.obj");

	m_Position = { 0.0f,1.5f,-3.0f };
	//m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };

	m_Parameter = { 0.0f,0.0f,0.0f,0.0f };

	m_Quaternion = XMQuaternionIdentity();

	m_Type = 6;

	m_Sphere = scene->AddGameObject<SphereDebugDraw>(1);
	m_Sphere->SetRadius(1.5f);
	m_Sphere->SetColor({ 0.0f,10.0f,0.0f,1.0f });


	Renderer::GetInstance().CreateVertexShader(&m_VertexShader[0], &m_VertexLayout[0], "unlitVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader[0], "unlitPS.cso");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader[1], &m_VertexLayout[1], "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader[1], "unlitTexturePS.cso");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader[2], &m_VertexLayout[2], "vertexLightingVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader[2], "vertexLightingPS.cso");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader[3], &m_VertexLayout[3], "pixelLightingVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader[3], "pixelLightingPS.cso");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader[4], &m_VertexLayout[4], "pixelLightingVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader[4], "blinnPhongPS.cso");


	Renderer::GetInstance().CreateVertexShader(&m_VertexShader[5], &m_VertexLayout[5], "pixelLightingVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader[5], "rimLightingPS.cso");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader[6], &m_VertexLayout[6], "pixelLightingSpotVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader[6], "pixelLightingSpotPS.cso");

	/*for (int i = 0; i < 6; i++)
	{
		flag[i] = false;
	}*/
	//flag[6] = true;
	fieldFlag[0] = true;
	m_Parameter.x = 3.14159f * 0.5f;
}

void ShaderSample::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	for (int i = 0; i < 7; i++)
	{
		if (m_VertexLayout[i]) m_VertexLayout[i]->Release();
		if (m_VertexShader[i]) m_VertexShader[i]->Release();
		if (m_PixelShader[i]) m_PixelShader[i]->Release();
	}

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

	Scene* scene = Manager::GetInstance().GetScene();

	MeshField* meshField = scene->GetGameObject<MeshField>(1);

	m_Position.y = meshField->GetHeight(m_Position) + 1.2f;

	{
		//window描画
		ImGui::Begin("Setting");

		//ImGui::SetNextTreeNodeOpen(true);
		if (ImGui::CollapsingHeader("Torus"))
		{

			if (ImGui::TreeNode("Position"))
			{
				ImGui::Text("Position_X :");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%.1f", m_Position.x);
				ImGui::Text("Position_Y :");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%.1f", m_Position.y);
				ImGui::Text("Position_Z :");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%.1f", m_Position.z);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("ShaderType"))
			{

				ImGui::BeginChild("Scrolling");

				if (ImGui::Checkbox("unLit", &flag[0]))
				{
					m_Type = 0;
					for (int i = 0; i < 7; i++)
					{
						if (i == 0) continue;

						flag[i] = false;

					}
				}
				if (ImGui::Checkbox("unLitTexture", &flag[1]))
				{
					m_Type = 1;
					for (int i = 0; i < 7; i++)
					{
						if (i == 1) continue;

						flag[i] = false;

					}
				}
				if (ImGui::Checkbox("vertexLighting", &flag[2]))
				{
					m_Type = 2;

					for (int i = 0; i < 7; i++)
					{
						if (i == 2) continue;

						flag[i] = false;

					}
				}

				if (ImGui::Checkbox("pixelLightingPhone", &flag[3]))
				{
					m_Type = 3;

					for (int i = 0; i < 7; i++)
					{
						if (i == 3) continue;

						flag[i] = false;

					}
				}
				if (ImGui::Checkbox("blinnPhong", &flag[4]))
				{
					m_Type = 4;

					for (int i = 0; i < 7; i++)
					{
						if (i == 4) continue;

						flag[i] = false;

					}
				}
				if (ImGui::Checkbox("rimLighting", &flag[5]))
				{
					m_Type = 5;

					for (int i = 0; i < 7; i++)
					{
						if (i == 5) continue;

						flag[i] = false;

					}
				}
				if (ImGui::Checkbox("SpotLight", &flag[6]))
				{
					m_Type = 6;

					for (int i = 0; i < 7; i++)
					{
						if (i == 6) continue;

						flag[i] = false;

					}
				}
				ImGui::EndChild();
				ImGui::TreePop();
			}

			//ImGui::Text(buff);
		}

		if (ImGui::CollapsingHeader("Light"))
		{

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "SpotLight");
			ImGui::SliderFloat("Angle", &m_Parameter.x, 0.0f, 3.14159f * 0.5f);
			if (ImGui::Checkbox("Lit", &fieldFlag[0]))
			{
				meshField->SetType(0);
				fieldFlag[1] = false;
			}

			if (ImGui::Checkbox("UnLit", &fieldFlag[1]))
			{
				meshField->SetType(1);
				fieldFlag[0] = false;
			}

		}


		ImGui::End();

	}

	

}

void ShaderSample::Draw()
{
	Scene* scene = Manager::GetInstance().GetScene();
	MeshField* meshField = scene->GetGameObject<MeshField>(1);

	Renderer::GetInstance().SetParameter(m_Parameter);
	XMFLOAT2 pos = GetScreenPosition(m_Position);

	XMFLOAT2 posLeft = GetScreenPosition({ m_Position.x - 1.0f,	m_Position.y,			m_Position.z });
	XMFLOAT2 posRight = GetScreenPosition({ m_Position.x + 1.0f,	m_Position.y,			m_Position.z });
	XMFLOAT2 posTop = GetScreenPosition({ m_Position.x,			m_Position.y + 1.0f,	m_Position.z });
	XMFLOAT2 posBottom = GetScreenPosition({ m_Position.x,			m_Position.y - 1.0f,	m_Position.z });



	if (posRight.x < 0.0f || posBottom.y < 0.0f || posLeft.x > SCREEN_WIDTH || posTop.y > SCREEN_HEIGHT) {


		buff = "culling";

		return;
	}
	else
	{
		buff = "draw";
	}



	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout[m_Type]);

	//シェーダ設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader[m_Type], NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader[m_Type], NULL, 0);

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
