#include "main.h"
#include "renderer.h"
#include "shadow.h"

ID3D11Buffer*				Shadow::m_VertexBuffer = NULL;
ID3D11ShaderResourceView*   Shadow::m_Texture = NULL;

ID3D11VertexShader*		Shadow::m_VertexShader = NULL;
ID3D11PixelShader*		Shadow::m_PixelShader = NULL;
ID3D11InputLayout*		Shadow::m_VertexLayout = NULL;

void Shadow::Load()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = { -1.0f, 0.1f, 1.0f };
	vertex[0].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { 1.0f, 0.1f, 1.0f };
	vertex[1].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { -1.0f, 0.1f, -1.0f };
	vertex[2].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { 1.0f, 0.1f, -1.0f };
	vertex[3].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャー読み込み
	D3DX11CreateShaderResourceViewFromFile(
		Renderer::GetInstance().GetDevice(),
		"asset/texture/shadow.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
	
}

void Shadow::Unload()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Shadow::Init()
{
	

	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };

}

void Shadow::Uninit()
{

}

void Shadow::Update()
{

}

void Shadow::Draw()
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
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジー設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::GetInstance().SetDepthEnable(false);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);

	Renderer::GetInstance().SetDepthEnable(true);

}
