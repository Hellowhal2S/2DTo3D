#pragma once
#include <algorithm>
#include <list>
#include <vector>
#include <string>
#include "Matrix.h"


#define PI 3.14

template<typename T>
using TArray = std::vector<T>;

template<typename T>
using TDoubleLinkedList = std::list<T>;

#define FString std::string

#define UE_LOG Console::GetInstance().AddLog
#define Super __super
#define TMap std::unordered_map

typedef int int32;
typedef unsigned int uint32;

enum class EObjectType
{
    Cube,
    Sphere
};


