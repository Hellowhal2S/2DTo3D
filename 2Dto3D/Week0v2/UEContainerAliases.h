#pragma once
/*
* TODO: Core/Containers/Array.h와 같은 형태로 재정의 할 것, 지금은 단순히 별칭만 정의
*/
#include <vector>
#include <set>
#include <unordered_map>
#include <string>
#include <cstdint>

template<typename T>
using TArray = std::vector<T>;

template<typename T>
using TDoubleLinkedList = std::list<T>;

template<typename T>
using TSet = std::set<T>;

template<typename KeyType, typename ValueType>
using TMap = std::unordered_map<KeyType, ValueType>;

template<typename T1, typename T2>
using TPair = std::pair<T1, T2>;

using FString = std::string;

typedef int int32;

typedef unsigned int uint32;