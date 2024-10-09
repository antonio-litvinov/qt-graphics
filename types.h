#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <QColor>

using namespace std;

typedef vector<double> TVector;
typedef vector<vector<double> > TMyCanvas;

typedef struct
{
    double X;
    double Y;
    double Z;
}  T3DPoint;

typedef struct
{
    T3DPoint Point;
    T3DPoint Normal;
    QColor Color;
}  TVertex;

typedef struct
{
    int A;
    int B;
    int C;
}  TFace;

typedef struct
{
    double Alpha;
    double Beta;
}  TCamera;

typedef struct
{
    T3DPoint Point;
    QColor Color;

}  TLight;

#endif // TYPES_H
