#include "ImGuiManager.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#include "DeviceManager.h"
#include "Define.h"
#include "UWorld.h"
#include "PickingSystem.h"

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

    ImGui::Begin("Raycast Panel");

    FVector hitPosition = UWorld::myWorld->pickingSystem->rayOrigin;
    float hitPos[3] = { hitPosition.x, hitPosition.y, hitPosition.z };

    FVector rayVec = UWorld::myWorld->pickingSystem->rayDir;
    float rayDir[3] = { rayVec.x, rayVec.y, rayVec.z };
    
    FVector Trans;
    FVector Rot;
    FVector Scale;
    float transArray[3];
    float RotArray[3];
    float ScaleArray[3];

    if (UWorld::pickingObject != nullptr) {
        Trans = UWorld::pickingObject->RelativeLocation;
        transArray[0] = Trans.x;
        transArray[1] = Trans.y;
        transArray[2] = Trans.z;

        Rot = UWorld::pickingObject->RelativeRotation;
        RotArray[0] = Rot.x;
        RotArray[1] = Rot.y;
        RotArray[2] = Rot.z;

        Scale = UWorld::pickingObject->RelativeScale3D;
        ScaleArray[0] = Scale.x;
        ScaleArray[1] = Scale.y;
        ScaleArray[2] = Scale.z;
    }

    ImGui::Text("Hit Pos");
    ImGui::SameLine();
    ImGui::InputFloat3("##hitPos", hitPos, "%.3f");
    ImGui::Text("Ray Dir;");
    ImGui::SameLine();
    ImGui::InputFloat3("##RayDir", rayDir, "%.3f");
    ImGui::End();

    ImGui::Begin("Jungle Property Window");
    
    if (ImGui::InputFloat3("##Translation", transArray, "%.3f")) {
        if (UWorld::pickingObject != nullptr)
        {
            UWorld::pickingObject->RelativeLocation = FVector(transArray[0], transArray[1], transArray[2]);
        }
    }
    ImGui::SameLine();
    ImGui::Text("Translation;");
    
    if (ImGui::InputFloat3("##Rotation", RotArray, "%.3f")) {
        if (UWorld::pickingObject != nullptr)
        {
            UWorld::pickingObject->RelativeRotation = FVector(RotArray[0], RotArray[1], RotArray[2]);
        }
    }
    ImGui::SameLine();
    ImGui::Text("Rotation;");

    if (ImGui::InputFloat3("##Scale", ScaleArray, "%.3f")) {
        if (UWorld::pickingObject != nullptr)
        {
            UWorld::pickingObject->RelativeScale3D = FVector(ScaleArray[0], ScaleArray[1], ScaleArray[2]);
        }
    }
    ImGui::SameLine();
    ImGui::Text("Scale;");

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