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
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            Result.M[i][j] = M[i][j] + Other.M[i][j];
    return Result;
}

// 행렬 뺄셈
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

// 스칼라 나눗셈
FMatrix FMatrix::operator/(float Scalar) const {
    FMatrix Result;
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

FMatrix FMatrix::Lookat(FVector pos, FVector look, FVector up) {
    FVector forward = (pos - look).Normalize();
    FVector right = up.Cross(forward).Normalize();
    FVector upDir = forward.Cross(right).Normalize();

    FMatrix result;
    result.Get(0, 0) = right.x;    result.Get(0, 1) = upDir.x;    result.Get(0, 2) = forward.x;    result.Get(0, 3) = 0.0f;
    result.Get(1, 0) = right.y;    result.Get(1, 1) = upDir.y;    result.Get(1, 2) = forward.y;    result.Get(1, 3) = 0.0f;
    result.Get(2, 0) = right.z;    result.Get(2, 1) = upDir.z;    result.Get(2, 2) = forward.z;    result.Get(2, 3) = 0.0f;
    result.Get(3, 0) = -right.Dot(pos);
    result.Get(3, 1) = -upDir.Dot(pos);
    result.Get(3, 2) = -forward.Dot(pos);
    result.Get(3, 3) = 1.0f;
    return result;
}

float& FMatrix::Get(int row, int col) {
    return M[row][col];
}

FMatrix FMatrix::Perspective(float fov, float width, float height, float zNear, float zFar) {
    FMatrix mat = FMatrix::Identity;
    float aspectRatio = width / height;
    float yScale = 1.0f / tanf(fov * 0.5f);  // Y축 스케일 (fov의 절반을 tan()으로 계산)
    float xScale = yScale / aspectRatio;     // X축 스케일
    float zRange = zFar - zNear;

    // 원근 투영 행렬 (Left-Handed, DirectX 스타일)
    mat.Get(0, 0) = xScale;
    mat.Get(1, 1) = yScale;
    mat.Get(2, 2) = zFar / zRange;  // zFar / (zFar - zNear)
    mat.Get(2, 3) = 1.0f;           // w 분리 (Perspective division)
    mat.Get(3, 2) = (-zNear * zFar) / zRange; // -zNear * zFar / (zFar - zNear)
    mat.Get(3, 3) = 0.0f;

    return mat;
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

    FMatrix Rotation = RotationZ * RotationY * RotationX;

    // 이동 행렬
    FMatrix Translation = FMatrix::Identity;
    Translation.M[3][0] = translation.x;
    Translation.M[3][1] = translation.y;
    Translation.M[3][2] = translation.z;
    Translation.M[3][3] = 1.0f;

    // 최종 모델 행렬 = 이동 * 회전 * 스케일
    return Scale * Rotation * Translation;
}