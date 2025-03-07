#pragma once
/*
* TODO: Core/Containers/Array.h�� ���� ���·� ������ �� ��, ������ �ܼ��� ��Ī�� ����
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