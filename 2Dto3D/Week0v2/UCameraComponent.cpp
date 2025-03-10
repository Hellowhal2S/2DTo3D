//#include "USceneComponent.h"
#include "UCameraComponent.h"
#include <d3d11.h>
#include "EngineLoop.h"
float UCameraComponent::FOV = 60.0f;
float UCameraComponent::width = 1280.0f;
float UCameraComponent::height = 800.0f;
float UCameraComponent::zNear = 0.1f;
float UCameraComponent::zFar = 1000.0f;
float UCameraComponent::velocity = 0.5f;
float UCameraComponent::aspect = 1.0f;
UCameraComponent::UCameraComponent() {
	// FIXME : 추후 카메라의 회전에 따라 변경.
	// this->RelativeLocation + this->GetForwardVector()
	forward = FVector(0, 0, 1);
	look = RelativeLocation + forward;
	right = FVector(1, 0, 0);
	up = FVector(0, 1, 0);
	GetCursorPos(&lastMousePos);
}

void UCameraComponent::MoveUp() {
	RelativeLocation = RelativeLocation + GetUpVector() * velocity;
}

void UCameraComponent::MoveDown() {
	RelativeLocation = RelativeLocation - GetUpVector() * velocity;
}

void UCameraComponent::MoveForward() {
	RelativeLocation = RelativeLocation + GetForwardVector() * velocity;
}

void UCameraComponent::MoveBack() {
	RelativeLocation = RelativeLocation + GetForwardVector() * -velocity;
	//SetEyePosition(RelativeLocation - (forward * velocity));
}

void UCameraComponent::MoveRight() {
	RelativeLocation = RelativeLocation + GetRightVector() * velocity;
	//SetEyePosition(RelativeLocation + (right * velocity));
}

void UCameraComponent::MoveLeft() {
	RelativeLocation = RelativeLocation + GetRightVector() * -velocity;
	//SetEyePosition(RelativeLocation - (right * velocity));
}
void UCameraComponent::OnLeftClick() {
	
	
}

void UCameraComponent::OnMouseMove(float deltaX, float deltaY, float deltaZ) {
	AdjustRotation(deltaX, deltaY, deltaZ);
}

void UCameraComponent::AdjustRotation(float x, float y, float z) {
	float sensitivity = 0.1f;
	FVector rot;
	rot.y =  x * sensitivity;		//yaw y축 회전
	rot.x = y * sensitivity;	// pitch x축 회전
	rot.z = z * sensitivity;	// roll z축 회전

	forward = Utils::FVectorRotate(forward, rot);
	right = Utils::FVectorRotate(right, rot);
	up = Utils::FVectorRotate(up, rot);
	/*RelativeRotation.x += x;
	RelativeRotation.y += y;
	RelativeRotation.z += z;*/
	UpdateViewMatrix();
}

FVector UCameraComponent::GetEyePosition() {
	return RelativeLocation;
}
void UCameraComponent::SetEyePosition(FVector pos) {
	RelativeLocation = pos;
}

FVector UCameraComponent::GetLook() {
	return look;
}
void UCameraComponent::SetLook(FVector pos) {
	look = pos;
}

FVector UCameraComponent::GetUp() {
	return up;
}
void UCameraComponent::SetUp(FVector pos) {
	up = pos;
}

FVector UCameraComponent::GetForward() {
	return forward;
}
void UCameraComponent::SetForward(FVector pos) {
	forward = pos;
}

FVector UCameraComponent::GetRight() {
	return right;
}
void UCameraComponent::SetRight(FVector pos) {
	right = pos;
}

void UCameraComponent::Input() {
	POINT curMousePos;
	GetCursorPos(&curMousePos);

	if (GetAsyncKeyState(0x51) & 0x8000) { // Q
		MoveForward();
	}
	if (GetAsyncKeyState(0x45) & 0x8000) { // E
		MoveBack();
	}
	if (GetAsyncKeyState(0x57) & 0x8000) { // W
		MoveUp();
		
	}
	if (GetAsyncKeyState(0x53) & 0x8000) { // S
		MoveDown();
	}
	if (GetAsyncKeyState(0x44) & 0x8000) { // A
		MoveRight();
		//MessageBox(EngineLoop::hWnd, L"오른", L"알림", MB_OK);
	}
	if (GetAsyncKeyState(0x41) & 0x8000) { // D
		MoveLeft();
		//MessageBox(EngineLoop::hWnd, L"왼", L"알림", MB_OK);
	}
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {	// 우클릭	
		if (curMousePos.x != lastMousePos.x || curMousePos.y != lastMousePos.y) {
			OnMouseMove(curMousePos.x - lastMousePos.x, curMousePos.y - lastMousePos.y, 0);
		}
	}
	lastMousePos = curMousePos;
}

FMatrix UCameraComponent::GetViewMatrix() {
	return viewMatrix;
}
void UCameraComponent::UpdateViewMatrix() {
	viewMatrix = viewMatrix.Lookat(RelativeLocation, RelativeLocation+GetForward(), up);
}

FMatrix UCameraComponent::GetProjectionMatrix() {
	return projMatrix;
}
void UCameraComponent::UpdateProjectionMatrix() {
	projMatrix = projMatrix.Perspective(60, 1.0f, 0.1f, 1000.0f);
}
void UCameraComponent::Init(UWorld* _World) {
	RelativeLocation = FVector(0.0f, 0.0f, 0.5f);
}

void UCameraComponent::Update(double deltaTime) {
	Input();
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}
void UCameraComponent::Release() {

}