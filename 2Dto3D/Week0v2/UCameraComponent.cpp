#include "UCameraComponent.h"
#include <d3d11.h>
#include "EngineLoop.h"

float UCameraComponent::FOV = 60.0f;
float UCameraComponent::width = 1280.0f;
float UCameraComponent::height = 800.0f;
float UCameraComponent::zNear = 0.1f;
float UCameraComponent::zFar = 1000.0f;
float UCameraComponent::velocity = 10.0f;
float asepct = 1.0f;
UCameraComponent::UCameraComponent() {
	// FIXME : 추후 카메라의 회전에 따라 변경.
	// this->RelativeLocation + this->GetForwardVector()
	forward = FVector(0, 0, 1);
	look = RelativeLocation + forward;
	
	right = FVector(1, 0, 0);
	
	up = FVector(0, 1, 0);
}
void UCameraComponent::MoveForward() {
	 SetEyePosition(RelativeLocation + (forward * velocity));
	 /*std::string msg = "forward {" + std::to_string(RelativeLocation.x) + ", " + std::to_string(RelativeLocation.y) + ", " + std::to_string(RelativeLocation.z);
	 std::wstring wmsg(msg.begin(), msg.end());
	 MessageBox(NULL, wmsg.c_str(), L"Message Box", MB_OK);*/
}

void UCameraComponent::MoveBack() {
	SetEyePosition(RelativeLocation - (forward * velocity));
}

void UCameraComponent::MoveRight() {
	SetEyePosition(RelativeLocation + (right * velocity));
}

void UCameraComponent::MoveLeft() {
	SetEyePosition(RelativeLocation - (right * velocity));
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
	if (GetAsyncKeyState(VK_UP) & 0x8000) { // ↑ 방향키
		//MessageBox(EngineLoop::hWnd, L"앞", L"알림", MB_OK);
		MoveForward();
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { // ↓ 방향키
		MoveBack();
		//MessageBox(EngineLoop::hWnd, L"뒤", L"알림", MB_OK);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { // → 방향키
		MoveRight();
		//MessageBox(EngineLoop::hWnd, L"오른", L"알림", MB_OK);
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { // ← 방향키
		MoveLeft();
		//MessageBox(EngineLoop::hWnd, L"왼", L"알림", MB_OK);
	}
}

FMatrix UCameraComponent::GetViewMatrix() {
	return viewMatrix;
}
void UCameraComponent::UpdateViewMatrix() {
	viewMatrix = viewMatrix.Lookat(RelativeLocation, look, up);
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