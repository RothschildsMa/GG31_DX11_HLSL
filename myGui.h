#pragma once

#include "main.h"
#include "renderer.h"
#include "singleton.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

class MyGui : public Singleton<MyGui>
{
private:
	static float m_Parameter;

public:
	MyGui(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static float GetParam() { return m_Parameter; }
};
