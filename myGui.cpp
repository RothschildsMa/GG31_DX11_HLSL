


#include "myGui.h"
#include<string>

float MyGui::m_Parameter = 0.0f;

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
	

	

	//ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);

	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
	//		if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
	//		if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//}
	//

	//ImGui::Text("Hello, world %d", 123);
	//ImGui::Button(" + ");
	//ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	//

	//ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
