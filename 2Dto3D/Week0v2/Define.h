#pragma once
#include <cmath>
#define NULL 0

struct FVector
{
	float X;
	float Y;
	float Z;

	FVector(float _x = 0, float _y = 0, float _z = 0) : X(_x), Y(_y), Z(_z) {}

	FVector operator+(const FVector& rhs) {
		return FVector(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
	}

	FVector operator-(const FVector& rhs) {
		return FVector(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
	}

	FVector operator*(float Scalar) const {
		return FVector(X * Scalar, Y * Scalar, Z * Scalar);
	}

	friend FVector operator*(float Scalar, const FVector& Vector) {
		return Vector * Scalar;
	}

	float Dot(const FVector& rhs) {
		return X * rhs.X + Y * rhs.Y + Z * rhs.Z;
	}

	FVector Cross(const FVector& rhs) {
		return FVector(Y * rhs.Z - Z * rhs.Y, Z * rhs.X - X * rhs.Z, X * rhs.Y - Y * rhs.X);
	}

	float Length() const {
		return sqrt(X * X + Y * Y + Z * Z);
	}

	FVector Normalize() {
		float length = Length();
		if (length > 0)
		{
			X /= length;
			Y /= length;
			Z /= length;
		}
		return *this;
	}
};

enum OBJECTLIST
{
	OL_PLAYER,
	OL_BALL,
	OL_DAGGER,
	OL_UI,
	OL_END
};