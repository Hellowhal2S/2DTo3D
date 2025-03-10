#include "ImGuiManager.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#include "DeviceManager.h"
#include "Define.h"
#include "UWorld.h"


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
	ImGui::Begin("Jungle Control Panel");
	ImGui::Text("Hello Jungle World!");
	//ImGui::Text("FPS: %.1f ms", ImGui::GetIO().Framerate());
	
	ImGui::Separator();

	// Primitive 설정
	static int selected_primitive = 0;
	const char* primitive_items[] = { "Sphere", "Cube" };
	ImGui::Combo("##Primitive", &selected_primitive, primitive_items, IM_ARRAYSIZE(primitive_items));
	ImGui::SameLine();
	ImGui::Text("Primitive", nullptr);
    if (ImGui::Button("Spawn")) {
        switch (selected_primitive){
            case OBJ_SPHERE:
                USphereComp::NewSphere(UWorld::myWorld);
                break;
            case OBJ_CUBE:
                UCubeComp::NewCube(UWorld::myWorld);
                break;
        }
    }
    /*
    // Speed 설정
    static int speed = 1;
    ImGui::InputInt("Speed", &speed);
    ImGui::SameLine();
    ImGui::Text("Number of spans");

    ImGui::Separator();

    // Scene 선택
    static char scene_name[32] = "Default";
    ImGui::InputText("Scene Name", scene_name, IM_ARRAYSIZE(scene_name));

    // Scene 조작 버튼
    if (ImGui::Button("New Scene")) {
        // 새로운 씬 로직 추가
    }
    ImGui::SameLine();
    if (ImGui::Button("Save Scene")) {
        // 씬 저장 로직 추가
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Scene")) {
        // 씬 로드 로직 추가
    }

    ImGui::Separator();

    // 카메라 정보 (Orthogonal, FOV)
    ImGui::Text("[Orthogonal]");
    ImGui::SameLine();
    ImGui::Text("50.000 FOV");

    // 카메라 위치 및 회전 정보
    static float camera_pos[3] = { 2.768f, -2.323f, -6.496f };
    static float camera_rot[3] = { 0.000f, 0.000f, 0.000f };

    ImGui::Text("Camera Location");
    ImGui::InputFloat3("##CamLoc", camera_pos, "%.3f");

    ImGui::Text("Camera Rotation");
    ImGui::InputFloat3("##CamRot", camera_rot, "%.3f");*/
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