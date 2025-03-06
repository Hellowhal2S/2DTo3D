#pragma once
#include <cmath>
#include <algorithm>
#include <list>
#include <vector>
#define PI 3.14
#pragma once

#define TArray std::vector
#define TDoubleLinkedList std::list

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

	// º¤ÅÍ ³»Àû
	float Dot(const FVector& other) const {
		return x * other.x + y * other.y + z * other.z;
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

	bool operator==(const FVector& other) const {
		return (x == other.x && y == other.y && z == other.z);
	}

	float Distance(const FVector& other) const {
		// µÎ º¤ÅÍÀÇ Â÷ º¤ÅÍÀÇ Å©±â¸¦ °è»ê
		return ((*this - other).Magnitude());
	}
};

