

#include "mouse.h"
#include "sphere.h"
#include "collision.h"
#include "mouseEvent.h"
#include "scene.h"
#include "player.h"

ID3D11Buffer*			SphereDebugDraw::m_VertexBuffer = nullptr;
ID3D11Buffer*			SphereDebugDraw::m_IndexBuffer = nullptr;

ID3D11VertexShader*		SphereDebugDraw::m_VertexShader = nullptr;
ID3D11PixelShader*		SphereDebugDraw::m_PixelShader = nullptr;
ID3D11InputLayout*		SphereDebugDraw::m_VertexLayout = nullptr;

VERTEX_3D*		SphereDebugDraw::m_VertexArray = nullptr;
WORD*			SphereDebugDraw::m_IndexArray = nullptr;



int SphereDebugDraw::m_VertexCount;
int SphereDebugDraw::m_IndexCount;


void SphereDebugDraw::Load()
{
	m_VertexCount = UMAX * (VMAX + 1);
	m_IndexCount = 2 * VMAX * (UMAX + 1);

	//頂点バッファ生成
	{
		m_VertexArray = new VERTEX_3D[m_VertexCount];

		D3D11_BUFFER_DESC vbDesc;
		ZeroMemory(&vbDesc, sizeof(vbDesc));
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		vbDesc.ByteWidth = sizeof(VERTEX_3D) * m_VertexCount;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA vrData;
		ZeroMemory(&vrData, sizeof(vrData));
		vrData.pSysMem = m_VertexArray;

		Renderer::GetInstance().GetDevice()->CreateBuffer(&vbDesc, &vrData, &m_VertexBuffer);
	}

	// インデックスバッファ生成
	{
		m_IndexArray = new WORD[m_IndexCount];
		int count = 0;

		for (int i = 0; i < VMAX; i++)
		{
			for (int j = 0; j < UMAX + 1; j++)
			{
				if (j == UMAX)
				{
					m_IndexArray[count] = i * UMAX;
					count += 1;
					m_IndexArray[count] = (i + 1) * UMAX;
					count += 1;
				}
				else
				{
					m_IndexArray[count] = (i * UMAX) + j;
					count += 1;
					m_IndexArray[count] = m_IndexArray[count - 1] + UMAX;
					count += 1;
				}
			}
		}

		D3D11_BUFFER_DESC ibDesc;
		ZeroMemory(&ibDesc, sizeof(ibDesc));
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.ByteWidth = sizeof(WORD) * m_IndexCount;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA irData;
		ZeroMemory(&irData, sizeof(irData));
		irData.pSysMem = m_IndexArray;

		Renderer::GetInstance().GetDevice()->CreateBuffer(&ibDesc, &irData, &m_IndexBuffer);

	}

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

void SphereDebugDraw::Unload()
{
	delete[] m_IndexArray;
	delete[] m_VertexArray;

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void SphereDebugDraw::Init()
{
	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };

	m_Radius = 1.0f;
	m_BoundingSphere = { {0.0f,0.0f,0.0f},m_Radius };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Lenth = 0.0f;
	m_RayTest = false;
	m_RectTest = false;
	m_DrawFlag = false;
	m_Chosen = false;
	m_ContainsTest = ContainmentType::DISJOINT;
}

void SphereDebugDraw::Uninit()
{

}

void SphereDebugDraw::Update()
{
	m_BoundingSphere.Center = { m_Position.x,m_Position.y,m_Position.z };
	m_BoundingSphere.Radius = m_Radius;

	XMVECTOR Near, Far;

	Mouse::GetInstance().GetRay(Near, Far);

	//レイと球の交差判定
	m_RayTest = m_BoundingSphere.Intersects(Near, XMVector3Normalize(XMVectorSubtract(Far, Near)), m_Lenth);

	if (Mouse::GetInstance().OnMouseTrigger(Left) && m_RayTest)
	{
		m_DrawFlag = true;
		m_Chosen = true;
	}

	if (!m_RayTest && Mouse::GetInstance().OnMouseTrigger(Left))
	{
		m_DrawFlag = false;
		m_Chosen = false;
	}
	Rect rect = MouseEvent::GetInstance().GetRect();
	XMFLOAT2 pos2D = GetScreenPosition(m_Position);
	m_RectTest = Collision::PointAndRect(pos2D, rect);


	if (m_RectTest && Mouse::GetInstance().OnMouseRelease(Left))
	{
		m_DrawFlag = true;
		m_Chosen = true;
	}

	/*Scene* scene = Manager::GetInstance().GetScene();
	std::vector<SphereDebugDraw*> spherelist = scene->GetGameObjects<SphereDebugDraw>(1);
	std::vector<Player*> playerlist = scene->GetGameObjects<Player>(1);
	std::vector<GameObject*> objlist = scene->GetGameObjects<GameObject>(1);
	for (SphereDebugDraw* sphere : spherelist)
	{
		BoundingSphere s;
		s = sphere->GetBoundingSphere();
		m_ContainsTest = m_BoundingSphere.Contains(s);

	}*/




}

void SphereDebugDraw::Draw()
{

	//vertex3D頂点情報を書き換えーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetInstance().GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* v = (VERTEX_3D*)msr.pData;

	for (int i = 0; i < VMAX + 1; i++)
	{
		for (int j = 0; j < UMAX; j++)
		{
			float theta = XMConvertToRadians(180.0f * i / VMAX);
			float phi = XMConvertToRadians(360.0f * j / UMAX);
			float x = sinf(theta) * cosf(phi) * m_Radius;
			float y = cosf(theta) * m_Radius;
			float z = sinf(theta) * sinf(phi) * m_Radius;
			v[UMAX * i + j].Position = { x,y,z };
			v[UMAX * i + j].Diffuse = m_Color;
		}
	}


	Renderer::GetInstance().GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー


	m_ScaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_RotMtx = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_TransMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_WorldMtx = m_ScaleMtx * m_RotMtx * m_TransMtx;

	Renderer::GetInstance().SetWorldMatrix(m_WorldMtx);


	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//プリミティブトポロジー設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME; //ワイヤーフレームモードで描画
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	Renderer::GetInstance().GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs);

	Renderer::GetInstance().GetDeviceContext()->RSSetState(rs);

	if (m_DrawFlag)
	{
		//ポリゴン描画
		Renderer::GetInstance().GetDeviceContext()->DrawIndexed(m_IndexCount, 0, 0);
	}
	
	// ラスタライザステートを元に戻す
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	Renderer::GetInstance().GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs);

	Renderer::GetInstance().GetDeviceContext()->RSSetState(rs);

}
