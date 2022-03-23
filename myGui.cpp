


#include "myGui.h"
#include<string>

float MyGui::m_Parameter = 0.0f;
bool MyGui::m_Drawflag = false;
bool MyGui::ShadowDepthView = false;
bool MyGui::RenderTargetView = false;
bool MyGui::HorrorTexture = false;

static char buf[256] = "";

static bool my_tool_active = true;

void MyGui::Init()
{
	//セットアップ ImGui
	IMGUI_CHECKVERSION();
	
	ImGui::CreateContext();
	//ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetInstance().GetDevice(), Renderer::GetInstance().GetDeviceContext());
	
	


	ImGui::StyleColorsClassic();

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(1.0f, 0.4f, 1.0f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(1.0f, 0.4f, 1.0f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.0f, 10.0f, 10.0f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
	m_Parameter = 0.0f;
	m_Drawflag = false;

}

void MyGui::Uninit()
{
}

void MyGui::Update()
{
	//フレームスタット
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MyGui::Draw()
{
	static int counter = 0;
	
	ImGui::Begin("Debug");
	
	ImGui::Checkbox("ShadowDepthView", &ShadowDepthView);
	ImGui::Checkbox("RenderTargetView", &RenderTargetView);
	ImGui::Checkbox("HorrorTexture", &HorrorTexture);

	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
