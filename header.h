#ifndef HEADER_H
#define HEADER_H

#include <fstream>
#include <vector>
#include <QColor>
#include <QTime>


using namespace std;

class TPicture
{
private:
   int Width;
   int Height;

   int CenterX;
   int CenterY;

   QColor FirstBackColor;
   QColor SecondBackColor;

   vector<vector<QColor> > MyCanvas;

public:
   TPicture();
   TPicture(int w, int h);

   int GetWidth();
   int GetHeight();

   int XC();
   int YC();

   void SetColor(int x, int y, QColor clr);
   QColor GetColor(int x, int y);

   void SetFirstBackColor(QColor First);
   void SetSecondBackColor(QColor Second);
   void DrawBackground();
};


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

typedef struct
{
   vector<TVertex> Vertices;
   vector<TFace> Faces;

   int NumVertex;
   int NumFaces;

   QColor Color;

}  TModel;

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

T3DPoint Vector(T3DPoint Start, T3DPoint Final);

T3DPoint GetPoint(TVector Vector, bool Integer = false);

TVector Multiply(TVector Vector);

TVector PutPoint(T3DPoint Point);

double ScalarProduct(T3DPoint A, T3DPoint B);

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

void DrawSphere(QColor Start, QColor Final);

void FindColor(TScene &Scene);

void Sort(vector<TVertex> &Triangle);

void Bresenham(vector<vector<int> > &Intersect, int StartX, int StartY, int FinalX, int FinalY, int Status, int ExitY);

void SetTime(TScene &Scene);

#endif // HEADER_H
