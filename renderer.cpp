
#include "main.h"
#include "renderer.h"
#include <io.h>


void Renderer::Init()
{
	HRESULT hr = S_OK;

	// �f�o�C�X�A�X���b�v�`�F�[���쐬
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain,
		&m_Device,
		&m_FeatureLevel,
		&m_DeviceContext);


	// �����_�[�^�[�Q�b�g�r���[�쐬
	ID3D11Texture2D* renderTarget = NULL;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
	renderTarget->Release();

	// �f�v�X�X�e���V���o�b�t�@�쐬
	ID3D11Texture2D* depthStencile = NULL;
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// �f�v�X�X�e���V���r���[�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


	// �r���[�|�[�g�ݒ�
	m_Viewport.Width = (FLOAT)SCREEN_WIDTH;
	m_Viewport.Height = (FLOAT)SCREEN_HEIGHT;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &m_Viewport);
	

	// ���X�^���C�U�X�e�[�g�ݒ�
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME  D3D11_FILL_SOLID
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;


	ID3D11RasterizerState *rs;
	m_Device->CreateRasterizerState(&rasterizerDesc, &rs);

	m_DeviceContext->RSSetState(rs);


	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_Device->CreateBlendState(&blendDesc, &blendState);
	m_DeviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);



	// �f�v�X�X�e���V���X�e�[�g�ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//�[�x�L���X�e�[�g

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//�[�x�����X�e�[�g

	m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);




	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MaxAnisotropy = 16;

	ID3D11SamplerState* samplerState = NULL;
	m_Device->CreateSamplerState(&samplerDesc, &samplerState);

	m_DeviceContext->PSSetSamplers(0, 1, &samplerState);



	// �萔�o�b�t�@����
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
	m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ViewBuffer);
	m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ProjectionBuffer);
	m_DeviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
	m_DeviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);


	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);
	m_DeviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	m_DeviceContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);

	bufferDesc.ByteWidth = sizeof(XMFLOAT4);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_CameraBuffer);
	m_DeviceContext->PSSetConstantBuffers(5, 1, &m_CameraBuffer);


	bufferDesc.ByteWidth = sizeof(XMFLOAT4);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ParameterBuffer);
	m_DeviceContext->VSSetConstantBuffers(6, 1, &m_ParameterBuffer);
	m_DeviceContext->PSSetConstantBuffers(6, 1, &m_ParameterBuffer);

	//// ���C�g������
	//LIGHT light{};
	//light.Enable = true;
	//light.Direction = { 1.0f, -1.0f, 1.0f, 0.0f };
	//light.Direction = XMVector4Normalize(light.Direction);
	//light.Ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	//light.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	//SetLight(light);

	// �}�e���A��������
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	SetMaterial(material);

	ShadowInit(swapChainDesc);
	RenderingTexture(swapChainDesc);
}



void Renderer::Uninit()
{
	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();

	m_DeviceContext->ClearState();
	m_RenderTargetView->Release();
	m_ShaderResourceView->Release();
	m_PPRenderTargetView->Release();
	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->Release();

}




void Renderer::Begin()
{
	//�����_�\�^�[�Q�b�g��Z�o�b�t�@�̃Z�b�g
	//�f�t�H���g�̃o�b�N�o�b�t�@�Ɛ[�x�o�b�t�@�֕��A�����Ă���
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, clearColor );
	m_DeviceContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::End()
{
	
	m_SwapChain->Present( 1, 0 );
}

void Renderer::BeginPP()
{
	//�����_�\�^�[�Q�b�g��Z�o�b�t�@�̃Z�b�g
	m_DeviceContext->OMSetRenderTargets(1, &m_PPRenderTargetView, m_DepthStencilView);
	//�킩��₷���悤�Ƀ����_�\�^�[�Q�b�g��΂ŃN���A���Ă���
	float ClearColor[4] = { 0.0f, 0.5f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_PPRenderTargetView, ClearColor);
	//Z�o�b�t�@�̃N���A
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void Renderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );
	else
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateDisable, NULL );

}

void Renderer::SetWorldViewProjection2D()
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX projectionMatrix = XMMatrixIdentity();

	ConstantBuffer cb = {};

	projectionMatrix = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(worldMatrix));
	XMStoreFloat4x4(&cb.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(projectionMatrix));

	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &cb.world, 0, 0);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &cb.view, 0, 0);
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &cb.projection, 0, 0 );

}


void Renderer::SetWorldMatrix(const XMMATRIX& WorldMatrix )
{
	XMMATRIX world = XMMatrixTranspose(WorldMatrix);
	XMFLOAT4X4 mtxW;
	XMStoreFloat4x4(&mtxW, world);
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &mtxW, 0, 0);
}

