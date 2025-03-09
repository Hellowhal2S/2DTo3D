#pragma once
#include "UObject.h"
#include "VectorUtils.h"
#include "USceneComponent.h"
#include "Define.h"
#include "UWorld.h"
#include "windows.h"
class UCameraComponent:public USceneComponent{
	// View 관련
private:
	FVector look;
	FVector up;
	FVector forward;
	FVector right;

	// 필요하면 넣기
	//FVector viewMatrix;
	//FVector ProjectionMatrix;

	static float FOV;
	static float width;
	static float height;
	static float zNear;
	static float zFar;

	static float velocity;

	FMatrix viewMatrix;
	FMatrix projMatrix;

public:
	POINT lastMousePos;

	UCameraComponent();

	void MoveForward();
	void MoveBack();
	void MoveRight();
	void MoveLeft();
	void OnLeftClick();
	void OnMouseMove(float deltaX, float deltaY, float deltaZ);
	void AdjustRotation(float x, float y, float z);
	FVector GetEyePosition();
	void SetEyePosition(FVector pos);
	
	FVector GetLook();
	void SetLook(FVector pos);

	FVector GetUp();
	void SetUp(FVector pos);
	
	FVector GetForward();
	void SetForward(FVector pos);

	FVector GetRight();
	void SetRight(FVector pos);

	FMatrix GetViewMatrix();
	void UpdateViewMatrix();

	FMatrix GetProjectionMatrix();
	void UpdateProjectionMatrix();

	void Init(UWorld* _World) override;
	void Update(double deltaTime) override;
	void Release() override;
	void Input();

	void RotateCamera();
};