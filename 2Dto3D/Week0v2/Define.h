#pragma once
#include <cmath>
#include <algorithm>
#include <list>
#include <vector>
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

	// ���� ����
	float Dot(const FVector& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// ���� ũ��
	float Magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	// ���� ����ȭ
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
	// ��Į�� ����
	FVector operator*(float scalar) const {
		return FVector(x * scalar, y * scalar, z * scalar);
	}

	bool operator==(const FVector& other) const {
		return (x == other.x && y == other.y && z == other.z);
	}

	float Distance(const FVector& other) const {
		// �� ������ �� ������ ũ�⸦ ���
		return ((*this - other).Magnitude());
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
};

struct FMatrix
{
	float M[4][4];
	static const FMatrix Identity;
	// �⺻ ������ �����ε�
	FMatrix operator+(const FMatrix& Other) const;
	FMatrix operator-(const FMatrix& Other) const;
	FMatrix operator*(const FMatrix& Other) const;
	FMatrix operator*(float Scalar) const;
	FMatrix operator/(float Scalar) const;

	// ��ƿ��Ƽ �Լ�
	static FMatrix Transpose(const FMatrix& Mat);
	static float Determinant(const FMatrix& Mat);
	static FMatrix Inverse(const FMatrix& Mat);
};

enum OBJECTS
{
	OBJ_CAMERA,
	OBJ_END
};