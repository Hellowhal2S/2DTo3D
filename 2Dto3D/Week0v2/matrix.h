#include <cmath>

#pragma once
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

	// ���� ����		
	FVector Cross(const FVector& other) const {
		return FVector(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
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
	static FMatrix CreateRotation(float roll, float pitch, float yaw);
	static FMatrix CreateScale(float scaleX, float scaleY, float scaleZ);
	static FVector TransformVector(const FVector& v, const FMatrix& m);
	static FVector4 TransformVector(const FVector4& v, const FMatrix& m);
	static FMatrix CreateTranslationMatrix(const FVector& position);

};

struct FQuat {
    float x, y, z, w;

    FQuat() : x(0), y(0), z(0), w(1) {}
    FQuat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    // Axis-Angle -> Quaternion ��ȯ
    static FQuat FromAxisAngle(const FVector& axis, float angleDeg) {
        float angleRad = angleDeg * (3.14159265359f / 180.0f);
        float halfAngle = angleRad * 0.5f;
        float sinHalfAngle = sin(halfAngle);

        return FQuat(
            axis.x * sinHalfAngle,
            axis.y * sinHalfAngle,
            axis.z * sinHalfAngle,
            cos(halfAngle)
        );
    }

    // Quaternion ����
    FQuat operator*(const FQuat& q) const {
        return FQuat(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z
        );
    }

    // Quaternion�� ȸ�� ��ķ� ��ȯ
    FMatrix ToMatrix() const {
        FMatrix result = FMatrix::Identity;
        float xx = x * x, yy = y * y, zz = z * z;
        float xy = x * y, xz = x * z, yz = y * z;
        float wx = w * x, wy = w * y, wz = w * z;

        result.M[0][0] = 1.0f - 2.0f * (yy + zz);
        result.M[0][1] = 2.0f * (xy - wz);
        result.M[0][2] = 2.0f * (xz + wy);

        result.M[1][0] = 2.0f * (xy + wz);
        result.M[1][1] = 1.0f - 2.0f * (xx + zz);
        result.M[1][2] = 2.0f * (yz - wx);

        result.M[2][0] = 2.0f * (xz - wy);
        result.M[2][1] = 2.0f * (yz + wx);
        result.M[2][2] = 1.0f - 2.0f * (xx + yy);

        return result;
    }

    // ���� ȸ��
    FVector RotateVector(const FVector& v) const {
        FQuat qv(v.x, v.y, v.z, 0);
        FQuat qInv(-x, -y, -z, w); // Quaternion ���Լ�
        FQuat rotatedQ = (*this) * qv * qInv;

        return FVector(rotatedQ.x, rotatedQ.y, rotatedQ.z);
    }

    // Quaternion�� Euler ������ ��ȯ
    FVector ToEuler() const {
        FVector euler;
        float sinr_cosp = 2 * (w * x + y * z);
        float cosr_cosp = 1 - 2 * (x * x + y * y);
        euler.x = atan2(sinr_cosp, cosr_cosp) * (180.0f / 3.14159265359f);

        float sinp = 2 * (w * y - z * x);
        if (fabs(sinp) >= 1)
            euler.y = copysign(90.0f, sinp); // Clamping
        else
            euler.y = asin(sinp) * (180.0f / 3.14159265359f);

        float siny_cosp = 2 * (w * z + x * y);
        float cosy_cosp = 1 - 2 * (y * y + z * z);
        euler.z = atan2(siny_cosp, cosy_cosp) * (180.0f / 3.14159265359f);

        return euler;
    }

    // Euler �����κ��� Quaternion ��ȯ
    static FQuat FromEuler(const FVector& euler) {
        float roll = euler.x * (3.14159265359f / 180.0f);
        float pitch = euler.y * (3.14159265359f / 180.0f);
        float yaw = euler.z * (3.14159265359f / 180.0f);

        float cy = cos(yaw * 0.5f);
        float sy = sin(yaw * 0.5f);
        float cp = cos(pitch * 0.5f);
        float sp = sin(pitch * 0.5f);
        float cr = cos(roll * 0.5f);
        float sr = sin(roll * 0.5f);

        return FQuat(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }
};





