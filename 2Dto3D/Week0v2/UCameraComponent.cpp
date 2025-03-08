#include "UCameraComponent.h"
#include <d3d11.h>

float UCameraComponent::FOV = 70.0f;
float UCameraComponent::width = 1280.0f;
float UCameraComponent::height = 800.0f;
float UCameraComponent::zNear = 0.1f;
float UCameraComponent::zFar = 1000.0f;
float UCameraComponent::velocity;
UCameraComponent::UCameraComponent() {
	// FIXME : 추후 카메라의 회전에 따라 변경.
	velocity = 0.1;
	forward = FVector(0, 0, -1);
	right = FVector(1, 0, 0);
	look = FVector(0, 0, 0);
	up = FVector(0, 1, 0);
}
void UCameraComponent::MoveForward() {
	 SetEyePosition(RelativeLocation + (forward * velocity));
	 std::string msg = "forward {" + std::to_string(RelativeLocation.x) + ", " + std::to_string(RelativeLocation.y) + ", " + std::to_string(RelativeLocation.z);
	 std::wstring wmsg(msg.begin(), msg.end());
	 MessageBox(NULL, wmsg.c_str(), L"Message Box", MB_OK);
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
	UpdateViewMatrix();
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

		MoveForward();
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { // ↓ 방향키
		MoveBack();
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { // → 방향키
		MoveRight();
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { // ← 방향키
		MoveLeft();
	}
}


void UCameraComponent::UpdateViewMatrix() {
	viewMatrix.Lookat(RelativeLocation, look, up);
}

void UCameraComponent::UpdateProjectionMatrix() {
	projMatrix = projMatrix.Perspective(FOV, width, height, zNear, zFar);
}
void UCameraComponent::Init(UWorld* _World) {
	RelativeLocation = FVector(0.0f, 0.0f, 0.5f);
}

void UCameraComponent::Update(double deltaTime) {
	Input();
	UpdateViewMatrix();
}
void UCameraComponent::Release() {

}