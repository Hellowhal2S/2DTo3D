#pragma once
#include "SceneComponent.h"
#include "Matrix.h"

class UCamera : public USceneComponent
{
public:
    UCamera();
    virtual ~UCamera();
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Release() override;

	float fovDegrees;
	bool bOrthographic;

    void SetProjection(float aspectRatio, float nearZ, float farZ);
    void MoveForward(float delta);
    void MoveRight(float delta);
    void Rotate(float yawDelta, float pitchDelta);

    FMatrix GetViewMatrix() const;
    FMatrix GetProjectionMatrix() const;
    FVector GetForward() const
    {
		return Forward;
    }
    void LookAt(const FVector& target);

private:
    FVector Forward;
    FVector Right;
    FVector Up;
    float Yaw;
    float Pitch;

    FMatrix ProjectionMatrix;

    void UpdateVectors();
   

};
