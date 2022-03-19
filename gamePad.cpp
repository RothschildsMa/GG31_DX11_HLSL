/*=======================================================================================

	  コントローラー制御クラス[gamePad.cpp]
												author:MA
												date:2021/08/13
 -----------------------------------------------------------------------------------------
 ※DirectInput API のみ対応する
 =========================================================================================*/
#include"gamePad.h"
#include"singleton.h"

BOOL SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device);
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

BOOL SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device)
{
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	diprg.diph.dwObj = DIJOFS_X;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	diprg.diph.dwObj = DIJOFS_Z;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	diprg.diph.dwObj = DIJOFS_RZ;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	return true;
}

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
	LPDIRECTINPUTDEVICE8 device = nullptr;

	// 既に発見しているなら終了
	if (parameter->FindCount >= 1)
	{
		return DIENUM_STOP;
	}

	// デバイス生成
	HRESULT hr = MyDirectInput::GetInstance().GetInterFace()->CreateDevice(
		lpddi->guidInstance,
		parameter->GamePadDevice,
		NULL);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// 入力フォーマットの指定
	device = *parameter->GamePadDevice;
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// プロパティの設定
	if (SetUpGamePadProperty(device) == false)
	{
		return DIENUM_STOP;
	}

	// 協調レベルの設定
	if (SetUpCooperativeLevel(device) == false)
	{
		return DIENUM_STOP;
	}

	// 発見数をカウント
	parameter->FindCount++;

	return DIENUM_CONTINUE;
}

BOOL GamePad::StartGamePadControl()
{
	// デバイスが生成されてない
	if (this->m_InputDevice == nullptr)
	{
		return false;
	}

	// 制御開始
	if (FAILED(this->m_InputDevice->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	this->m_InputDevice->GetCapabilities(&cap);
	// ポーリング判定
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始
		/*
			PollはAcquireの前に行うとされていたが、
			Acquireの前で実行すると失敗したので
			後で実行するようにした
		*/
		if (FAILED(this->m_InputDevice->Poll()))
		{
			return false;
		}
	}

	return true;
}

void GamePad::InitController(void)
{
	// 入力情報の初期化
	for (int i = 0; i < ButtonKindMax; i++)
	{
		m_JoyStates[i] = JoyStateNone;
	}
}

void GamePad::UpdateController(void)
{
	if (this->m_InputDevice == nullptr) return;

	DIJOYSTATE pad_data;

	// デバイス取得
	HRESULT hr = this->m_InputDevice->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
	if (FAILED(hr))
	{
		// 再度制御開始
		if (FAILED(this->m_InputDevice->Acquire()))
		{
			for (int i = 0; i < ButtonKindMax; i++)
			{
				m_JoyStates[i] = JoyStateNone;
			}
			this->m_InputDevice->Poll();
		}
		return;
	}

	bool is_push[ButtonKindMax];

	//==================================================================
	// 左スティックの設定
	//==================================================================
	//アナログスティックの左右の状態が -1000～+1000

	int unresponsive_range = 200;

	if (pad_data.lX < -unresponsive_range)
	{
		is_push[LStickLeft] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{
		is_push[LStickRight] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{
		is_push[LStickUP] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{
		is_push[LStickDown] = true;
	}

	//==================================================================
	// 右スティックの設定
	//==================================================================
	if (pad_data.lZ < -unresponsive_range)
	{
		is_push[RStickLeft] = true;
	}
	else if (pad_data.lZ > unresponsive_range)
	{
		is_push[RStickRight] = true;
	}

	if (pad_data.lRz < -unresponsive_range)
	{
		is_push[RStickUP] = true;
	}
	else if (pad_data.lRz > unresponsive_range)
	{
		is_push[RStickDown] = true;
	}


	//==================================================================
	// 十字キーの設定
	//==================================================================
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		float rad = D3DXToRadian((pad_data.rgdwPOV[0] / 100.0f));

		float x = sinf(rad);
		float y = cosf(rad);

		if (x < -0.01f)
		{
			is_push[LeftButton] = true;
		}
		else if (x > 0.01f)
		{
			is_push[RightButton] = true;
		}

		if (y > 0.01f)
		{
			is_push[UpButton] = true;
		}
		else if (y < -0.01f)
		{
			is_push[DownButton] = true;
		}
	}



	//==================================================================
	// ボタンの設定
	//==================================================================
	for (int i = 0; i < 32; i++)
	{
		if (!(pad_data.rgbButtons[i] & 0x80))
		{
			continue;
		}

		switch (i)
		{
		case 0:
			is_push[ButtonY] = true;
			break;
		case 1:
			is_push[ButtonB] = true;
			break;
		case 2:
			is_push[ButtonA] = true;
			break;
		case 3:
			is_push[ButtonX] = true;
			break;
		case 4:
			is_push[ButtonLB] = true;
			break;
		case 5:
			is_push[ButtonRB] = true;
			break;
		case 6:
			is_push[ButtonLT] = true;
			break;
		case 7:
			is_push[ButtonRT] = true;
			break;
		case 8:
			is_push[Button_BACK] = true;
			break;
		case 9:
			is_push[Button_START] = true;
			break;
		}
	}


	// 入力情報からボタンの状態を更新する
	for (int i = 0; i < ButtonKindMax; i++)
	{
		if (is_push[i] == true)
		{
			if (m_JoyStates[i] == JoyStateNone)
			{
				m_JoyStates[i] = JoyStateDown;
			}
			else
			{
				m_JoyStates[i] = JoyStatePush;
			}
		}
		else
		{
			if (m_JoyStates[i] == JoyStatePush)
			{
				m_JoyStates[i] = JoyStateUp;
			}
			else
			{
				m_JoyStates[i] = JoyStateNone;
			}
		}
	}
}

void GamePad::UninitController()
{
	if (this->m_InputDevice != nullptr)
	{
		// 制御を停止
		this->m_InputDevice->Unacquire();
		this->m_InputDevice->Release();
		this->m_InputDevice = nullptr;
	}

}

bool GamePad::JoystickPress(ButtonKind button)
{
	if (m_JoyStates[button] == JoyStatePush)
	{
		return true;
	}

	return false;
}

bool GamePad::JoystickRelease(ButtonKind button)
{
	if (m_JoyStates[button] == JoyStateUp)
	{
		return true;
	}

	return false;
}

bool GamePad::JoystickTrigger(ButtonKind button)
{
	if (m_JoyStates[button] == JoyStateDown)
	{
		return true;
	}

	return false;
}

void GamePad::CreateGamePadDevice()
{
	DeviceEnumParameter parameter;

	parameter.FindCount = 0;
	parameter.GamePadDevice = &this->m_InputDevice;

	// GAMEPADを調べる
	MyDirectInput::GetInstance().GetInterFace()->EnumDevices(
		DI8DEVTYPE_GAMEPAD,			// 検索するデバイスの種類
		DeviceFindCallBack,			// 発見時に実行する関数
		&parameter,					// 関数に渡す値
		DIEDFL_ATTACHEDONLY			// 検索方法
	);

	// JOYSTICKを調べる
	MyDirectInput::GetInstance().GetInterFace()->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	// どちらも見つけることが出来なかったら失敗
	if (parameter.FindCount == 0) return;

	int count = 0;
	// 制御開始
	while (StartGamePadControl() == false)
	{
		Sleep(100);
		count++;
		if (count >= 5)
		{
			break;
		}
	}
}


