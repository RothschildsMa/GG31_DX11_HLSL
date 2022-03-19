
#include "mouse.h"
#include "scene.h"
#include "sphere.h"
#include "collision.h"
#include "mouseEvent.h"
#include "player.h"
#include "field.h"
#include "explosion.h"
#include "line.h"


ID3D11Buffer*			LineDebugDraw::m_VertexBuffer = nullptr;

void LineDebugDraw::Load()
{
	//頂点バッファ生成
	{
		VERTEX_3D vertex[2];

		vertex[0].Diffuse = { 0.0f,10.0f,0.0f,1.0f };
		vertex[1].Diffuse = { 0.0f,10.0f,0.0f,1.0f };

		D3D11_BUFFER_DESC vbDesc;
		ZeroMemory(&vbDesc, sizeof(vbDesc));
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		vbDesc.ByteWidth = sizeof(VERTEX_3D) * 2;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA vrData;
		ZeroMemory(&vrData, sizeof(vrData));
		vrData.pSysMem = vertex;

		Renderer::GetInstance().GetDevice()->CreateBuffer(&vbDesc, &vrData, &m_VertexBuffer);
	}
}

void LineDebugDraw::Unload()
{
	m_VertexBuffer->Release();
}

void LineDebugDraw::Init()
{
	
	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };

	m_Start = { 0.0f,0.0f,0.0f };
	m_End = { 0.0f,0.0f,0.0f };
}

void LineDebugDraw::Uninit()
{

}

void LineDebugDraw::Update()
{
	Scene* scene = Manager::GetInstance().GetScene();

	m_Start = m_Position;
	
}

void LineDebugDraw::Draw()
{
	//マトリクス設定
	m_ScaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_RotMtx = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_TransMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_WorldMtx = m_ScaleMtx * m_RotMtx * m_TransMtx;

	

	//vertex3D頂点情報を書き換えーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetInstance().GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* v = (VERTEX_3D*)msr.pData;

	//v[0].Position = m_Start;
	v[1].Position = m_End;
	v[0].Diffuse = { 0.0f,10.0f,0.0f,1.0f };
	v[1].Diffuse = { 0.0f,10.0f,0.0f,1.0f };

	Renderer::GetInstance().GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	
	Renderer::GetInstance().SetWorldMatrix(m_WorldMtx);


	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//プリミティブトポロジー設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(2, 0);
}
