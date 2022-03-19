/*=======================================================================================

	 マウス制御クラス[mouse.h]
												author:MA
												date:2021/08/14
 -----------------------------------------------------------------------------------------
  DirectInput API
 =========================================================================================*/
#pragma once
#include "main.h"
#include "singleton.h"
#include "directinput.h"

#include <DirectXMath.h>
using namespace DirectX;

enum MouseButton
{
	Left,
	Right,
	Center
};


class Mouse : public Singleton<Mouse>
{
private:
	LPDIRECTINPUTDEVICE8 m_InputDevice;
	DIMOUSESTATE m_CurrentMouseState;
	DIMOUSESTATE m_PrevMouseState;
	XMFLOAT2 m_MousePosition;
	POINT m_Point;
	RECT m_Rect;

	BOOL StartMouseControl();

public:
	void InitMouse();
	void UpdateMouse();
	void UninitMouse();

	bool OnMousePress(MouseButton button_type);
	bool OnMouseRelease(MouseButton button_type);
	bool OnMouseTrigger(MouseButton button_type);

	void GetRay(XMVECTOR& Near, XMVECTOR& Far);
	
	void CreateMouseDevice();

	const XMFLOAT2& GetMousePosition() { return m_MousePosition; }
	XMFLOAT2 GetMouseVelocity() { return { (float)m_CurrentMouseState.lX,(float)m_CurrentMouseState.lY }; }

	const LPDIRECTINPUTDEVICE8& GetDevice(void) { return m_InputDevice; }

};

XMFLOAT3 transScreenToWorld(
	float sx,
	float sy,
	float z,
	XMMATRIX view,
	XMMATRIX proj
);