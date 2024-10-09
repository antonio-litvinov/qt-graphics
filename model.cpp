#include "model.h"
#include "functions.h"

TModel::TModel()
{

}

TVector TModel::Multiply(TVector Vector)
{
    TVector Result;
    double Sum;

    for (int I = 0; I < 4; I++)
    {
        Sum = 0;
        for (int J = 0; J < 4; J++)
            Sum += Vector[J] * Transform[I][J];
        Result.push_back(Sum);
    }
    return Result;
}

void TModel::Clear()
{
    for (int I = 0; I < 4; I++)
        for (int J = 0; J < 4; J++)
            Transform[I][J] = 0;
}

void TModel::InitMove(double dx, double dy, double dz)
{
    Clear();
    Transform[0][0] = 1;
    Transform[1][1] = 1;
    Transform[2][2] = 1;
    Transform[3][3] = 1;
    Transform[0][3] = dx;
    Transform[1][3] = dy;
    Transform[2][3] = dz;
}

void TModel::InitScale(double kx, double ky, double kz)
{
    Clear();
    Transform[0][0] = kx;
    Transform[1][1] = ky;
    Transform[2][2] = kz;
    Transform[3][3] = 1;
}

void TModel::InitRotate(double Angle, char Axis)
{
    Clear();
    Angle = DegToRad(Angle);

    switch (Axis)
    {
    case 'X':
        Transform[0][0] = 1;
        Transform[1][1] = cos(Angle);
        Transform[1][2] = -sin(Angle);
        Transform[2][1] = sin(Angle);
        Transform[2][2] = cos(Angle);
        Transform[3][3] = 1;
        break;

    case 'Y':
        Transform[0][0] = cos(Angle);
        Transform[0][2] = -sin(Angle);
        Transform[1][1] = 1;
        Transform[2][0] = sin(Angle);
        Transform[2][2] = cos(Angle);
        Transform[3][3] = 1;
        break;

    case 'Z':
        Transform[0][0] = cos(Angle);
        Transform[0][1] = -sin(Angle);
        Transform[1][0] = sin(Angle);
        Transform[1][1] = cos(Angle);
        Transform[2][2] = 1;
        Transform[3][3] = 1;
        break;
    }
}

void TModel::InitTransform(double Alpha, double Beta)
{
    Clear();

    Alpha = DegToRad(Alpha);
    Beta = DegToRad(Beta);

    Transform[0][0] = cos(Alpha);
    Transform[0][1] = -sin(Alpha);
    Transform[1][0] = sin(Alpha) * cos(Beta);
    Transform[1][1] = cos(Alpha) * cos(Beta);
    Transform[1][2] = -sin(Beta);
    Transform[2][0] = sin(Alpha) * sin(Beta);
    Transform[2][1] = cos(Alpha) * sin(Beta);
    Transform[2][2] = cos(Beta);
    Transform[3][3] = 1;
}

void TModel::Move(double DX, double DY, double DZ)
{
    TVector xyz,XYZ;

    InitMove(DX, DY, DZ);

    for (int I = 0; I < NumVertex; I++)
    {
        xyz = PutPoint(Vertices[I].Point);
        XYZ = Multiply(xyz);
        Vertices[I].Point = GetPoint(XYZ);
    }
}

void TModel::Scale(double KX, double KY, double KZ)
{
    TVector xyz,XYZ;

    InitScale(KX, KY, KZ);

    for (int I = 0; I < NumVertex; I++)
    {
        xyz = PutPoint(Vertices[I].Point);
        XYZ = Multiply(xyz);
        Vertices[I].Point = GetPoint(XYZ);
    }
}

void TModel::Rotate(double X, double Y, double Z)
{
    TVector xyz,XYZ;

    for (int I = 0; I < NumVertex; I++)
    {
        xyz = PutPoint(Vertices[I].Point);

        InitRotate(X, 'X');
        XYZ = Multiply(xyz);
        xyz = XYZ;

        InitRotate(Y, 'Y');
        XYZ = Multiply(xyz);
        xyz = XYZ;

        InitRotate(Z, 'Z');
        XYZ = Multiply(xyz);

        Vertices[I].Point = GetPoint(XYZ);

        xyz = PutPoint(Vertices[I].Normal);

        InitRotate(X, 'X');
        XYZ = Multiply(xyz);
        xyz = XYZ;

        InitRotate(Y, 'Y');
        XYZ = Multiply(xyz);
        xyz = XYZ;

        InitRotate(Z, 'Z');
        XYZ = Multiply(xyz);

        Vertices[I].Normal = GetPoint(XYZ);
    }
}
