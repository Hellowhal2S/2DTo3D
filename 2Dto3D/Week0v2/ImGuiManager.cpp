#include "ImGuiManager.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#include "DeviceManager.h"

void ImGuiManager::InitImGui(URenderer& renderer, HWND hwnd) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hwnd);
	ImGui_ImplDX11_Init(DeviceManager::Device, DeviceManager::DeviceContext);
}

void ImGuiManager::NewImGuiFrame() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::DrawRenderData() {
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::SetupUI() {
	ImGui::Begin("Jungle Property Window");
	ImGui::Text("Hello Jungle World!");
	ImGui::End();
}

void ImGuiManager::RenderImGui() {
	ImGuiManager::NewImGuiFrame();
	ImGuiManager::SetupUI();
	ImGuiManager::DrawRenderData();
}

void ImGuiManager::ReleaseImGUi() {
	// 여기에서 ImGui 소멸
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}