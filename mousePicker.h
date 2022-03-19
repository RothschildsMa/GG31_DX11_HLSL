/*=======================================================================================

	 ゲームマウスカーソル制御クラス[mousePicker.h]
												author:MA
												date:2021/08/14
 -----------------------------------------------------------------------------------------

 =========================================================================================*/
#pragma once

#include "singleton.h"
#include "mouse.h"
#include "sprite2D.h"

class MousePicker : public Singleton<MousePicker>
{
private:

	XMFLOAT2 m_Position;
	int m_TextureId;
	int m_Frame = 0;
	int m_Count = 0;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

};

