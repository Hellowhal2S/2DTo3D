#pragma once
#include <algorithm>
#include <list>
#include <vector>
#include "Matrix.h"


#define PI 3.14

template<typename T>
using TArray = std::vector<T>;

template<typename T>
using TDoubleLinkedList = std::list<T>;
typedef int int32;
typedef unsigned int uint32;

enum class EObjectType
{
    Cube,
    Sphere
};


