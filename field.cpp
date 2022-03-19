

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "mouse.h"
#include "scene.h"
#include "field.h"
#include "explosion.h"
#include "input.h"

void Field::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = { -10.0f, 0.0f, 10.0f };
	vertex[0].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { 10.0f, 0.0f, 10.0f };
	vertex[1].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[1].TexCoord = { 5.0f, 0.0f };

	vertex[2].Position = { -10.0f, 0.0f, -10.0f };
	vertex[2].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[2].TexCoord = { 0.0f, 5.0f };

	vertex[3].Position = { 10.0f, 0.0f, -10.0f };
	vertex[3].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[3].TexCoord = { 5.0f, 5.0f };

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
		"asset/texture/02.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	//テクスチャー読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetInstance().GetDevice(),
		"asset/texture/Rock_Normal.bmp",
		NULL,
		NULL,
		&m_NormalTexture,
		NULL);

	assert(m_NormalTexture);

	//Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	//Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	//Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingSpotVS.cso");

	//Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "pixelLightingSpotPS.cso");

	//Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "normalMappingVS.cso");
	//Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "normalMappingPS.cso");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "depthShadowMappingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "depthShadowMappingPS.cso");

	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };

	m_Box.Center = { m_Position.x,m_Position.y + 0.5f,m_Position.z };
	m_Box.Extents = { 50.0f,0.05f,50.0f };
	m_Intersection = { 0.0f,0.0f,0.0f };
	
	
	m_RayTest = false;
	
}

void Field::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Field::Update()
{
	XMVECTOR Near, Far;

	Mouse::GetInstance().GetRay(Near, Far);

	
	m_RayTest = m_Box.Intersects(Near, XMVector3Normalize(XMVectorSubtract(Far, Near)), m_Lenth);

	XMVECTOR vec = XMVector3Normalize(XMVectorSubtract(Far, Near)) * m_Lenth + Near;

	XMStoreFloat3(&m_Intersection, vec);

	Scene* scene = Manager::GetInstance().GetScene();
	/*if (m_RayTest)
	{
		Explosion* test;
		test = scene->AddGameObject<Explosion>(1);
		test->SetTextureTag(Explosion::TYPE::TYPE_EXP00, 30, 5, 6);
		
		test->SetPosition(m_Intersection);
	}*/

	

	
	
}

void Field::Draw()
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
	//Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(1, 1, &m_NormalTexture);

	ID3D11ShaderResourceView* shadowDepthTexture = Renderer::GetInstance().GetShadowDepthTexture();
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);

	//プリミティブトポロジー設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);

}
