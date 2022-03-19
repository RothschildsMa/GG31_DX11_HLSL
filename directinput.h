/*===========================================================================================

	DirectInput APIクラス[directinput.h]
													author : Ma 
													date : 2021/08/13
---------------------------------------------------------------------------------------------

=============================================================================================*/
#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "main.h"
#include "singleton.h"


class MyDirectInput : public Singleton<MyDirectInput>
{
private:
	LPDIRECTINPUT8 m_InputInterface = nullptr;		

	// 入力インターフェースの作成
	void CreateInputInterface();

public:
	void CreateDeviceInstance();
	void DestoryDeviceInstance();

	//入力デバイスの初期化
	void InitInput();

	//入力デバイスの更新処理
	void UpdateInput();

	//入力デバイスの終了処理
	void ReleaseInput();

	const LPDIRECTINPUT8& GetInterFace() { return m_InputInterface; }

};

BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device);