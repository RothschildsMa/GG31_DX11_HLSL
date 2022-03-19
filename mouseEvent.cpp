
#include "mouse.h"
#include "mouseEvent.h"
#include "collision.h"

void MouseEvent::Init()
{
	//頂点バッファ生成
	{
		VERTEX_3D vertex[8] = {};

		D3D11_BUFFER_DESC vbDesc;
		ZeroMemory(&vbDesc, sizeof(vbDesc));
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		vbDesc.ByteWidth = sizeof(VERTEX_3D) * 8;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA vrData;
		ZeroMemory(&vrData, sizeof(vrData));
		vrData.pSysMem = vertex;

		Renderer::GetInstance().GetDevice()->CreateBuffer(&vbDesc, &vrData, &m_VertexBuffer);
	}

	m_Start = { 0.0f,0.0f };
	m_End = { 0.0f,0.0f };

	m_Rect = { {0.0f,0.0f},{0.0f,0.0f} };
}

void MouseEvent::Uninit()
{
	m_VertexBuffer->Release();
}

void MouseEvent::Update()
{
	if (Mouse::GetInstance().OnMouseTrigger(Left))
	{
		m_Start = Mouse::GetInstance().GetMousePosition();
		m_End = Mouse::GetInstance().GetMousePosition();
		m_Rect.Position = Mouse::GetInstance().GetMousePosition();
		//m_Rect.Size = Mouse::GetInstance().GetMousePosition();
	}

	if (Mouse::GetInstance().OnMousePress(Left))
	{
		m_End = Mouse::GetInstance().GetMousePosition();
		m_Rect.Size = { { m_End.x - m_Rect.Position.x },{m_End.y - m_Rect.Position.y} };
	}

	if (Mouse::GetInstance().OnMouseRelease(Left))
	{
		m_Start = Mouse::GetInstance().GetMousePosition();
		m_End = Mouse::GetInstance().GetMousePosition();
		
		//m_Rect.Size = Mouse::GetInstance().GetMousePosition();
		m_Rect.Size = { 0.0f,0.0f };
	}


}

void MouseEvent::Draw()
{
	//vertex3D頂点情報を書き換えーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetInstance().GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* v = (VERTEX_3D*)msr.pData;

	v[0].Position = { m_Start.x,m_Start.y,0.0f };
	v[1].Position = { m_End.x,m_Start.y,0.0f };

	v[2].Position = { m_End.x,m_Start.y,0.0f };
	v[3].Position = { m_End.x,m_End.y,0.0f };

	v[4].Position = { m_End.x,m_End.y,0.0f };
	v[5].Position = { m_Start.x,m_End.y,0.0f };

	v[6].Position = { m_Start.x,m_End.y,0.0f };
	v[7].Position = { m_Start.x,m_Start.y,0.0f };

	for (int i = 0; i < 8; i++)
	{
		v[i].Diffuse = { 0.0f,1.0f,0.0f,1.0f };
	}


	Renderer::GetInstance().GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

	//マトリクス設定
	Renderer::GetInstance().SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//プリミティブトポロジー設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(8, 0);
}
