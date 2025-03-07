#pragma once
#include <initializer_list>
#include <cstring>
#include <cmath>

struct FMatrix
{
    float M[4][4];

    static const FMatrix Identity;

    FMatrix();
    FMatrix(std::initializer_list<std::initializer_list<float>> values);
    FMatrix(float values[4][4]);

    FMatrix Transpose() const;
    FMatrix Inverse() const;
    FMatrix NormalMatrix() const;

    bool IsOrthogonal() const;
    bool IsOrthonormal() const;

    static float Determinant2x2(float a, float b, float c, float d);
    static float Determinant3x3(const float(&m)[3][3]);
    float Determinant() const;
};