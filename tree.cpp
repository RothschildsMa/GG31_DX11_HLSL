#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "tree.h"
#include "camera.h"

ID3D11Buffer*				Tree::m_VertexBuffer = NULL;
ID3D11ShaderResourceView*   Tree::m_Texture = NULL;

ID3D11VertexShader*			Tree::m_VertexShader = NULL;
ID3D11PixelShader*			Tree::m_PixelShader = NULL;
ID3D11InputLayout*			Tree::m_VertexLayout = NULL;

void Tree::Load()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = { -4.0f, 8.0f, 0.0f };
	vertex[0].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { 4.0f, 8.0f, 0.0f };
	vertex[1].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { -4.0f, 0.0f, 0.0f };
	vertex[2].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { 4.0f, 0.0f, 0.0f };
	vertex[3].Normal = { 0.0f, 1.0f, 0.0f };
	vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���[�ǂݍ���
	D3DX11CreateShaderResourceViewFromFileA(
		Renderer::GetInstance().GetDevice(),
		"asset/texture/tree.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

}

void Tree::Unload()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Tree::Init()
{

	m_Position = { 0.0f,4.5f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };

}

void Tree::Uninit()
{

}

void Tree::Update()
{
	
}

void Tree::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�J�����̃r���[�}�g���N�X�擾
	Scene* scene = Manager::GetInstance().GetScene();
	TPSCamera* camera = scene->GetGameObject<TPSCamera>(0);
	XMMATRIX view = camera->GetViewMatrix();

	//�r���[�̋t�s��
	XMMATRIX invView = XMMatrixInverse(nullptr, view);
	XMFLOAT4X4 mat44;
	XMStoreFloat4x4(&mat44, invView);
	mat44._41 = 0.0f;
	mat44._42 = 0.0f;
	mat44._43 = 0.0f;
	invView = XMLoadFloat4x4(&mat44);

	//�}�g���N�X�ݒ�
	m_ScaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_TransMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_WorldMtx = m_ScaleMtx * invView * m_TransMtx;

	Renderer::GetInstance().SetWorldMatrix(m_WorldMtx);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Renderer::GetInstance().SetMaterial(material);

	//�e�N�X�`���[�ݒ�
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�[�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);

}
