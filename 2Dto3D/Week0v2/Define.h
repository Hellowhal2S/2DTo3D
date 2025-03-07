#pragma once
#include <cmath>
#include <algorithm>
#include <list>
#include <vector>
#include <DirectXMath.h>

#define PI 3.14
#pragma once

#define TArray std::vector
#define TDoubleLinkedList std::list

#define Super __super
struct FVertexSimple
{
	float x, y, z;    // Position
	float r, g, b, a; // Color
};

struct FVector
{
	float x, y, z;
	FVector(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

	FVector operator-(const FVector& other) const {
		return FVector(x - other.x, y - other.y, z - other.z);
	}
	FVector operator+(const FVector& other) const {
		return FVector(x + other.x, y + other.y, z + other.z);
	}

	// 벡터 내적
	float Dot(const FVector& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// 벡터 크기
	float Magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	// 벡터 정규화
	FVector Normalize() const {
		float mag = Magnitude();
		return (mag > 0) ? FVector(x / mag, y / mag, z / mag) : FVector(0, 0, 0);
	}
	FVector Cross(const FVector& Other) const
	{
		return FVector{
			y * Other.z - z * Other.y,
			z * Other.x - x * Other.z,
			x * Other.y - y * Other.x
		};
	}
	// 스칼라 곱셈
	FVector operator*(float scalar) const {
		return FVector(x * scalar, y * scalar, z * scalar);
	}

	bool operator==(const FVector& other) const {
		return (x == other.x && y == other.y && z == other.z);
	}

	float Distance(const FVector& other) const {
		// 두 벡터의 차 벡터의 크기를 계산
		return ((*this - other).Magnitude());
	}
	DirectX::XMFLOAT3 ToXMFLOAT3() const
	{
		return DirectX::XMFLOAT3(x, y, z);
	}
};

struct FVector4 {
	float x, y, z, a;
	FVector4(float _x = 0, float _y = 0, float _z = 0, float _a = 0) : x(_x), y(_y), z(_z), a(_a) {}

	FVector4 operator-(const FVector4& other) const {
		return FVector4(x - other.x, y - other.y, z - other.z, a - other.a);
	}
	FVector4 operator+(const FVector4& other) const {
		return FVector4(x + other.x, y + other.y, z + other.z, a + other.a);
	}
	FVector4 operator/(float scalar) const
	{
		return FVector4{ x / scalar, y / scalar, z / scalar, a / scalar };
	}
};

struct FMatrix
{
	float M[4][4];
	static const FMatrix Identity;
	// 기본 연산자 오버로딩
	FMatrix operator+(const FMatrix& Other) const;
	FMatrix operator-(const FMatrix& Other) const;
	FMatrix operator*(const FMatrix& Other) const;
	FMatrix operator*(float Scalar) const;
	FMatrix operator/(float Scalar) const;

	// 유틸리티 함수
	static FMatrix Transpose(const FMatrix& Mat);
	static float Determinant(const FMatrix& Mat);
	static FMatrix Inverse(const FMatrix& Mat);

	DirectX::XMMATRIX ToXMMATRIX() const
	{
		return DirectX::XMMatrixSet(
			M[0][0], M[1][0], M[2][0], M[3][0], // 첫 번째 열
			M[0][1], M[1][1], M[2][1], M[3][1], // 두 번째 열
			M[0][2], M[1][2], M[2][2], M[3][2], // 세 번째 열
			M[0][3], M[1][3], M[2][3], M[3][3]  // 네 번째 열
		);
	}
	FVector4 TransformFVector4(const FVector4& vector)
	{
		return FVector4(
			M[0][0] * vector.x +M[1][0] * vector.y + M[2][0] * vector.z + M[3][0] * vector.a,
			M[0][1] * vector.x +M[1][1] * vector.y + M[2][1] * vector.z + M[3][1] * vector.a,
			M[0][2] * vector.x +M[1][2] * vector.y + M[2][2] * vector.z + M[3][2] * vector.a,
			M[0][3] * vector.x +M[1][3] * vector.y + M[2][3] * vector.z + M[3][3] * vector.a
		);														
	}
	FVector TransformPosition(const FVector& vector) const
	{
		float x = M[0][0] * vector.x + M[1][0] * vector.y + M[2][0] * vector.z + M[3][0];
		float y = M[0][1] * vector.x + M[1][1] * vector.y + M[2][1] * vector.z + M[3][1];
		float z = M[0][2] * vector.x + M[1][2] * vector.y + M[2][2] * vector.z + M[3][2];
		float w = M[0][3] * vector.x + M[1][3] * vector.y + M[2][3] * vector.z + M[3][3];
		return w != 0.0f ? FVector{ x / w, y / w, z / w } : FVector{ x, y, z };
	}
};

enum OBJECTS
{
	OBJ_SPHERE,
	OBJ_CUBE,
	OBJ_TRIANGLE,
	OBJ_CAMERA,
	OBJ_END
};

