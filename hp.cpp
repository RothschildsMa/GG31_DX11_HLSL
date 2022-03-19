#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "hp.h"
#include "camera.h"


ID3D11Buffer*				HP::m_VertexBuffer = NULL;

ID3D11VertexShader*			HP::m_VertexShader = NULL;
ID3D11PixelShader*			HP::m_PixelShader = NULL;
ID3D11InputLayout*			HP::m_VertexLayout = NULL;

Texture_List HP::m_Texture[Type::TYPE_MAX] =
{
	{Type::EnemyHp, "asset/texture/enemyHp.png"},
	{Type::PlayerHp, "asset/texture/playerHp.png"}

};

void HP::Load()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = { -1.0f, 1.0f, 0.0f };
	vertex[0].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { 1.0f, 1.0f, 0.0f };
	vertex[1].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { -1.0f, -1.0f, 0.0f };
	vertex[2].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { 1.0f, -1.0f, 0.0f };
	vertex[3].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	for (int i = 0; i < Type::TYPE_MAX; i++)
	{
		//テクスチャー読み込み
		D3DX11CreateShaderResourceViewFromFile(
			Renderer::GetInstance().GetDevice(),
			m_Texture[i].fileName.c_str(),
			NULL,
			NULL,
			&m_Texture[i].pTexture,
			NULL);

		assert(m_Texture[i].pTexture);
	}

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

}

void HP::Unload()
{
	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


	for (int i = 0; i < Type::TYPE_MAX; i++)
	{
		m_Texture[i].pTexture->Release();
	}
}

void HP::Init()
{
	m_Position = { 0.0f,1.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,0.2f,1.0f };

	
	m_Handle = m_TexCount = m_CountMax = m_CountX = m_CountY = m_CurrentFrame = m_UpdateFrame = 0;
	m_Width = m_Height = 0.0f;
}

void HP::Uninit()
{
	
}

void HP::Update()
{
	m_CurrentFrame++;
	if (m_CurrentFrame >= m_UpdateFrame)
	{
		m_TexCount++;
		m_CurrentFrame = 0;
	}
	
}

void HP::Draw()
{
	UpdateAnimation();
	
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//カメラのビューマトリクス取得
	Scene* scene = Manager::GetInstance().GetScene();
	Camera* camera = scene->GetGameObject<Camera>(0);
	XMMATRIX view = camera->GetViewMatrix();

	//ビューの逆行列
	XMMATRIX invView = XMMatrixInverse(nullptr, view);
	XMFLOAT4X4 mat44;
	XMStoreFloat4x4(&mat44, invView);
	mat44._41 = 0.0f;
	mat44._42 = 0.0f;
	mat44._43 = 0.0f;
	invView = XMLoadFloat4x4(&mat44);

	//マトリクス設定
	m_ScaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_TransMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_WorldMtx = m_ScaleMtx * invView * m_TransMtx;

	Renderer::GetInstance().SetWorldMatrix(m_WorldMtx);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Renderer::GetInstance().SetMaterial(material);

	//テクスチャー設定
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[m_Handle].pTexture);

	//プリミティブトポロジー設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);

}


void HP::SetTextureTag(int handle, int countMax, int countX, int countY, int frame, float width, float height)
{
	m_Handle = handle;
	m_CountMax = countMax;
	m_CountX = countX;
	m_CountY = countY;
	m_UpdateFrame = frame;
	m_Width = width;
	m_Height = height;
}

void HP::UpdateAnimation()
{
	float x = m_TexCount % m_CountX * (1.0f / m_CountX);
	float y = m_TexCount / m_CountY * (1.0f / m_CountY);

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetInstance().GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* v = (VERTEX_3D*)msr.pData;

	v[0].Position = { -1.0f, 1.0f, 0.0f };
	v[0].Normal = { 0.0f, 1.0f, 0.0f };
	v[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[0].TexCoord = { x, y };

	v[1].Position = { m_Width, 1.0f, 0.0f };
	v[1].Normal = { 0.0f, 1.0f, 0.0f };
	v[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[1].TexCoord = { x + (1.0f / m_CountX), y };

	v[2].Position = { -1.0f, -1.0f - m_Height, 0.0f };
	v[2].Normal = { 0.0f, 1.0f, 0.0f };
	v[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[2].TexCoord = { x, y + (1.0f / m_CountY) };

	v[3].Position = { m_Width, -1.0f - m_Height, 0.0f };
	v[3].Normal = { 0.0f, 1.0f, 0.0f };
	v[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[3].TexCoord = { x + (1.0f / m_CountX), y + (1.0f / m_CountY) };


	Renderer::GetInstance().GetDeviceContext()->Unmap(m_VertexBuffer, 0);


}
