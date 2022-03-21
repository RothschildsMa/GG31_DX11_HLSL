
#include "main.h"
#include "renderer.h"
#include "meshField.h"


float g_FieldHeight[21][21] = 
{
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,6.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,5.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,4.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f},
	{3.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f}
};

void MeshField::Init()
{
	//���_�o�b�t�@����
	{
		for (int x = 0; x < 21; x++)
		{
			for (int z = 0; z < 21; z++)
			{
				//g_FieldHeight[z][x]
				m_Vertex[x][z].Position = { (x - 10) * 5.0f,g_FieldHeight[z][x],(z - 10) * -5.0f };
				m_Vertex[x][z].Normal = { 0.0f,1.0f,0.0f };
				m_Vertex[x][z].Diffuse = { 1.0f,1.0f,1.0f,1.0f };
				m_Vertex[x][z].TexCoord = { x * 0.5f,z * 0.5f };
				m_Vertex[x][z].Tangent = { 1.0f,0.0f,0.0f };
				m_Vertex[x][z].Binormal = { 0.0f,0.0f,-1.0f };
			}

		}

		//�@���x�N�g���Z�o
		for (int x = 1; x <= 19; x++)
		{
			for (int z = 1; z <= 19; z++)
			{
				XMVECTOR vx, vz, vn;

				vx = XMVectorSubtract(XMLoadFloat3(&m_Vertex[x + 1][z].Position), XMLoadFloat3(&m_Vertex[x - 1][z].Position));
				vz = XMVectorSubtract(XMLoadFloat3(&m_Vertex[x][z - 1].Position), XMLoadFloat3(&m_Vertex[x][z + 1].Position));

				vn = XMVector3Cross(vz, vx);//�O��
				vn = XMVector3Normalize(vn);//���K��
				XMStoreFloat3(&m_Vertex[x][z].Normal, vn);
				//XMStoreFloat3(&m_Vertex[x][z].Tangent, vn);
				//XMStoreFloat3(&m_Vertex[x][z].Binormal, vn);
			}
		}




		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//�C���f�b�N�X�o�b�t�@����
	{
		unsigned int index[(22 * 2) * 20 - 2];

		int i = 0;
		for (int x = 0; x < 20; x++)
		{
			for (int z = 0; z < 21; z++)
			{
				index[i] = x * 21 + z;
				i++;

				index[i] = (x + 1) * 21 + z;
				i++;

			}

			if (x == 19)
				break;

			//�k�ރ|���S���p�C���f�b�N�X
			index[i] = (x + 1) * 21 + 20;
			i++;

			index[i] = (x + 1) * 21;
			i++;

		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}


	//�e�N�X�`���[�ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetInstance().GetDevice(),
		"asset/texture/field.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	//�e�N�X�`���[�ǂݍ���
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
	//Renderer::GetInstance().CreatePixelShader(&m_PixelShader[0], "normalMappingPS.cso");
	//Renderer::GetInstance().CreatePixelShader(&m_PixelShader[1], "normalMapSpotPS.cso");

	Renderer::GetInstance().CreateVertexShader(&m_VertexShader, &m_VertexLayout, "depthShadowMappingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&m_PixelShader, "depthShadowMappingPS.cso");

	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 1.0f,1.0f,1.0f };

	m_Type = 0;

}

void MeshField::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	//m_PixelShader[1]->Release();
}

void MeshField::Update()
{

}

void MeshField::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	m_ScaleMtx = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_RotMtx = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_TransMtx = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_WorldMtx = m_ScaleMtx * m_RotMtx * m_TransMtx;

	Renderer::GetInstance().SetWorldMatrix(m_WorldMtx);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Renderer::GetInstance().SetMaterial(material);

	//�e�N�X�`���[�ݒ�
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	//Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(1, 1, &m_NormalTexture);

	//�V���h�E�o�b�t�@���Z�b�g
	ID3D11ShaderResourceView* shadowDepthTexture = Renderer::GetInstance().GetShadowDepthTexture();
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);


	//�v���~�e�B�u�g�|���W�[�ݒ�
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetInstance().GetDeviceContext()->DrawIndexed((22 * 2) * 20 - 2, 0, 0);

}

float MeshField::GetHeight(XMFLOAT3 Position)
{
	int x, z;

	//�u���b�N�ԍ��Z�o
	x = Position.x / 5.0f + 10.0f;
	z = Position.z / -5.0f + 10.0f;

	XMFLOAT3 pos0, pos1, pos2, pos3;

	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	XMVECTOR v12, v1p, c;
	
	v12 = XMVectorSubtract(XMLoadFloat3(&pos2), XMLoadFloat3(&pos1));
	v1p = XMVectorSubtract(XMLoadFloat3(&Position), XMLoadFloat3(&pos1));

	c = XMVector3Cross(v12, v1p);

	float py;
	XMVECTOR n;

	if (XMVectorGetY(c) > 0.0f)
	{
		//����|���S��
		XMVECTOR v10;
		v10 = XMVectorSubtract(XMLoadFloat3(&pos0), XMLoadFloat3(&pos1));
		n = XMVector3Cross(v10, v12);
	}
	else
	{
		//�E���|���S��
		XMVECTOR v13;
		v13 = XMVectorSubtract(XMLoadFloat3(&pos3), XMLoadFloat3(&pos1));
		n = XMVector3Cross(v12, v13);

	}
	
	//�����擾
	py = -((Position.x - pos1.x) * XMVectorGetX(n) + (Position.z - pos1.z) * XMVectorGetZ(n)) / XMVectorGetY(n) + pos1.y;

	return py;

}
