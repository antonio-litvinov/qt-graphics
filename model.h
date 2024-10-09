#ifndef MODEL_H
#define MODEL_H

#include "types.h"

class TModel
{
public:
    TModel();

    vector<TVertex> Vertices;
    vector<TFace> Faces;

    int NumVertex;
    int NumFaces;

    QColor Color;

    double Transform[4][4];

    TVector Multiply(TVector Vector);
    void Clear();
    void InitMove(double dx, double dy, double dz);
    void InitScale(double kx, double ky, double kz);
    void InitRotate(double Angle, char Axis);
    void InitTransform(double Alpha, double Beta);

    void Move(double DX, double DY, double DZ);
    void Scale(double KX, double KY, double KZ);
    void Rotate(double X, double Y, double Z);
};

#endif // MODEL_H
