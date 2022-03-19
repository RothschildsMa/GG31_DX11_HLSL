/*=======================================================================================

	 マウス制御クラス[mouse.h]
												author:MA
												date:2021/08/14
 -----------------------------------------------------------------------------------------
 DirectInput API
 =========================================================================================*/
#include "mouse.h"
#include "singleton.h"
#include "manager.h"
#include "renderer.h"
#include "mousePicker.h"
#include "input.h"
#include "scene.h"
#include "camera.h"

BOOL Mouse::StartMouseControl()
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

void Mouse::InitMouse()
{
	ZeroMemory(&m_CurrentMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&m_PrevMouseState, sizeof(DIMOUSESTATE));

	////ウインドウの矩形を算出
	//m_Rect.left = (GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2 + 5;
	//m_Rect.top = (GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2;
	//m_Rect.right = SCREEN_WIDTH + m_Rect.left - 10;
	//m_Rect.bottom = SCREEN_HEIGHT + m_Rect.top;
	
}

void Mouse::UpdateMouse()
{
	if (this->m_InputDevice == nullptr) return;

	// 更新前に最新マウス情報を保存する
	m_PrevMouseState = m_CurrentMouseState;

	// マウスの状態を取得します
	HRESULT	hr = this->m_InputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	if (FAILED(hr))
	{
		this->m_InputDevice->Acquire();
		hr = this->m_InputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	}

	XMFLOAT2 prev = m_MousePosition;
	
	//マウスカーソルをゲームウインドウの矩形に維持する
	//ClipCursor(&m_Rect);

	ShowCursor(FALSE);

	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&m_Point);

	// スクリーン座標にクライアント座標に変換する
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &m_Point);

	// 変換した座標を保存
	m_MousePosition = { (float)m_Point.x,(float)m_Point.y };

	//カーソル位置リセット
	if (Input::GetKeyPress('R')) SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	
}

void Mouse::UninitMouse()
{
	if (this->m_InputDevice != nullptr)
	{
		// 制御を停止
		this->m_InputDevice->Unacquire();
		this->m_InputDevice->Release();
		this->m_InputDevice = nullptr;
	}


}

bool Mouse::OnMousePress(MouseButton button_type)
{
	if (m_PrevMouseState.rgbButtons[button_type] & 0x80 && m_CurrentMouseState.rgbButtons[button_type] & 0x80)
	{
		return true;
	}

	return false;
}

bool Mouse::OnMouseRelease(MouseButton button_type)
{
	if (m_PrevMouseState.rgbButtons[button_type] & 0x80 && !(m_CurrentMouseState.rgbButtons[button_type] & 0x80))
	{
		return true;
	}

	return false;
}

bool Mouse::OnMouseTrigger(MouseButton button_type)
{
	if (!(m_PrevMouseState.rgbButtons[button_type] & 0x80) && m_CurrentMouseState.rgbButtons[button_type] & 0x80)
	{
		return true;
	}

	return false;
}

void Mouse::GetRay(XMVECTOR& Near, XMVECTOR& Far)
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

	XMFLOAT3 rayStart = { m_MousePosition.x,m_MousePosition.y,0.0f };

	Near = XMVector3Unproject
	(
		XMLoadFloat3(&rayStart),
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

	XMFLOAT3 rayEnd = { m_MousePosition.x,m_MousePosition.y,1.0f };

	Far = XMVector3Unproject
	(
		XMLoadFloat3(&rayEnd),
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

}

void Mouse::CreateMouseDevice()
{
	this->m_InputDevice = nullptr;

	// デバイス生成
	HRESULT hr = MyDirectInput::GetInstance().GetInterFace()->CreateDevice(
		GUID_SysMouse,
		&this->m_InputDevice,
		NULL);

	if (FAILED(hr)) return;
	
	// 入力フォーマットの指定
	hr = this->m_InputDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return;
	

	// 協調レベルの設定
	if (!SetUpCooperativeLevel(this->m_InputDevice)) return;
	

	int count = 0;
	// 制御開始
	while (StartMouseControl() == false)
	{
		Sleep(100);
		count++;
		if (count >= 5)
		{
			break;
		}
	}

}

