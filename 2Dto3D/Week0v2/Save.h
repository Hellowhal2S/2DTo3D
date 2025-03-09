#pragma once
#include <string>
#include "Object.h"
 // json-for-modern-cpp 라이브러리 포함

class USaveManager
{
public:
    USaveManager();
    ~USaveManager();

    // 씬 초기화
    void NewScene(std::vector<UObject*>& ObjectList);

    // 씬 저장
    void SaveScene(const std::string& FilePath, const std::vector<UObject*>& ObjectList);

    // 씬 불러오기
    void LoadScene(const std::string& FilePath, std::vector<UObject*>& ObjectList);

    // ImGui를 통해 씬 관리 UI 표시
    void RenderSceneManagerUI(std::vector<UObject*>& ObjectList);

    std::string filename;
};