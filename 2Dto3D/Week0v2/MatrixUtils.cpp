#include "MatrixUtils.h"

const FMatrix FMatrix::Identity = FMatrix({
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
    });

FMatrix::FMatrix()
{
    *this = Identity;
}

FMatrix::FMatrix(std::initializer_list<std::initializer_list<float>> values)
{
    int i = 0;
    for (const auto& row : values)
    {
        if (i >= 4) break; // 초과 방지
        int j = 0;
        for (const auto& element : row)
        {
            if (j >= 4) break; // 초과 방지
            M[i][j] = element;
            j++;
        }
        i++;
    }
}

FMatrix::FMatrix(float values[4][4])
{
    std::memcpy(M, values, sizeof(M)); // 행렬 복사
}

FMatrix FMatrix::Transpose() const
{
    FMatrix result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.M[i][j] = M[j][i]; // 행과 열을 바꿈
    return result;
}

float FMatrix::Determinant2x2(float a, float b, float c, float d)
{
    return (a * d) - (b * c);
}

float FMatrix::Determinant3x3(const float(&m)[3][3])
{
    return m[0][0] * Determinant2x2(m[1][1], m[1][2], m[2][1], m[2][2])
        - m[0][1] * Determinant2x2(m[1][0], m[1][2], m[2][0], m[2][2])
        + m[0][2] * Determinant2x2(m[1][0], m[1][1], m[2][0], m[2][1]);
}

float FMatrix::Determinant() const
{
    float subMatrix[3][3];
    float determinant = 0.0f;

    for (int i = 0; i < 4; i++)
    {
        int subi = 0;
        for (int j = 1; j < 4; j++)
        {
            int subj = 0;
            for (int k = 0; k < 4; k++)
            {
                if (k == i) continue;
                subMatrix[subi][subj] = M[j][k];
                subj++;
            }
            subi++;
        }
        determinant += M[0][i] * Determinant3x3(subMatrix) * ((i % 2 == 0) ? 1 : -1);
    }
    return determinant;
}

FMatrix FMatrix::Inverse() const
{
    float det = this->Determinant();
    if (fabs(det) < 1e-6) return Identity; // 역행렬이 존재하지 않으면 단위 행렬 반환

    FMatrix adjugate;
    float subMatrix[3][3];

    // 여인수 행렬 (Cofactor) 계산
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int subi = 0;
            for (int si = 0; si < 4; si++)
            {
                if (si == i) continue;
                int subj = 0;
                for (int sj = 0; sj < 4; sj++)
                {
                    if (sj == j) continue;
                    subMatrix[subi][subj] = M[si][sj];
                    subj++;
                }
                subi++;
            }
            adjugate.M[j][i] = Determinant3x3(subMatrix) * (((i + j) % 2 == 0) ? 1 : -1);
        }
    }

    // 최종 역행렬 = 여인수 행렬 / 행렬식
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            adjugate.M[i][j] /= det;

    return adjugate;
}

FMatrix FMatrix::NormalMatrix() const
{
    FMatrix normalMatrix = this->Inverse();
    return normalMatrix.Transpose();
}

bool FMatrix::IsOrthogonal() const
{
    FMatrix transposed = Transpose();
    FMatrix identityCheck = FMatrix::Identity;

    // 행렬 곱셈 (A * A^T)
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            identityCheck.M[i][j] = 0;
            for (int k = 0; k < 4; k++)
                identityCheck.M[i][j] += M[i][k] * transposed.M[k][j];
        }

    // 단위 행렬과 비교
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (fabs(identityCheck.M[i][j] - Identity.M[i][j]) > 1e-6)
                return false;

    return true;
}

bool FMatrix::IsOrthonormal() const
{
    if (!IsOrthogonal()) return false;

    // 각 행벡터의 크기가 1인지 확인
    for (int i = 0; i < 4; i++)
    {
        float lengthSquared = 0.0f;
        for (int j = 0; j < 4; j++)
            lengthSquared += M[i][j] * M[i][j];

        if (fabs(lengthSquared - 1.0f) > 1e-6) return false;
    }
    return true;
}
