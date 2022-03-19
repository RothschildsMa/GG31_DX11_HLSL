/*===========================================================================================

	DirectInput APIクラス[directinput.cpp]
													author : Ma
													date : 2021/08/13
---------------------------------------------------------------------------------------------

=============================================================================================*/
#include <Windows.h>
#include <stdio.h>
#include "directinput.h"
#include "gamePad.h"
#include "mouse.h"

BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device)
{
	//協調モードの設定
	if (FAILED(device->SetCooperativeLevel(
		FindWindow(CLASS_NAME, WINDOW_NAME),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	))
	{
		return false;
	}

	return true;
}


void MyDirectInput::CreateDeviceInstance()
{
	GamePad::CreateInstance();
	Mouse::CreateInstance();
}

void MyDirectInput::DestoryDeviceInstance()
{
	GamePad::DestroyInstance();
	Mouse::DestroyInstance();
}

void MyDirectInput::InitInput()
{
	this->CreateInputInterface();

	GamePad::GetInstance().CreateGamePadDevice();
	GamePad::GetInstance().InitController();

	Mouse::GetInstance().CreateMouseDevice();
	Mouse::GetInstance().InitMouse();

}

void MyDirectInput::UpdateInput()
{
	GamePad::GetInstance().UpdateController();
	Mouse::GetInstance().UpdateMouse();

}


void MyDirectInput::ReleaseInput()
{
	// デバイスの解放
	GamePad::GetInstance().UninitController();
	Mouse::GetInstance().UninitMouse();

	// インターフェースの解放
	if (m_InputInterface != nullptr)
	{
		m_InputInterface->Release();
		m_InputInterface = nullptr;
	}
}

void MyDirectInput::CreateInputInterface()
{
	// インターフェース作成
	HRESULT ret = DirectInput8Create(
		GetModuleHandle(nullptr),	// インスタンスハンドル
		DIRECTINPUT_VERSION,		// DirectInputのバージョン
		IID_IDirectInput8,			// 使用する機能
		(void**)&m_InputInterface,	// 作成されたインターフェース代入用
		NULL						// NULL固定
	);

	if (FAILED(ret)) return;
}



