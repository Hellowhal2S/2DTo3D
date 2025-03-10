#include "Save.h"
#include "ImguiManager.h"
#include "Model.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "json.hpp"
#include "SceneComponent.h"

USaveManager::USaveManager() {}
USaveManager::~USaveManager() {}

void USaveManager::NewScene(std::vector<UObject*>& ObjectList)
{
    // 기존 씬 초기화
    for (auto& Object : ObjectList)
    {
        delete Object;
    }
    ObjectList.clear();
}

void USaveManager::SaveScene(const std::string& FilePath, const std::vector<UObject*>& ObjectList)
{
    json::JSON obj;


    obj["NextUUID"] = ObjectList.size();
    obj["Version"] = 1;

    json::JSON primitives = json::Object();
    for (size_t i = 0; i < ObjectList.size(); ++i)
    {
        USceneComponent* SceneComp = dynamic_cast<USceneComponent*>(ObjectList[i]);
        if (!SceneComp) continue;

        json::JSON primitive;
        primitive["Location"] = json::Array(SceneComp->RelativeLocation.x, SceneComp->RelativeLocation.y, SceneComp->RelativeLocation.z);
        primitive["Rotation"] = json::Array(SceneComp->RelativeRotation.x, SceneComp->RelativeRotation.y, SceneComp->RelativeRotation.z);
        primitive["Scale"] = json::Array(SceneComp->RelativeScale3D.x, SceneComp->RelativeScale3D.y, SceneComp->RelativeScale3D.z);

        if (dynamic_cast<UCubeComp*>(ObjectList[i]))
        {
            primitive["Type"] = "Cube";
        }
        else if (dynamic_cast<USphereComp*>(ObjectList[i]))
        {
            primitive["Type"] = "Sphere";
        }

        primitives[std::to_string(i)] = primitive;
    }

    obj["Primitives"] = primitives;

    std::ofstream file(FilePath);
    if (file.is_open())
    {
        file << obj.dump(4);
        file.close();
    }
    else
    {
        std::cerr << "Failed to save scene to " << FilePath << std::endl;
    }
}

void USaveManager::LoadScene(const std::string& FilePath, std::vector<UObject*>& ObjectList)
{
    std::ifstream file(FilePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to load scene from " << FilePath << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonString = buffer.str();
    file.close();

    json::JSON obj = json::JSON::Load(jsonString);

    // 기존 씬 초기화
    NewScene(ObjectList);

    json::JSON primitives = obj["Primitives"];
    for (const auto& key : primitives.ObjectRange())
    {
        json::JSON primitive = key.second;

        FVector Location(primitive["Location"][0].ToFloat(), primitive["Location"][1].ToFloat(), primitive["Location"][2].ToFloat());
        FVector Rotation(primitive["Rotation"][0].ToFloat(), primitive["Rotation"][1].ToFloat(), primitive["Rotation"][2].ToFloat());
        FVector Scale(primitive["Scale"][0].ToFloat(), primitive["Scale"][1].ToFloat(), primitive["Scale"][2].ToFloat());

        UObject* NewObject = nullptr;
        std::string Type = primitive["Type"].ToString();

        if (Type == "Cube")
        {
            NewObject = new UCubeComp();
        }
        else if (Type == "Sphere")
        {
            NewObject = new USphereComp();
        }

        if (NewObject)
        {
            USceneComponent* SceneComp = dynamic_cast<USceneComponent*>(NewObject);
            if (SceneComp)
            {
                SceneComp->RelativeLocation = Location;
                SceneComp->RelativeRotation = Rotation;
                SceneComp->RelativeScale3D = Scale;
            }
            ObjectList.push_back(NewObject);
        }
    }
}

void USaveManager::RenderSceneManagerUI(std::vector<UObject*>& ObjectList)
{
    char buffer[256]; 
    strncpy_s(buffer, filename.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0'; 

    if (ImGui::InputText("Scene Name", buffer, IM_ARRAYSIZE(buffer))) {
        filename = buffer;
    }
    if (ImGui::Button("New Scene"))
    {
        NewScene(ObjectList);
    }
    if (ImGui::Button("Save Scene"))
    {
        SaveScene(filename + ".scene", ObjectList);
    }

    if (ImGui::Button("Load Scene"))
    {
        LoadScene(filename + ".scene", ObjectList);
    }   
}