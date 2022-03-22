/*=======================================================================================

	 �}�E�X����N���X[mouse.h]
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
	// �f�o�C�X����������ĂȂ�
	if (this->m_InputDevice == nullptr)
	{
		return false;
	}
	// ����J�n
	if (FAILED(this->m_InputDevice->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	this->m_InputDevice->GetCapabilities(&cap);
	// �|�[�����O����
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// �|�[�����O�J�n
		/*
			Poll��Acquire�̑O�ɍs���Ƃ���Ă������A
			Acquire�̑O�Ŏ��s����Ǝ��s�����̂�
			��Ŏ��s����悤�ɂ���
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

	////�E�C���h�E�̋�`���Z�o
	//m_Rect.left = (GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2 + 5;
	//m_Rect.top = (GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2;
	//m_Rect.right = SCREEN_WIDTH + m_Rect.left - 10;
	//m_Rect.bottom = SCREEN_HEIGHT + m_Rect.top;
	
}

void Mouse::UpdateMouse()
{
	if (this->m_InputDevice == nullptr) return;

	// �X�V�O�ɍŐV�}�E�X����ۑ�����
	m_PrevMouseState = m_CurrentMouseState;

	// �}�E�X�̏�Ԃ��擾���܂�
	HRESULT	hr = this->m_InputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	if (FAILED(hr))
	{
		this->m_InputDevice->Acquire();
		hr = this->m_InputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	}

	XMFLOAT2 prev = m_MousePosition;
	
	//�}�E�X�J�[�\�����Q�[���E�C���h�E�̋�`�Ɉێ�����
	//ClipCursor(&m_Rect);

	ShowCursor(FALSE);

	// �}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&m_Point);

	// �X�N���[�����W�ɃN���C�A���g���W�ɕϊ�����
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &m_Point);

	// �ϊ��������W��ۑ�
	m_MousePosition = { (float)m_Point.x,(float)m_Point.y };

	//�J�[�\���ʒu���Z�b�g
	if (Input::GetKeyPress('R')) SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
	
}

void Mouse::UninitMouse()
{
	if (this->m_InputDevice != nullptr)
	{
		// ������~
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
	TPSCamera* camera = scene->GetGameObject<TPSCamera>(0);

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

	// �f�o�C�X����
	HRESULT hr = MyDirectInput::GetInstance().GetInterFace()->CreateDevice(
		GUID_SysMouse,
		&this->m_InputDevice,
		NULL);

	if (FAILED(hr)) return;
	
	// ���̓t�H�[�}�b�g�̎w��
	hr = this->m_InputDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return;
	

	// �������x���̐ݒ�
	if (!SetUpCooperativeLevel(this->m_InputDevice)) return;
	

	int count = 0;
	// ����J�n
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

