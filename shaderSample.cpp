
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "model.h"
#include "camera.h"
#include "shaderSample.h"
#include "bullet.h"
#include "meshField.h"
#include "myGui.h"
#include "sphere.h"

#include <string>

static const char* buff = "";
static bool g_Flag[ShaderTypeMax];
static bool fieldFlag[2];
static float g_Num = 1;  //fur用



static std::string g_TypeName[] = {
	"unLit",
	"unLitTexture",
	"vertexLighting",
	"pixelLightingPhone",
	"blinnPhong",
	"rimLighting",
	"SpotLight",
	"fur",
};

static const int g_TypeCnt = ARRAYSIZE(g_TypeName);

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

	m_TpsCamera = scene->AddGameObject<TPSCamera>(0);

	m_Sphere = scene->AddGameObject<SphereDebugDraw>(1);
	m_Sphere->SetRadius(1.5f);
	m_Sphere->SetColor({ 0.0f,10.0f,0.0f,1.0f });

	// Furテクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(
		Renderer::GetInstance().GetDevice(),
		"asset/texture/fur.png",
		NULL,
		NULL,
		&m_FurTexture,
		NULL);
	assert(m_FurTexture);


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

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader[7], &m_VertexLayout[7], "furVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader[7], "furPS.cso");

	/*for (int i = 0; i < 6; i++)
	{
		flag[i] = false;
	}*/
	//flag[6] = true;

	fieldFlag[0] = true;
	
	g_Flag[0] = true;

	m_Parameter.x = 3.14159f * 0.5f;
}

void ShaderSample::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	for (int i = 0; i < ShaderTypeMax; i++)
	{
		if (m_VertexLayout[i]) m_VertexLayout[i]->Release();
		if (m_VertexShader[i]) m_VertexShader[i]->Release();
		if (m_PixelShader[i]) m_PixelShader[i]->Release();
	}

	m_FurTexture->Release();

	//m_Sphere->SetDestroy();
}

void ShaderSample::Update()
{
	//if (Input::GetKeyPress('Q')) m_Rotation.x -= 0.1f;
	//if (Input::GetKeyPress('E')) m_Rotation.x += 0.1f;

	m_TpsCamera->SetCameraStick(m_Position, { 0.0f,5.0f,0.0f }, GetForwardVector());

	m_Sphere->SetPosition(m_Position);

	//fur用
	if (Input::GetKeyPress('Z'))
	{
		g_Num -= 0.5f;
		if (g_Num < 1.0f) g_Num = 1.0f;
	}
	if (Input::GetKeyPress('X'))
	{
		g_Num += 0.5f;
		if (g_Num > 500.0f) g_Num = 500.0f;
	}

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

	m_Position.y = meshField->GetHeight(m_Position) + 5.0f; //1.2f

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

			//shaderTypeの切り替え...checkBoxで操作する
			if (ImGui::TreeNode("ShaderType"))
			{
				ImGui::BeginChild("Scrolling");

				for (int i = 0; i < ShaderTypeMax; i++)
				{
					if (ImGui::Checkbox(g_TypeName[i].c_str(), &g_Flag[i]))
					{
						m_Type = i;
						for (int j = 0; j < ShaderTypeMax; j++)
						{
							if (j == m_Type) continue;
							g_Flag[j] = false;
						}

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

	// furテクスチャ
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(1, 1, &m_FurTexture);
	//ファー表現のために繰り返しモデルを描画する
	for (int i = 0; i < (int)g_Num; i++)
	{
		XMFLOAT4 param;
		param.x = i; //現在の描画回数をシェーダーのParameter.xへ渡す
		Renderer::GetInstance().SetParameter(param);
		m_Model->Draw();
	}

	//m_Model->Draw();


}
