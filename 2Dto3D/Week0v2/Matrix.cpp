#include "Matrix.h"

// 단위 행렬 
const FMatrix FMatrix::Identity = { {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
} };

FMatrix FMatrix::operator+(const FMatrix& Other) const {
    FMatrix Result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] + Other.M[i][j];
    return Result;
}

FMatrix FMatrix::operator-(const FMatrix& Other) const {
    FMatrix Result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] - Other.M[i][j];
    return Result;
}

// 행렬 곱셈
FMatrix FMatrix::operator*(const FMatrix& Other) const {
    FMatrix Result = {};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                Result.M[i][j] += M[i][k] * Other.M[k][j];
    return Result;
}

// 스칼라 곱셈
FMatrix FMatrix::operator*(float Scalar) const {
    FMatrix Result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] * Scalar;
    return Result;
}

// 스칼라 나눗셈
FMatrix FMatrix::operator/(float Scalar) const {
    FMatrix Result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] / Scalar;
    return Result;
}

// 전치 행렬
FMatrix FMatrix::Transpose(const FMatrix& Mat) {
    FMatrix Result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Result.M[i][j] = Mat.M[j][i];
    return Result;
}

// 행렬식 계산
float FMatrix::Determinant(const FMatrix& Mat) {
    float det = 0.0f;
    for (int i = 0; i < 4; i++) {
        float subMat[3][3];
        for (int j = 1; j < 4; j++) {
            int colIndex = 0;
            for (int k = 0; k < 4; k++) {
                if (k == i) continue;
                subMat[j - 1][colIndex] = Mat.M[j][k];
                colIndex++;
            }
        }
        float minorDet =
            subMat[0][0] * (subMat[1][1] * subMat[2][2] - subMat[1][2] * subMat[2][1]) -
            subMat[0][1] * (subMat[1][0] * subMat[2][2] - subMat[1][2] * subMat[2][0]) +
            subMat[0][2] * (subMat[1][0] * subMat[2][1] - subMat[1][1] * subMat[2][0]);
        det += (i % 2 == 0 ? 1 : -1) * Mat.M[0][i] * minorDet;
    }
    return det;
}

// 역행렬
FMatrix FMatrix::Inverse(const FMatrix& Mat) {
    float det = Determinant(Mat);
    if (fabs(det) < 1e-6) {
        return Identity;
    }

    FMatrix Inv;
    float invDet = 1.0f / det;

    // 여인수 행렬 계산 후 전치하여 역행렬 계산
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float subMat[3][3];
            int subRow = 0;
            for (int r = 0; r < 4; r++) {
                if (r == i) continue;
                int subCol = 0;
                for (int c = 0; c < 4; c++) {
                    if (c == j) continue;
                    subMat[subRow][subCol] = Mat.M[r][c];
                    subCol++;
                }
                subRow++;
            }
            float minorDet =
                subMat[0][0] * (subMat[1][1] * subMat[2][2] - subMat[1][2] * subMat[2][1]) -
                subMat[0][1] * (subMat[1][0] * subMat[2][2] - subMat[1][2] * subMat[2][0]) +
                subMat[0][2] * (subMat[1][0] * subMat[2][1] - subMat[1][1] * subMat[2][0]);

            Inv.M[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * minorDet * invDet;
        }
    }
    return Inv;
}

FVector4 ConvertV3ToV4(FVector vec3)
{
    FVector4 newVec4;
    newVec4.x = vec3.x;
    newVec4.y = vec3.y;
    newVec4.z = vec3.z;
    return newVec4;
}

FMatrix CreateRotation(float angle, FVector axis)
{
    axis = axis.Normalize();
    float cosA = cosf(angle * (3.14159265359f / 180.0f));
    float sinA = sinf(angle * (3.14159265359f / 180.0f));
    float oneMinusCosA = 1.0f - cosA;

    return {
        {
            {
                cosA + axis.x * axis.x * oneMinusCosA,
                axis.x * axis.y * oneMinusCosA - axis.z * sinA,
                axis.x * axis.z * oneMinusCosA + axis.y * sinA,
                0
            },
            {
                axis.y * axis.x * oneMinusCosA + axis.z * sinA,
                cosA + axis.y * axis.y * oneMinusCosA,
                axis.y * axis.z * oneMinusCosA - axis.x * sinA,
                0
            },
            {
                axis.z * axis.x * oneMinusCosA - axis.y * sinA,
                axis.z * axis.y * oneMinusCosA + axis.x * sinA,
                cosA + axis.z * axis.z * oneMinusCosA,
                0
            },
            {0, 0, 0, 1}
        }
    };
}



