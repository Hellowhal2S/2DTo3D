#pragma once
#include <string>
#include "Object.h"
 // json-for-modern-cpp ���̺귯�� ����

class USaveManager
{
public:
    USaveManager();
    ~USaveManager();

    // �� �ʱ�ȭ
    void NewScene(std::vector<UObject*>& ObjectList);

    // �� ����
    void SaveScene(const std::string& FilePath, const std::vector<UObject*>& ObjectList);

    // �� �ҷ�����
    void LoadScene(const std::string& FilePath, std::vector<UObject*>& ObjectList);

    // ImGui�� ���� �� ���� UI ǥ��
    void RenderSceneManagerUI(std::vector<UObject*>& ObjectList);

    std::string filename;
};