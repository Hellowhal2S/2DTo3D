#include "UObject.h"

TArray<UObject*> UObject::GUObjectArray;
UObject::UObject() {
    static uint32_t NextUUID = 1;
    UUID = NextUUID++;
    Internalndex = UUID - 1;
}

UObject::~UObject() {}

//template <typename T>
//UObject* UObject::NewObject() {
//
//    //UObject 하위 클래스만 생성 가능하도록 제한
//    //static_assert(std::is_base_of<UObject, T>::value, "T must be a subclass of UObject");
//
//    //T* NewObj = new T();
//    UObject* NewObj = new UObject;
//    GUObjectArray.push_back(NewObj);
//    return NewObj;
//};

//template UObject* UObject::NewObject<UObject>();