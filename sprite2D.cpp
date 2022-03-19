
#include "main.h"
#include "renderer.h"
#include "sprite2D.h"

void Sprite2D::Init(void)
{
	//頂点バッファ生成
	{
		VERTEX_3D vertex[4] = {};

		D3D11_BUFFER_DESC vbDesc;
		ZeroMemory(&vbDesc, sizeof(vbDesc));
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		vbDesc.ByteWidth = sizeof(VERTEX_3D) * 4;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA vrData;
		ZeroMemory(&vrData, sizeof(vrData));
		vrData.pSysMem = vertex;

		Renderer::GetInstance().GetDevice()->CreateBuffer(&vbDesc, &vrData, &m_VertexBuffer);
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

		Renderer::GetInstance().GetDevice()->CreateBuffer(&ibDesc, &irData, &m_IndexBuffer);

	}

	m_Color = { 1.0f,1.0f, 1.0f, 1.0f };

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

}	


void Sprite2D::Uninit(void)
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void Sprite2D::SpriteDraw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, float cx, float cy, float angle)
{
	unsigned long w = Texture::GetInstance().GetWidth(textureId);
	unsigned long h = Texture::GetInstance().GetHeight(textureId);

	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	//vertex3D頂点情報を書き換えーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetInstance().GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* v = (VERTEX_3D*)msr.pData;

	v[0] = { { 0.0f,	  0.0f,		 0.0f},{ 0.0f, 0.0f, 0.0f}, m_Color,{ u0,v0 } };
	v[1] = { { 0.0f + dw, 0.0f,		 0.0f},{ 0.0f, 0.0f, 0.0f}, m_Color,{ u1,v0 } };
	v[2] = { { 0.0f,	  0.0f + dh, 0.0f},{ 0.0f, 0.0f, 0.0f}, m_Color,{ u0,v1 } };
	v[3] = { { 0.0f + dw, 0.0f + dh, 0.0f},{ 0.0f, 0.0f, 0.0f}, m_Color,{ u1,v1 } };
	
	XMMATRIX TransCenterMtx = XMMatrixTranslation(-cx, -cy, 0.0f);
	XMMATRIX m_TransMtx = XMMatrixTranslation(cx + dx, cy + dy, 0.0f);
	XMMATRIX m_RotMtx = XMMatrixRotationZ(angle);

	XMMATRIX m_WorldMtx = TransCenterMtx * m_RotMtx * m_TransMtx;

	XMVECTOR pos;

	for (int i = 0; i < 4; i++)
	{
		pos = XMVector3Transform(XMLoadFloat3(&v[i].Position), m_WorldMtx);

		XMStoreFloat3(&v[i].Position, pos);
	}

	Renderer::GetInstance().GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

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

	// インデックスバッファ設定
	Renderer::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	ID3D11ShaderResourceView* texture = Texture::GetInstance().GetTexture(textureId);

	//テクスチャー設定
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1,&texture);

	//プリミティブトポロジー設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->DrawIndexed(6, 0, 0);

}
