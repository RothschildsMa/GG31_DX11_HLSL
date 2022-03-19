

#pragma once
#include "singleton.h"
#include "texture.h"

#include <DirectXMath.h>
using namespace DirectX;

struct Transform2D
{
	XMFLOAT2 Position;
	XMFLOAT2 Rotation;
	XMFLOAT2 Scale;
};

class Sprite2D : public Singleton<Sprite2D>
{
private:
	ID3D11Buffer*		 m_VertexBuffer;
	ID3D11Buffer*		 m_IndexBuffer;

	ID3D11VertexShader*			 m_VertexShader;
	ID3D11PixelShader*			 m_PixelShader;
	ID3D11InputLayout*			 m_VertexLayout;

	XMFLOAT4 m_Color;

public:
	void Init(void);
	void Uninit(void);
	
	//2D描画
	//
	//引数：
	//			textureId	...テクスチャー番号
	//			dx			...描画座標 X
	//			xy			...描画座標 Y
	//			dw			...描画サイズの幅 
	//			dh			...描画サイズの高さ
	//			tcx			...テクスチャー切り取り座標 X
	//			tcy			...テクスチャー切り取り座標 Y
	//			tcw			...テクスチャー切り取り幅
	//			tch			...テクスチャー切り取り高さ
	//			cx			...回転中心点座標 X
	//			cy			...回転中心点座標 Y
	//			angle		...回転角度
	//
	void SpriteDraw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, float cx = 0.0f, float cy = 0.0f, float angle = 0.0f);


	void SetColor(const XMFLOAT4& color) { m_Color = color; }

};