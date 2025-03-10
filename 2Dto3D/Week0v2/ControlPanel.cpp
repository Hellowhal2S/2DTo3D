#include "ControlPanel.h"

ControlPanel::ControlPanel()
{
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::Draw(UWorld* world, double elapsedTime)
{
	UCameraComponent* Camera = static_cast<UCameraComponent*>(world->GetCamera());

	ImGui::Begin("Jungle Control Panel");
	double fps = 1000.0 / elapsedTime;
	ImGui::Text("FPS %.2f (%.2fms)", fps, elapsedTime);

	ImGui::Separator();

    ImGui::Text("Number of Spawned: %d", static_cast<int>(world->GetObjectArr().size()));

	ImGui::Separator();

	static int32 primitiveType = 0;
	const char* primitives[] = { "Sphere", "Cube", "Triangle" };
	ImGui::Combo("Primitive", &primitiveType, primitives, IM_ARRAYSIZE(primitives));
	ImGui::SameLine();
	if (ImGui::Button("Spawn"))
	{
		world->SpawnObject(static_cast<OBJECTS>(primitiveType));
	}

	ImGui::Separator();

	static char sceneName[64] = "Default";
	ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));

	if (ImGui::Button("New scene")) {
		world->NewScene();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save scene")) {
		FString SceneName(sceneName);
		SceneData SaveData = world->SaveData();
		FSceneManager::SaveSceneToFile(SceneName, SaveData);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load scene")) {
		FString SceneName(sceneName);
		FString LoadJsonData = FSceneManager::LoadSceneFromFile(SceneName);
		SceneData LoadData = FSceneManager::ParseSceneData(LoadJsonData);
		world->LoadData(LoadData);
	}

	ImGui::Separator();

	ImGui::Text("Orthogonal");
	ImGui::SliderFloat("FOV", &Camera->GetFov(), 30.0f, 120.0f);

	float cameraLocation[3] = { Camera->GetLocation().x, Camera->GetLocation().y, Camera->GetLocation().z };
	ImGui::InputFloat3("Camera Location", cameraLocation);

	float cameraRotation[3] = { Camera->GetRotation().x, Camera->GetRotation().y, Camera->GetRotation().z };
	ImGui::InputFloat3("Camera Rotation", cameraRotation);

	Camera->SetLocation(FVector(cameraLocation[0], cameraLocation[1], cameraLocation[2]));
	Camera->SetRotation(FVector(cameraRotation[0], cameraRotation[1], cameraRotation[2]));

	ImGui::End();
}
