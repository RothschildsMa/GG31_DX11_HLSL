
#include "main.h"
#include "renderer.h"
#include "bloomTexture.h"
#include "myGui.h"

void BloomTexture::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = { 0.0f, 0.0f, 0.0f };
	vertex[0].Normal = { 0.0f, 0.0f, 0.0f };
	vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { SCREEN_WIDTH, 0.0f, 0.0f };
	vertex[1].Normal = { 0.0f, 0.0f, 0.0f };
	vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { 0.0f, SCREEN_HEIGHT, 0.0f };
	vertex[2].Normal = { 0.0f, 0.0f, 0.0f };
	vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f };
	vertex[3].Normal = { 0.0f, 0.0f, 0.0f };
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

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "bloomVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "bloomPS.cso");

}

void BloomTexture::Uninit()
{
	m_VertexBuffer->Release();
	//m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void BloomTexture::Update()
{

}

void BloomTexture::Draw()
{
	if (!MyGui::BloomTexture) return;


	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::GetInstance().SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	ID3D11ShaderResourceView* ppTexture = Renderer::GetInstance().GetRenderTexture();
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &ppTexture);


	//プリミティブトポロジー設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);

}

