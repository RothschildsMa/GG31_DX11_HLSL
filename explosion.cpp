#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "explosion.h"
#include "camera.h"


ID3D11Buffer*				Explosion::m_VertexBuffer = nullptr;
ID3D11Buffer*				Explosion::m_IndexBuffer = nullptr;
							
ID3D11VertexShader*			Explosion::m_VertexShader = nullptr;
ID3D11PixelShader*			Explosion::m_PixelShader = nullptr;
ID3D11InputLayout*			Explosion::m_VertexLayout = nullptr;

Texture_List Explosion::m_Texture[TYPE::TYPE_MAX]=
{
	{TYPE::TYPE_EXP00, "asset/texture/explosion/00.png"},
	{TYPE::TYPE_EXP01, "asset/texture/explosion/01.png"},
	{TYPE::TYPE_EXP02, "asset/texture/explosion/02.png"},
	{TYPE::TYPE_EXP03, "asset/texture/explosion/03.png"},
	{TYPE::TYPE_EXP04, "asset/texture/explosion/04.png"}
};

void Explosion::Load()
{
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
	}

	// インデックスバッファ生成
	{
		WORD Index[6] = {
			0, 1, 2,
			1, 3, 2,
		};

		D3D11_BUFFER_DESC ibDesc;
		ZeroMemory(&ibDesc, sizeof(ibDesc));
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA irData;
		ZeroMemory(&irData, sizeof(irData));
		irData.pSysMem = Index;

		Singleton<Renderer>::GetInstance().GetDevice()->CreateBuffer(&ibDesc, &irData, &m_IndexBuffer);

	}

	for (int i = 0; i < TYPE::TYPE_MAX; i++)
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

void Explosion::Unload()
{
	m_IndexBuffer->Release();
	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_Texture[i].pTexture->Release();
	}
}

void Explosion::Init()
{
	
	m_Position = { 0.0f,1.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };

	m_Handle = m_TexCount = m_CountMax = m_CountX = m_CountY = m_CurrentFrame = m_UpdateFrame = 0;
	m_Width = m_Height = 0.0f;

	

}

void Explosion::Uninit()
{
	
}

void Explosion::Update()
{
	m_CurrentFrame++;
	if (m_CurrentFrame >= m_UpdateFrame)
	{
		m_TexCount++;
		m_CurrentFrame = 0;
	}

	if (m_TexCount >= m_CountMax)
	{
		SetDestroy();
		return;
	}
}

void Explosion::Draw()
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

	Renderer::GetInstance().SetDepthEnable(false);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);

	Renderer::GetInstance().SetDepthEnable(true);
}

void Explosion::SetTextureTag(int handle, int countMax, int countX, int countY, int frame, float width, float height)
{
	m_Handle = handle;
	m_CountMax = countMax;
	m_CountX = countX;
	m_CountY = countY;
	m_UpdateFrame = frame;
	m_Width = width;
	m_Height = height;
}

void Explosion::UpdateAnimation()
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

	v[1].Position = { 1.0f + m_Width, 1.0f, 0.0f };
	v[1].Normal = { 0.0f, 1.0f, 0.0f };
	v[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[1].TexCoord = { x + (1.0f / m_CountX), y };

	v[2].Position = { -1.0f, -1.0f - m_Height, 0.0f };
	v[2].Normal = { 0.0f, 1.0f, 0.0f };
	v[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[2].TexCoord = { x, y + (1.0f / m_CountY) };

	v[3].Position = { 1.0f + m_Width, -1.0f - m_Height, 0.0f };
	v[3].Normal = { 0.0f, 1.0f, 0.0f };
	v[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	v[3].TexCoord = { x + (1.0f / m_CountX), y + (1.0f / m_CountY) };


	Renderer::GetInstance().GetDeviceContext()->Unmap(m_VertexBuffer, 0);


}
