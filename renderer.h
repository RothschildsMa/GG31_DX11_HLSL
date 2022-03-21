#pragma once

#include "main.h"
#include "singleton.h"



//struct VERTEX_3D
//{
//    XMFLOAT3 Position;
//	XMFLOAT3 Normal;
//	XMFLOAT4 Diffuse;
//	XMFLOAT2 TexCoord;
//};

struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
	XMFLOAT3 Tangent;
	XMFLOAT3 Binormal;

};

struct ConstantBuffer 
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};


struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	float		Dummy[3];
};


struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	XMVECTOR	Direction;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Ambient;
	XMFLOAT4	Position;
	XMFLOAT4	Angle;

	XMMATRIX	ViewMatrix;
	XMMATRIX	ProjectionMatrix;


};

class Renderer : public Singleton<Renderer>
{
private:

	//メンバ変数、プロパティ、フィールド
	D3D_FEATURE_LEVEL       m_FeatureLevel;

	D3D11_VIEWPORT			m_Viewport;
	
	ID3D11Device*           m_Device = nullptr;
	ID3D11DeviceContext*    m_DeviceContext = nullptr;
	IDXGISwapChain*         m_SwapChain = nullptr;
	ID3D11RenderTargetView* m_RenderTargetView = nullptr;
	ID3D11DepthStencilView* m_DepthStencilView = nullptr;

	ID3D11DepthStencilView*		m_ShadowDepthStencilView = nullptr;
	ID3D11ShaderResourceView*	m_ShadowDepthShaderResourceView = nullptr;

	ID3D11Buffer*			m_WorldBuffer = nullptr;
	ID3D11Buffer*			m_ViewBuffer = nullptr;
	ID3D11Buffer*			m_ProjectionBuffer = nullptr;
	ID3D11Buffer*			m_ConstantBuffer = nullptr;
	ID3D11Buffer*			m_MaterialBuffer = nullptr;
	ID3D11Buffer*			m_LightBuffer = nullptr;
	ID3D11Buffer*			m_CameraBuffer = nullptr;
	ID3D11Buffer*			m_ParameterBuffer = nullptr;

	

	ID3D11DepthStencilState* m_DepthStateEnable = nullptr;
	ID3D11DepthStencilState* m_DepthStateDisable = nullptr;


	void ShadowInit(DXGI_SWAP_CHAIN_DESC sd);

public:
	void Init();
	void Uninit();
	void Begin();
	void End();

	void SetDepthEnable(bool Enable);
	void SetWorldViewProjection2D();
	void SetWorldMatrix(const XMMATRIX& WorldMatrix);
	void SetViewMatrix(const XMMATRIX& ViewMatrix);
	void SetProjectionMatrix(const XMMATRIX& ProjectionMatrix);
	void SetMaterial(MATERIAL Material);
	void SetLight(LIGHT Light);

	void SetCameraPosition(XMFLOAT3 CameraPosition);
	void SetParameter(XMFLOAT4 Parameter);
	
	ID3D11Device* GetDevice( void ){ return m_Device; }
	ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }

	const D3D11_VIEWPORT& GetViewport() { return m_Viewport; }

	void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	ID3D11ShaderResourceView* GetShadowDepthTexture() { return m_ShadowDepthShaderResourceView; }

	void BeginDepth();

	
};

