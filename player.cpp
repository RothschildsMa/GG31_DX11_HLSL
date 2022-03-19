
#include "player.h"
#include "airplane.h"


void Player::Load()
{
	Airplane::Load();
}

void Player::Unload()
{
	Airplane::Unload();
}

void Player::SubHp(float value)
{ 
	m_HpValue -= value;
	m_ShootSE[1]->Play();
}

void Player::RotateToTarget()
{
	XMVECTOR cross = XMVector3Cross(GetForwardVector(), m_TargetDir);
	float sign = XMVectorGetY(cross) < 0.0f ? -1.0f : 1.0f;

	float dot = 0.0f;
	XMStoreFloat(&dot, XMVector3Dot(GetForwardVector(), m_TargetDir));

	float direction_difference = acosf(min(dot, 1.0f));

	float rotation = 0.05f * sign;

	//‚±‚ÌƒtƒŒ[ƒ€‚Å‰ñ“]‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚½—Ê‚ª–Ú“I‚Ì•ûŒü‚ð’´‚¦‚Ä‚µ‚Ü‚¤ê‡
	if (rotation > direction_difference)
	{
		rotation = direction_difference;
	}

	m_Rotation.y += rotation;

	if (XMVector3Equal(GetForwardVector(), m_TargetDir))
	{
		
		m_IsRotating = false;
		
		return;
	}

}

void Player::MoveToTarget()
{
	m_Time += 1 / GetDistanceFromVector(m_NextTarget, m_CurrentTarget) * 0.1f;

	XMStoreFloat3(&m_Position, XMVectorLerp(m_CurrentTarget, m_NextTarget, m_Time));

	if (m_Time >= 1.0f)
	{
		m_Time = 0.0f;
		m_IsMoving = false;
		return;
	}

}

