#pragma once
#include <math.h>
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

	// º¤ÅÍ ³»Àû
	float Dot(const FVector& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	FVector Cross(const FVector& other) const {
		return FVector(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}
	// º¤ÅÍ Å©±â
	float Magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	// º¤ÅÍ Á¤±ÔÈ­
	FVector Normalize() const {
		float mag = Magnitude();
		return (mag > 0) ? FVector(x / mag, y / mag, z / mag) : FVector(0, 0, 0);
	}
	// ½ºÄ®¶ó °ö¼À
	FVector operator*(float scalar) const {
		return FVector(x * scalar, y * scalar, z * scalar);
	}
};

struct FVector4
{
	float x, y, z, w;

	FVector4(float _x = 0, float _y = 0, float _z = 0, float _w = 1)
		: x(_x), y(_y), z(_z), w(_w) {
	}

	FVector4 operator-(const FVector4& other) const {
		return FVector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	FVector4 operator+(const FVector4& other) const {
		return FVector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	// º¤ÅÍ ³»Àû (Dot Product)
	float Dot(const FVector4& other) const {
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	// º¤ÅÍ Å©±â (Magnitude)
	float Magnitude() const {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	// º¤ÅÍ Á¤±ÔÈ­ (Normalize)
	FVector4 Normalize() const {
		float mag = Magnitude();
		return (mag > 0) ? FVector4(x / mag, y / mag, z / mag, w / mag) : FVector4(0, 0, 0, 0);
	}

	// ½ºÄ®¶ó °ö¼À (Scalar Multiplication)
	FVector4 operator*(float scalar) const {
		return FVector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}
};



