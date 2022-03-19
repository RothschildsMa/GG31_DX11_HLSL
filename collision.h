#pragma once

#include"main.h"
#include <DirectXMath.h>
using namespace DirectX;

#define IKD_EPSIRON 0.00001f

// 線分構造体
struct Line
{
	XMFLOAT2 Start;
	XMFLOAT2 End;
};

// 円構造体
struct Circle
{
	XMFLOAT2 Position;
	float radius;
};

struct Line3D
{
	XMFLOAT3 Start;
	XMFLOAT3 End;
};

struct Tama
{
	XMFLOAT3 Position;
	float Radius;
};

struct Rect
{
	XMFLOAT2 Position;
	XMFLOAT2 Size;
};

class Collision
{

public:

	static bool PointAndRect(const XMFLOAT2& point, const Rect& rect);
	static bool PointAndCircle(const XMFLOAT2& point, const Circle& circle);
};


//頂点間の長さを求める
float GetVertexLength(const D3DXVECTOR2& position1, const D3DXVECTOR2& position2);


//線分と円の衝突判定
bool Collision_LineAndCircle(Line line, Circle circle);
