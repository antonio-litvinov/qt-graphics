#ifndef HEADER_H
#define HEADER_H

#include <fstream>
#include <vector>
#include <QColor>
#include <QTime>


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

typedef struct
{
   QString FileName;

   vector <TModel> Models;
   vector <TLight> Light;

   TCamera  Camera;

   T3DPoint Move;
   T3DPoint Scale;
   T3DPoint Rotate;

   double Ka;
   double Kd;
   double Ks;

   int Hour;
   int Min;
   int Sec;

   bool Clock;

}  TScene;

TVector Multiply(TVector Vector);

void InitScene(TScene &Scene);

void InitMove(double dx, double dy, double dz);

void InitScale(double kx, double ky, double kz);

void InitRotate(double Angle, char Axis);

void InitTransform(double Alpha, double Beta);

void Projection(TScene &Scene);

void Move(TModel &Model, double dx, double dy, double dz);

void Scale(TModel &Model, double kx, double ky, double kz);

void Rotate(TModel &Model, double X, double Y, double Z);

void DrawScene(TScene Scene);

void FindColor(TScene &Scene);

void SetTime(TScene &Scene);

#endif // HEADER_H
