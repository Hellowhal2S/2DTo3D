#pragma once
#include <initializer_list>
#include <cstring>
#include <cmath>
#include "VectorUtils.h"

struct FMatrix
{
    float M[4][4];

    static const FMatrix Identity;


    FMatrix();
    FMatrix(std::initializer_list<std::initializer_list<float>> values);
    FMatrix(float values[4][4]);
    //FMatrix(FVector vec);

    FMatrix Transpose() const;
    FMatrix Inverse() const;
    FMatrix NormalMatrix() const;

    bool IsOrthogonal() const;
    bool IsOrthonormal() const;

    static float Determinant2x2(float a, float b, float c, float d);
    static float Determinant3x3(const float(&m)[3][3]);
    float Determinant() const;

    FMatrix GetModelMatrix(FVector eye, FVector target, FVector up);
    FMatrix Lookat(FVector pos, FVector look, FVector up);
    FMatrix Perspective(float fov, float width, float height, float zNear, float zFar);
    float& Get(int row, int col);

    FMatrix operator+(const FMatrix& Other) const;
    FMatrix operator-(const FMatrix& Other) const;
    FMatrix operator*(const FMatrix& Other) const;
    FMatrix operator/(float Scalar) const;
};