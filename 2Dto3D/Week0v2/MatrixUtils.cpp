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

/* 대각행렬 생성
FMatrix::FMatrix(FVector vec)
{
    // 벡터의 x, y, z 값을 대각 요소로 설정한 스케일 행렬을 생성
    M[0][0] = vec.X; M[0][1] = 0.0f;  M[0][2] = 0.0f;  M[0][3] = 0.0f;
    M[1][0] = 0.0f;  M[1][1] = vec.Y; M[1][2] = 0.0f;  M[1][3] = 0.0f;
    M[2][0] = 0.0f;  M[2][1] = 0.0f;  M[2][2] = vec.Z; M[2][3] = 0.0f;
    M[3][0] = 0.0f;  M[3][1] = 0.0f;  M[3][2] = 0.0f;  M[3][3] = 1.0f; // 동차 좌표 값
}*/

// 행렬 덧셈
FMatrix FMatrix::operator+(const FMatrix& Other) const {
    FMatrix Result;
    memset(Result.M, 0, sizeof(Result.M));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] + Other.M[i][j];
    return Result;
}

// 행렬 뺄셈
FMatrix FMatrix::operator-(const FMatrix& Other) const {
    FMatrix Result;
    memset(Result.M, 0, sizeof(Result.M));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] - Other.M[i][j];
    return Result;
}

// 행렬 곱셈
FMatrix FMatrix::operator*(const FMatrix& Other) const {
    FMatrix Result = {};
    memset(Result.M, 0, sizeof(Result.M));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                Result.M[i][j] += M[i][k] * Other.M[k][j];
    return Result;
}

// 스칼라 나눗셈
FMatrix FMatrix::operator/(float Scalar) const {
    FMatrix Result;
    memset(Result.M, 0, sizeof(Result.M));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] / Scalar;
    return Result;
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

FMatrix FMatrix::Lookat(FVector eye, FVector target, FVector up) {
    FVector zAxis = (target - eye).Normalize();  // DirectX는 LH이므로 -z가 아니라 +z 사용
    FVector xAxis = (up.Cross(zAxis)).Normalize();
    FVector yAxis = zAxis.Cross(xAxis);

    FMatrix View;
    View.M[0][0] = xAxis.x; View.M[0][1] = yAxis.x; View.M[0][2] = zAxis.x; View.M[0][3] = 0;
    View.M[1][0] = xAxis.y; View.M[1][1] = yAxis.y; View.M[1][2] = zAxis.y; View.M[1][3] = 0;
    View.M[2][0] = xAxis.z; View.M[2][1] = yAxis.z; View.M[2][2] = zAxis.z; View.M[2][3] = 0;
    View.M[3][0] = -xAxis.Dot(eye);
    View.M[3][1] = -yAxis.Dot(eye);
    View.M[3][2] = -zAxis.Dot(eye);
    View.M[3][3] = 1;

    return View;;
}

float& FMatrix::Get(int row, int col) {
    return M[row][col];
}

FMatrix FMatrix::Perspective(float fov, float aspect, float nearPlane, float farPlane) {
    float rad = fov * (3.141592f / 180.0f);
    float tanHalfFOV = tan(rad / 2.0f);
    float depth = farPlane - nearPlane;

    FMatrix Projection = {};
    Projection.M[0][0] = 1.0f / (aspect * tanHalfFOV);
    Projection.M[1][1] = 1.0f / tanHalfFOV;
    Projection.M[2][2] = farPlane / depth;  // 수정된 부분
    Projection.M[2][3] = 1.0f;              // 수정된 부분 (RH는 -1.0f였음)
    Projection.M[3][2] = -(nearPlane * farPlane) / depth;
    Projection.M[3][3] = 1.0f;

    return Projection;
}

FMatrix FMatrix::GetModelMatrix(FVector translation, FVector rotation, FVector scale) {
    // 스케일 행렬
    FMatrix Scale = FMatrix::Identity;
    Scale.M[0][0] = scale.x;
    Scale.M[1][1] = scale.y;
    Scale.M[2][2] = scale.z;

    // 회전 행렬 (Yaw-Pitch-Roll)
    float cosX = cos(rotation.x), sinX = sin(rotation.x);
    float cosY = cos(rotation.y), sinY = sin(rotation.y);
    float cosZ = cos(rotation.z), sinZ = sin(rotation.z);

    FMatrix RotationX = FMatrix::Identity;
    RotationX.M[1][1] = cosX; RotationX.M[1][2] = -sinX;
    RotationX.M[2][1] = sinX; RotationX.M[2][2] = cosX;

    FMatrix RotationY = FMatrix::Identity;
    RotationY.M[0][0] = cosY; RotationY.M[0][2] = sinY;
    RotationY.M[2][0] = -sinY; RotationY.M[2][2] = cosY;

    FMatrix RotationZ = FMatrix::Identity;
    RotationZ.M[0][0] = cosZ; RotationZ.M[0][1] = -sinZ;
    RotationZ.M[1][0] = sinZ; RotationZ.M[1][1] = cosZ;

    FMatrix Rotation = RotationZ * RotationY * RotationX;;

    // 이동 행렬
    FMatrix Translation = FMatrix::Identity;
    Translation.M[3][0] = translation.x;
    Translation.M[3][1] = translation.y;
    Translation.M[3][2] = translation.z;
    Translation.M[3][3] = 1.0f;

    // 최종 모델 행렬 = 이동 * 회전 * 스케일
    return Scale * Rotation * Translation;
}