void Renderer::SetViewMatrix(const XMMATRIX& ViewMatrix )
{
	XMMATRIX view = XMMatrixTranspose(ViewMatrix);
	XMFLOAT4X4 mtxV;
	XMStoreFloat4x4(&mtxV, view);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &mtxV, 0, 0);
}

void Renderer::SetProjectionMatrix(const XMMATRIX& ProjectionMatrix )
{
	XMMATRIX projection = XMMatrixTranspose(ProjectionMatrix);
	XMFLOAT4X4 mtxP;
	XMStoreFloat4x4(&mtxP, projection);
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &mtxP, 0, 0);
}



void Renderer::SetMaterial( MATERIAL Material )
{
	m_DeviceContext->UpdateSubresource( m_MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetLight(LIGHT Light)
{
	if (Light.Enable)
	{
		//�V�F�[�_�[���̓s����ōs���]�u���Ă���
		Light.ViewMatrix = XMMatrixTranspose(Light.ViewMatrix);
		Light.ProjectionMatrix = XMMatrixTranspose(Light.ProjectionMatrix);

		m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
	}

}

void Renderer::SetCameraPosition(XMFLOAT3 CameraPosition)
{
	m_DeviceContext->UpdateSubresource(m_CameraBuffer, 0, NULL, &XMFLOAT4(CameraPosition.x, CameraPosition.y, CameraPosition.z, 1.0f), 0, 0);
}

void Renderer::SetParameter(XMFLOAT4 Parameter)
{
	m_DeviceContext->UpdateSubresource(m_ParameterBuffer, 0, NULL, &Parameter, 0, 0);
}


void Renderer::CreateVertexShader( ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName )
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader( ID3D11PixelShader** PixelShader, const char* FileName )
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}

void Renderer::BeginDepth()
{
	//�V���h�E�o�b�t�@��[�x�o�b�t�@�ɐݒ肵�A���e��1�œh��Ԃ����Ă���
	m_DeviceContext->OMSetRenderTargets(0, NULL, m_ShadowDepthStencilView);
	m_DeviceContext->ClearDepthStencilView(m_ShadowDepthStencilView,D3D11_CLEAR_DEPTH, 1.0f, 0);

}


void Renderer::ShadowInit(DXGI_SWAP_CHAIN_DESC sd)
{
	//�V���h�E�o�b�t�@�쐬
	ID3D11Texture2D* depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = sd.BufferDesc.Width; //�o�b�N�o�b�t�@�̃T�C�Y���󂯂�
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R32_TYPELESS;//32bit�̎��R�Ȍ`���̃f�[�^�Ƃ���
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT; // ���f�v�X���X�e���V���o�b�t�@�Ƃ��č쐬
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	m_Device->CreateTexture2D(&td, NULL, &depthTexture);

	//�f�v�X�X�e���V���^�[�Q�b�g�r���[�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;//�s�N�Z���t�H�[�}�b�g��32BitFLOAT�^
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	m_Device->CreateDepthStencilView(depthTexture, &dsvd,&m_ShadowDepthStencilView);
	//�V�F�[�_�[���\�[�X�r���[�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;//�s�N�Z���t�H�[�}�b�g��32BitFLOAT�^
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	m_Device->CreateShaderResourceView(depthTexture, &SRVDesc,&m_ShadowDepthShaderResourceView);

}

void Renderer::RenderingTexture(DXGI_SWAP_CHAIN_DESC sd)
{
	//�e�N�X�`���[�쐬
	ID3D11Texture2D* ppTexture = NULL;
	D3D11_TEXTURE2D_DESC td; //�e�N�X�`���쐬�p�f�X�N���v�^�\���̕ϐ�
	ZeroMemory(&td, sizeof(td)); 

	td.Width = sd.BufferDesc.Width; //�\����sd��Init�֐��̍ŏ��ō���Ă���B
	td.Height = sd.BufferDesc.Height;//�o�b�N�o�b�t�@�̏�񂪊i�[����Ă���
	td.MipLevels = 1;//�~�b�v�}�b�v�̐� 0�͌��E�܂ō��
	td.ArraySize = 1;

	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //�s�N�Z���t�H�[�}�b�g
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;

	//�g�p�@�̃t���O��ݒ�
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	//�\���̂̐ݒ�ɏ]���ăe�N�X�`���̈���쐬
	m_Device->CreateTexture2D(&td, NULL, &ppTexture);

	//�����_�[�^�[�Q�b�g�r���[�쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(rtvd));
	rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	m_Device->CreateRenderTargetView(ppTexture, &rtvd, &m_PPRenderTargetView);

	//�V�F�[�_�[���\�[�X�r���[�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;
	m_Device->CreateShaderResourceView(ppTexture, &srvd, &m_ShaderResourceView);


}



