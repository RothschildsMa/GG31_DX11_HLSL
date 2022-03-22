#include "main.h"
#include "renderer.h"
#include "shadowDepthView.h"
#include "myGui.h"

void ShadowDepthView::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = { 0.0f, 0.0f, 0.0f };
	vertex[0].Normal = { 0.0f, 0.0f, 0.0f };
	vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[0].TexCoord = { 0.0f, 0.0f };
	
	vertex[1].Position = { 500.0f, 0.0f, 0.0f };
	vertex[1].Normal = { 0.0f, 0.0f, 0.0f };
	vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { 0.0f, 500.0f, 0.0f };
	vertex[2].Normal = { 0.0f, 0.0f, 0.0f };
	vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { 500.0f, 500.0f, 0.0f };
	vertex[3].Normal = { 0.0f, 0.0f, 0.0f };
	vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	
	////�e�N�X�`���[�ǂݍ���
	//D3DX11CreateShaderResourceViewFromFile(Renderer::GetInstance().GetDevice(),
	//										"asset/texture/rockHp.png",
	//										NULL,
	//										NULL,
	//										&m_Texture,
	//										NULL);
	
	//assert(m_Texture);

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

}

void ShadowDepthView::Uninit()
{
	m_VertexBuffer->Release();
	//m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ShadowDepthView::Update()
{

}

void ShadowDepthView::Draw()
{
	if (!MyGui::ShadowDepthView) return;

	//���̓��C�A�E�g�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	Renderer::GetInstance().SetWorldViewProjection2D();

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	////�e�N�X�`���[�ݒ�
	ID3D11ShaderResourceView* shadowDepthTexture = Renderer::GetInstance().GetShadowDepthTexture();
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &shadowDepthTexture);

	//�v���~�e�B�u�g�|���W�[�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);

}

