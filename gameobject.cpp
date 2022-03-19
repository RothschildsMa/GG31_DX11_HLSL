
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "gameobject.h"
#include "camera.h"



void GameObject::AddWorldOffset(const XMFLOAT3& direction, float speed)
{
	XMStoreFloat3(&m_Position, XMVectorAdd(XMLoadFloat3(&m_Position), XMLoadFloat3(&direction) * speed));

}

void GameObject::AddWorldOffsetFromVector(const XMVECTOR& direction, float speed)
{
	XMStoreFloat3(&m_Position, XMVectorAdd(XMLoadFloat3(&m_Position), direction * speed));

}

void GameObject::AddWorldRotation(const XMFLOAT3& axis, float angle)
{
	XMStoreFloat3(&m_Rotation, XMVectorAdd(XMLoadFloat3(&m_Rotation), XMLoadFloat3(&axis) * angle));

}

XMFLOAT2 GameObject::GetScreenPosition(XMFLOAT3 position)
{
	XMMATRIX mtxWorld, mtxView, mtxProj;

	mtxWorld = XMMatrixAffineTransformation
	(
		XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)
	);

	Scene* scene = Manager::GetInstance().GetScene();
	Camera* camera = scene->GetGameObject<Camera>(0);

	mtxView = camera->GetViewMatrix();
	mtxProj = camera->GetProjectionMatrix();

	D3D11_VIEWPORT viewport = Renderer::GetInstance().GetViewport();

	XMVECTOR vec = XMVector3Project
	(
		XMLoadFloat3(&position),
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		viewport.MinDepth,
		viewport.MaxDepth,
		mtxProj,
		mtxView,
		mtxWorld
	);

	XMFLOAT2 pos;

	XMStoreFloat2(&pos, vec);

	return pos;
}

float GameObject::GetDistance(const XMFLOAT3& pos1, const XMFLOAT3& pos2)
{
	float distance = 0.0f;
	XMStoreFloat(&distance, XMVector3Length(XMVectorSubtract(XMLoadFloat3(&pos1), XMLoadFloat3(&pos2))));
	return distance;
}

float GameObject::GetDistanceFromVector(const XMVECTOR & vec1, const XMVECTOR & vec2)
{
	float distance = 0.0f;
	XMStoreFloat(&distance, XMVector3Length(XMVectorSubtract(vec1, vec2)));
	return distance;
}

// キャラクタ姿勢行列算出関数 (任意方向）
XMMATRIX GameObject::LookAtMatrix(const XMFLOAT3& Pos, const XMFLOAT3& Look, const XMFLOAT3& Up)
{
	XMFLOAT3 xVec, yVec, zVec;

	XMVECTOR Z = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&Look), XMLoadFloat3(&Pos)));
	XMVECTOR Y = XMVector3Normalize(XMLoadFloat3(&Up));
	XMVECTOR X = XMVector3Normalize(XMVector3Cross(Y, Z));
	Y = XMVector3Normalize(XMVector3Cross(Z, X));
	
	XMStoreFloat3(&xVec, X);
	XMStoreFloat3(&yVec, Y);
	XMStoreFloat3(&zVec, Z);

	XMFLOAT4X4 mat44;

	mat44._11 = xVec.x; mat44._12 = xVec.y; mat44._13 = xVec.z; mat44._14 = 0;
	mat44._21 = yVec.x; mat44._22 = yVec.y; mat44._23 = yVec.z; mat44._24 = 0;
	mat44._31 = zVec.x; mat44._32 = zVec.y; mat44._33 = zVec.z; mat44._34 = 0;
	mat44._41 = 0.0f;   mat44._42 = 0.0f;   mat44._43 = 0.0f;   mat44._44 = 1.0f;

	return XMLoadFloat4x4(&mat44);
}


// キャラクタ束縛姿勢行列算出関数
XMMATRIX GameObject::LookAtMatrixAxisFix(const XMFLOAT3& Pos, const XMFLOAT3& Look, const XMFLOAT3& Up)
{
	XMFLOAT3 xVec, yVec, zVec;

	XMVECTOR D = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&Look), XMLoadFloat3(&Pos)));
	XMVECTOR Y = XMVector3Normalize(XMLoadFloat3(&Up));
	XMVECTOR X = XMVector3Normalize(XMVector3Cross(Y, D));
	XMVECTOR Z = XMVector3Normalize(XMVector3Cross(X, Y));
	
	XMStoreFloat3(&xVec, X);
	XMStoreFloat3(&yVec, Y);
	XMStoreFloat3(&zVec, Z);

	XMFLOAT4X4 mat44;

	mat44._11 = xVec.x; mat44._12 = xVec.y; mat44._13 = xVec.z; mat44._14 = 0;
	mat44._21 = yVec.x; mat44._22 = yVec.y; mat44._23 = yVec.z; mat44._24 = 0;
	mat44._31 = zVec.x; mat44._32 = zVec.y; mat44._33 = zVec.z; mat44._34 = 0;
	mat44._41 = 0.0f;   mat44._42 = 0.0f;   mat44._43 = 0.0f;   mat44._44 = 1.0f;

	return XMLoadFloat4x4(&mat44);
}

//Z方向ベクトル取得
XMVECTOR GameObject::GetForwardVector()
{
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMFLOAT4X4 mat44;
	XMStoreFloat4x4(&mat44, rot);

	XMFLOAT3 z = { mat44._31, mat44._32, mat44._33 };

	return XMLoadFloat3(&z);
}

//Y方向ベクトル取得
XMVECTOR GameObject::GetUpVector()
{
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMFLOAT4X4 mat44;

	XMStoreFloat4x4(&mat44, rot);

	XMFLOAT3 y = { mat44._21,mat44._22,mat44._23 };

	return XMLoadFloat3(&y);
}

//X方向ベクトル取得
XMVECTOR GameObject::GetRightVector()
{

	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMFLOAT4X4 mat44;

	XMStoreFloat4x4(&mat44, rot);

	XMFLOAT3 x = { mat44._11,mat44._12,mat44._13 };

	return XMLoadFloat3(&x);
}
