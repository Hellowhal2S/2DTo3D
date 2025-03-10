#include "SceneManager.h"

#include <fstream>
#include "json.hpp"
#include "ObjectFactory.h"

#include "Object.h"
#include "SphereComponent.h"
#include "CubeComponent.h"
#include "ArrowComponent.h"

using json = nlohmann::json;

SceneData FSceneManager::ParseSceneData(const std::string& jsonStr)
{
    SceneData sceneData;

    try {
        json j = json::parse(jsonStr);

        // ������ NextUUID �б�
        sceneData.Version = j["Version"].get<int>();
        sceneData.NextUUID = j["NextUUID"].get<int>();

        // Primitives ó�� (C++14 ��Ÿ��)
        auto primitives = j["Primitives"];
        for (auto it = primitives.begin(); it != primitives.end(); ++it) {
            int id = std::stoi(it.key());  // Key�� ���ڿ�, ���ڷ� ��ȯ
            const json& value = it.value();
            UObject* primitive = nullptr;
            if (value.contains("Type"))
            {
                if (value["Type"].get<FString>() == "Sphere")
                {
                    primitive = FObjectFactory::ConstructObject<USphereComp>();
                }
                else if (value["Type"].get<FString>() == "Cube")
                {
                    primitive = FObjectFactory::ConstructObject<UCubeComp>();
                }
                else if (value["Type"].get<FString>() == "Arrow")
                {
                    primitive = FObjectFactory::ConstructObject<UArrowComp>();
                }
            }

            if (value.contains("Location")) primitive->SetLocation(FVector(value["Location"].get<TArray<float>>()[0],
                value["Location"].get<TArray<float>>()[1],
                value["Location"].get<TArray<float>>()[2]));
            if (value.contains("Rotation")) primitive->SetRotation(FVector(value["Rotation"].get<TArray<float>>()[0],
                value["Rotation"].get<TArray<float>>()[1],
                value["Rotation"].get<TArray<float>>()[2]));
            if (value.contains("Scale")) primitive->SetScale(FVector(value["Scale"].get<TArray<float>>()[0],
                value["Scale"].get<TArray<float>>()[1],
                value["Scale"].get<TArray<float>>()[2]));
            if (value.contains("Type")) static_cast<UPrimitiveComponent*>(primitive)->SetType(value["Type"].get<FString>());

            sceneData.Primitives[id] = primitive;
        }
    }
    catch (const std::exception& e) {
        FString errorMessage = "Error parsing JSON: ";
        errorMessage += e.what();

        MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
    }

    return sceneData;
}

FString FSceneManager::LoadSceneFromFile(const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile) {
        UE_LOG(LogLevel::Error, "Failed to open file for reading: %s", filename.c_str());
        return FString();
    }

    json j;
    try {
        inFile >> j; // JSON ���� �б�
    }
    catch (const std::exception& e) {
        UE_LOG(LogLevel::Error, "Error parsing JSON: ");
        return FString();
    }

    inFile.close();

    return j.dump(4);
}

std::string FSceneManager::SerializeSceneData(const SceneData& sceneData)
{
    json j;

    // Version�� NextUUID ����
    j["Version"] = sceneData.Version;
    j["NextUUID"] = sceneData.NextUUID;

    // Primitives ó�� (C++14 ��Ÿ��)
    for (auto it = sceneData.Primitives.begin(); it != sceneData.Primitives.end(); ++it) {
        int id = it->first;
        UObject* primitive = it->second;
        TArray<float> Location = { primitive->GetLocation().x,primitive->GetLocation().y,primitive->GetLocation().z };
        TArray<float> Rotation = { primitive->GetRotation().x,primitive->GetRotation().y,primitive->GetRotation().z };
        TArray<float> Scale = { primitive->GetScale().x,primitive->GetScale().y,primitive->GetScale().z };

        j["Primitives"][std::to_string(id)] = {
            {"Location", Location},
            {"Rotation", Rotation},
            {"Scale", Scale},
            {"Type", static_cast<UPrimitiveComponent*>(primitive)->GetType()}
        };
    }

    return j.dump(4); // 4�� �鿩���� ����
}

bool FSceneManager::SaveSceneToFile(const std::string& filename, const SceneData& sceneData)
{
    std::ofstream outFile(filename);
    if (!outFile) {
        FString errorMessage = "Failed to open file for writing: ";
        MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    std::string jsonData = SerializeSceneData(sceneData);
    outFile << jsonData;
    outFile.close();

    return true;
}
