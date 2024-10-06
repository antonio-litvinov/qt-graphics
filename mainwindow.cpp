#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      drawingWidget(new DrawingWidget(this))
{
    ui->setupUi(this);

    QWidget *placeholder = ui->widgetMain->findChild<QWidget*>("widgetDraw");
    if (placeholder) {
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(drawingWidget);
        placeholder->setLayout(layout);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

TPicture::TPicture() {}

TPicture::TPicture(int w, int h)
{
   Width = w;
   Height = h;

   CenterX = Width / 2;
   CenterY = Height / 2;

   FirstBackColor = Qt::red;
   SecondBackColor = Qt::blue;

   MyCanvas = vector<vector<QColor> >(Width, vector<QColor>(Height));
}

QColor TPicture::GetColor(int x, int y)
{
   return MyCanvas[x][y];
}

void TPicture::SetColor(int x, int y, QColor clr)
{
   MyCanvas[x][y] = clr;
}

int TPicture::GetWidth()
{
   return Width;
}

int TPicture::GetHeight()
{
   return Height;
}

int TPicture::XC()
{
   return CenterX;
}

int TPicture::YC()
{
   return CenterY;
}

void TPicture::SetFirstBackColor(QColor First)
{
   FirstBackColor = First;
}

void TPicture::SetSecondBackColor(QColor Second)
{
   SecondBackColor = Second;
}

void TPicture::DrawBackground()
{
   QColor Color;
   double R2, r2, K;

   int R = Width * 0.6;

   QColor teal(0, 128, 128);

   SecondBackColor = Qt::black;
   FirstBackColor = teal;

   R2 = R * R;

   for(int X = 0; X < Width; X++)
      for(int Y = 0; Y < Height; Y++)
          MyCanvas[X][Y] = FirstBackColor;
   for(int X = 0; X <= R; X++)
      for(int Y = 0; Y <= X; Y++)
      {
            r2 = X * X + Y * Y;

            if( r2 > R2 ) break;

            K = 1 - r2 / R2;


            int R = (int)(K * SecondBackColor.red() + (1 - K) * FirstBackColor.red());
            int G = (int)(K * SecondBackColor.green() + (1 - K) * FirstBackColor.green());
            int B = (int)(K * SecondBackColor.blue() + (1 - K) * FirstBackColor.blue());


            Color = QColor::fromRgb(R, G, B);

            if ((CenterX + X) < Width && (CenterY + Y) < Height)
                MyCanvas[CenterX + X][CenterY + Y] = Color;

            if ((CenterX + X) < Width && (CenterY - Y) >= 0)
                MyCanvas[CenterX + X][CenterY - Y] = Color;

            if ((CenterY + Y) < Height && (CenterX - X) >= 0)
                MyCanvas[CenterX - X][CenterY + Y] = Color;

            if ((CenterX - X) >= 0 && (CenterY - Y) >= 0)
                MyCanvas[CenterX - X][CenterY - Y] = Color;

            if ((CenterX + Y) < Width && (CenterY + X) < Height)
                MyCanvas[CenterX + Y][CenterY + X] = Color;

            if ((CenterX + Y) < Width && (CenterY - X) >= 0)
                MyCanvas[CenterX + Y][CenterY - X] = Color;

            if ((CenterY + X) < Height && (CenterX - Y) >= 0)
                MyCanvas[CenterX - Y][CenterY + X] = Color;

            if ((CenterX - Y) >= 0 && (CenterY - X) >= 0)
                MyCanvas[CenterX - Y][CenterY - X] = Color;

         }
}

TScene Scene;
TPicture MyPicture;

double Transform[4][4];

vector <vector <int> > Buffer;

double Length(T3DPoint Vector)
{
   return sqrt(Vector.X * Vector.X + Vector.Y * Vector.Y + Vector.Z * Vector.Z);
}

T3DPoint Vector(T3DPoint Start, T3DPoint Final)
{
   T3DPoint Result;

   Result.X = Final.X - Start.X;
   Result.Y = Final.Y - Start.Y;
   Result.Z = Final.Z - Start.Z;

   double L = Length(Result);

   Result.X /= L;
   Result.Y /= L;
   Result.Z /= L;

   return Result;
}

double ScalarProduct(T3DPoint A, T3DPoint B)
{
   return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

TVector Multiply(TVector Vector)
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

double DegToRad(double degrees) {
    return degrees * M_PI / 180.0;
}

double RadToDeg(double radians) {
    return radians * 180.0 / M_PI;
}

void Clear()
{
   for (int I = 0; I < 4; I++)
      for (int J = 0; J < 4; J++)
         Transform[I][J] = 0;
}

void InitMove(double dx, double dy, double dz)
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

void InitScale(double kx, double ky, double kz)
{
   Clear();
   Transform[0][0] = kx;
   Transform[1][1] = ky;
   Transform[2][2] = kz;
   Transform[3][3] = 1;
}

void InitRotate(double Angle, char Axis)
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

void InitTransform(double Alpha, double Beta)
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

T3DPoint GetPoint(TVector Vector, bool Integer)
{
   T3DPoint Result;

   if (Integer)
      {
         Result.X = (int)Vector[0];
         Result.Y = (int)Vector[1];
         Result.Z = (int)Vector[2];
      }
   else
      {
         Result.X = Vector[0];
         Result.Y = Vector[1];
         Result.Z = Vector[2];
      }

   return Result;
}

TVector PutPoint(T3DPoint Point)
{
   TVector Result;

   Result.push_back(Point.X);
   Result.push_back(Point.Y);
   Result.push_back(Point.Z);
   Result.push_back(1);

   return Result;
}

void Projection(TScene &Scene)
{
   TVector xyz,XYZ;

   InitTransform(Scene.Camera.Alpha,Scene.Camera.Beta);

   for (unsigned Index = 0; Index < Scene.Models.size(); Index++)
      {
         for (int I = 0; I < Scene.Models[Index].NumVertex; I++)
            {
               xyz = PutPoint(Scene.Models[Index].Vertices[I].Point);
               XYZ = Multiply(xyz);
               Scene.Models[Index].Vertices[I].Point = GetPoint(XYZ, true);
            }
      }
}

void Move(TModel &Model, double DX, double DY, double DZ)
{
   TVector xyz,XYZ;

   InitMove(DX, DY, DZ);

   for (int I = 0; I < Model.NumVertex; I++)
      {
         xyz = PutPoint(Model.Vertices[I].Point);
         XYZ = Multiply(xyz);
         Model.Vertices[I].Point = GetPoint(XYZ);
      }
}

void Scale(TModel &Model, double KX, double KY, double KZ)
{
   TVector xyz,XYZ;

   InitScale(KX, KY, KZ);

   for (int I = 0; I < Model.NumVertex; I++)
      {
         xyz = PutPoint(Model.Vertices[I].Point);
         XYZ = Multiply(xyz);
         Model.Vertices[I].Point = GetPoint(XYZ);
      }
}

void Rotate(TModel &Model, double X, double Y, double Z)
{
   TVector xyz,XYZ;

   for (int I = 0; I < Model.NumVertex; I++)
      {
         xyz = PutPoint(Model.Vertices[I].Point);

         InitRotate(X, 'X');
         XYZ = Multiply(xyz);
         xyz = XYZ;

         InitRotate(Y, 'Y');
         XYZ = Multiply(xyz);
         xyz = XYZ;

         InitRotate(Z, 'Z');
         XYZ = Multiply(xyz);

         Model.Vertices[I].Point = GetPoint(XYZ);

         xyz = PutPoint(Model.Vertices[I].Normal);

         InitRotate(X, 'X');
         XYZ = Multiply(xyz);
         xyz = XYZ;

         InitRotate(Y, 'Y');
         XYZ = Multiply(xyz);
         xyz = XYZ;

         InitRotate(Z, 'Z');
         XYZ = Multiply(xyz);

         Model.Vertices[I].Normal = GetPoint(XYZ);
      }
}

void FindColor(TScene &Scene)
{
   TModel Model;

   int p = 100;

   double CosA, CosB;
   double tmp, tmp2;

   T3DPoint Light, Reflected;
   T3DPoint Camera, CameraPos;

   CameraPos.X = cos(Scene.Camera.Beta) * cos(Scene.Camera.Alpha);
   CameraPos.Y = cos(Scene.Camera.Beta) * sin(Scene.Camera.Alpha);
   CameraPos.Z = sin(Scene.Camera.Beta);

   int R,G,B;

   for (unsigned Index = 0; Index < Scene.Models.size(); Index++)
      {
         Model = Scene.Models[Index];
         for (int I = 0; I < Model.NumVertex; I++)
            {
               R = Model.Color.red() * Scene.Ka;
               G = Model.Color.green() * Scene.Ka;
               B = Model.Color.blue() * Scene.Ka;

               for (unsigned J = 0; J < Scene.Light.size(); J++)
                  {
                     Light = Vector(Model.Vertices[I].Point, Scene.Light[J].Point);

                     CosA = ScalarProduct(Model.Vertices[I].Normal, Light);

                     Reflected.X = 2 * Model.Vertices[I].Normal.X * CosA - Light.X;
                     Reflected.Y = 2 * Model.Vertices[I].Normal.Y * CosA - Light.Y;
                     Reflected.Z = 2 * Model.Vertices[I].Normal.Z * CosA - Light.Z;

                     Camera = Vector(Model.Vertices[I].Point, CameraPos);
                     CosB = ScalarProduct(Camera,Reflected);

                     if (CosA > 0)
                        {
                           tmp = Scene.Kd * CosA;
                           tmp2 = Scene.Ks * pow(CosB,p);
                           R += Scene.Light[J].Color.red() * tmp +
                           Scene.Light[0].Color.red() * tmp2;

                           G += Scene.Light[J].Color.green() * tmp +
                           Scene.Light[0].Color.green() * tmp2;

                           B += Scene.Light[J].Color.blue() * tmp +
                           Scene.Light[0].Color.blue() * tmp2;
                        }

                     if (R > 255) R = 255;
                     if (G > 255) G = 255;
                     if (B > 255) B = 255;
                  }

               Model.Vertices[I].Color = QColor::fromRgb(R, G, B);
            }
         Scene.Models[Index] = Model;
      }
}

void Sort(vector<TVertex> &Triangle)
{
   TVertex Temp;

   for (int I = 0; I < 3; I++)
     for (int J = 0; J < 3; J++)
        {
            if (Triangle[I].Point.Y < Triangle[J].Point.Y)
                {
                   Temp = Triangle[I];
                   Triangle[I] = Triangle[J];
                   Triangle[J] = Temp;
                }
        }
}

int Sign(float X)
{
   if (X < 0) return -1;
   if (X > 0) return 1;
   return 0;
}

void Bresenham(vector<vector<int> > &Intersect, int StartX, int StartY, int FinalX, int FinalY, int Status, int ExitY)
{
   int e;
   int Change;
   int Temp;
   int DX,DY;
   int X,Y;
   int Sx,Sy;
   int I;

   int Index = 0;

   X = StartX; Y = StartY;

   DX = FinalX - StartX; DY = FinalY - StartY;

   Sx = Sign(DX); Sy = Sign(DY);

   DX = abs(DX); DY = abs(DY);

   if (DX > DY) Change = 0;
   else
      {
         Change = 1;
         Temp = DX;
         DX = DY;
         DY = Temp;
      }

   e = 2 * DY - DX;

   for (I = 1; I <= DX; I++)
      {
         Intersect[Index][Status] = X;

         if (e >= 0)
            {
               if (Change == 0)
                  {
                     Y += Sy;
                     Index++;
                  }
               else
                  X += Sx;

               e -= 2 * DX;
            }

         if (Change == 0)
            X += Sx;
         else
            {
               Y += Sy;
               Index++;
            }

         e += 2 * DY;

         if (Y == ExitY)
            {
               Intersect[Index][Status] = X;
               return;
            }
      }
   Intersect[Index][Status] = X;
}

void SetTime(TScene &Scene)
{
    unsigned short Hour, Min, Sec;
    QTime Time = QTime::currentTime();

    Hour = Time.hour();
    Min = Time.minute();
    Sec = Time.second();

    double HourAngle = Hour * 30 + (double)Min / 60 * 30;
    double MinAngle = Min * 6;
    double SecAngle = Sec * 6;

    Rotate(Scene.Models[Scene.Hour],0,0,-HourAngle);
    Rotate(Scene.Models[Scene.Min], 0,0,-MinAngle);
    Rotate(Scene.Models[Scene.Sec],0,0,-SecAngle);
}

void InitBuffer()
{
   for (unsigned I = 0; I < Buffer.size(); I++)
      for (unsigned J = 0; J < Buffer[I].size(); J++)
         Buffer[I][J] = -1000;
}

void DrawTriangle(TVertex A, TVertex B, TVertex C, vector<vector<int> > Intersect, bool Bottom)
{
   int StartR, StartG, StartB;

   double StartZ;

   int FinalR, FinalG, FinalB;

   double FinalZ;

   int ResultR, ResultG, ResultB;

   double ResultZ;

   int Y = A.Point.Y;

   double K1, K2;

   double tmp, tmp2;

   if (A.Point.X == B.Point.X) K1 = 90;
   else
      K1 = RadToDeg(atan( (B.Point.Y - A.Point.Y) / (B.Point.X - A.Point.X) ));

   if (A.Point.X == C.Point.X) K2 = 90;
   else
      K2 = RadToDeg(atan( (C.Point.Y - A.Point.Y) / (C.Point.X - A.Point.X) ));

   for (unsigned I = 0; I < Intersect.size(); I++)
      {
         if (fabs(K1) <= 45)
            {
               tmp = Intersect[I][0] - A.Point.X;
               tmp2 = B.Point.X - A.Point.X;
               tmp2 = B.Point.X - A.Point.X;
               StartR = A.Color.red() + tmp * (B.Color.red() - A.Color.red()) / tmp2;
               StartG = A.Color.green() + tmp * (B.Color.green() - A.Color.green()) / tmp2;
               StartB = A.Color.blue() + tmp * (B.Color.blue() - A.Color.blue()) / tmp2;

               StartZ = A.Point.Z + tmp * (B.Point.Z - A.Point.Z) / tmp2;
            }
         else
            {
               tmp = Y - A.Point.Y;
               tmp2 = B.Point.Y - A.Point.Y;
               StartR = A.Color.red() + tmp * (B.Color.red() - A.Color.red()) / tmp2;
               StartG = A.Color.green() + tmp * (B.Color.green() - A.Color.green()) / tmp2;
               StartB = A.Color.blue() + tmp * (B.Color.blue() - A.Color.blue()) / tmp2;

               StartZ = A.Point.Z + tmp * (B.Point.Z - A.Point.Z) / tmp2;
            }

         if (fabs(K2) <= 45)
            {
               tmp = Intersect[I][1] - A.Point.X;
               tmp2 = C.Point.X - A.Point.X;
               FinalR = A.Color.red()+ tmp * (C.Color.red() - A.Color.red()) / tmp2;
               FinalG = A.Color.green() + tmp * (C.Color.green() - A.Color.green()) / tmp2;
               FinalB = A.Color.blue() + tmp * (C.Color.blue() - A.Color.blue()) / tmp2;

               FinalZ = A.Point.Z + tmp * (C.Point.Z - A.Point.Z) / tmp2;
            }
         else
            {
               tmp = Y - A.Point.Y;
               tmp2 = C.Point.Y - A.Point.Y;
               FinalR = A.Color.red() + tmp * (C.Color.red() - A.Color.red()) / tmp2;
               FinalG = A.Color.green() + tmp * (C.Color.green() - A.Color.green()) / tmp2;
               FinalB = A.Color.blue() + tmp * (C.Color.blue() - A.Color.blue()) / tmp2;

               FinalZ = A.Point.Z + tmp * (C.Point.Z - A.Point.Z) / tmp2;
            }

         for (int X = Intersect[I][0]; X <= Intersect[I][1]; X++)
            {
               ResultR = StartR;
               ResultG = StartG;
               ResultB = StartB;

               ResultZ = StartZ;

               if (Intersect[I][0] != Intersect[I][1])
                  {
                      tmp = X - Intersect[I][0];
                      tmp2 = Intersect[I][1] - Intersect[I][0];
                      ResultR = StartR + tmp * (FinalR - StartR) / tmp2;
                      ResultG = StartG + tmp * (FinalG - StartG) / tmp2;
                      ResultB = StartB + tmp * (FinalB - StartB) / tmp2;

                      ResultZ = StartZ + tmp * (FinalZ - StartZ) / tmp2;
                  }

               if (((unsigned)(MyPicture.XC() + X) < Buffer.size())  &&
                   ((unsigned)(MyPicture.YC() - Y) < Buffer[0].size()) &&
                   ((MyPicture.XC() + X) > 0) && ((MyPicture.YC() - Y) > 0))
                  if ((ResultZ >= Buffer[MyPicture.XC() + X][MyPicture.YC() - Y]) && (ResultZ >= -1000))
                     {
                        //Canvas->Pixels[XC + X][YC - Y] = (TColor)RGB(ResultR,ResultG,ResultB);
                        MyPicture.SetColor(MyPicture.XC() + X, MyPicture.YC() - Y, QColor::fromRgb(ResultR,ResultG,ResultB));
                        Buffer[MyPicture.XC() + X][MyPicture.YC() - Y] = ResultZ;
                     }
            }

         if (Bottom) Y--;
         else        Y++;
      }
}

void Rasterize(TVertex A, TVertex B, TVertex C)
{
   int dY;
   vector <vector <int> > Intersect;

   vector <TVertex> Temp;

   Temp.push_back(A);
   Temp.push_back(B);
   Temp.push_back(C);

   Sort(Temp);

   A = Temp[0];
   B = Temp[1];
   C = Temp[2];

   double K,X;

   if (A.Point.Y == C.Point.Y) X = A.Point.X;
   else
      {
         K = (A.Point.X - C.Point.X) / (A.Point.Y - C.Point.Y);
         X = C.Point.X + K * (B.Point.Y - C.Point.Y);
      }

   dY = B.Point.Y - A.Point.Y + 1;

   Intersect.resize(dY);
   for (int i = 0; i < dY; i++) Intersect[i].resize(2);

   if (X > B.Point.X)
      {
         if (dY == 1)
            {
               Intersect[0][0] = B.Point.X;
               Intersect[0][1] = A.Point.X;
            }
         else
            {
               Bresenham(Intersect, A.Point.X, A.Point.Y, B.Point.X, B.Point.Y, 0, B.Point.Y);
               Bresenham(Intersect, A.Point.X, A.Point.Y, C.Point.X, C.Point.Y, 1, B.Point.Y);
            }
         DrawTriangle(A, B, C, Intersect, false);
      }
   else
      {
        if (dY == 1)
           {
              Intersect[0][0] = A.Point.X;
              Intersect[0][1] = B.Point.X;
           }
        else
           {
              Bresenham(Intersect, A.Point.X, A.Point.Y, C.Point.X, C.Point.Y, 0, B.Point.Y);
              Bresenham(Intersect, A.Point.X, A.Point.Y, B.Point.X, B.Point.Y, 1, B.Point.Y);
           }
        DrawTriangle(A, C, B, Intersect, false);
      }

   dY = C.Point.Y - B.Point.Y + 1;

   Intersect.resize(dY);
   for (int i = 0; i < dY; i++) Intersect[i].resize(2);

   if (X > B.Point.X)
      {
         if (dY == 1)
            {
               Intersect[0][0] = B.Point.X;
               Intersect[0][1] = C.Point.X;
            }
         else
            {
               Bresenham(Intersect, C.Point.X, C.Point.Y, B.Point.X, B.Point.Y, 0, B.Point.Y);
               Bresenham(Intersect, C.Point.X, C.Point.Y, A.Point.X, A.Point.Y, 1, B.Point.Y);
            }
         DrawTriangle(C, B, A, Intersect, true);
      }
   else
      {
         if (dY == 1)
            {
               Intersect[0][0] = C.Point.X;
               Intersect[0][1] = B.Point.X;
            }
         else
            {
               Bresenham(Intersect, C.Point.X, C.Point.Y, A.Point.X, A.Point.Y, 0, B.Point.Y);
               Bresenham(Intersect, C.Point.X, C.Point.Y, B.Point.X, B.Point.Y, 1, B.Point.Y);
            }
         DrawTriangle(C, A, B, Intersect, true);
      }
}

void DrawModel(TModel Model)
{
   TVertex A,B,C;

   for (int I = 0; I < Model.NumFaces; I++)
      {
         A = Model.Vertices[Model.Faces[I].A];
         B = Model.Vertices[Model.Faces[I].B];
         C = Model.Vertices[Model.Faces[I].C];
         Rasterize(A, B, C);
      }
}

void DrawScene(TScene Scene)
{
  if ((Scene.Clock) && (Scene.Models.size() >= 3)
  && (Scene.Hour != -1) && (Scene.Min != -1) && (Scene.Sec != -1))
     SetTime(Scene);

  for (unsigned Index = 0; Index < Scene.Models.size(); Index++)
     {
        Move(Scene.Models[Index], Scene.Move.X, Scene.Move.Y, Scene.Move.Z);
        Scale(Scene.Models[Index], Scene.Scale.X, Scene.Scale.Y, Scene.Scale.Z);
        Rotate(Scene.Models[Index], Scene.Rotate.X, Scene.Rotate.Y, Scene.Rotate.Z);
     }

  InitBuffer();
  FindColor(Scene);
  Projection(Scene);

  for (unsigned Index = 0; Index < Scene.Models.size(); Index++)
     DrawModel(Scene.Models[Index]);
}

void Load(QString FileName, TScene &Scene)
{
    ifstream File(FileName.toStdString());
    string String;

    TVertex Vertex;
    TFace Face;

    double R,G,B;

    Scene.Models.clear();
    Scene.Light.clear();

    while (File >> String)
    {
        if (String == "*SCENE_LIGHT_POINT")
           {
              Scene.Light.resize(Scene.Light.size() + 1);

              File >> Scene.Light.back().Point.X;
              File >> Scene.Light.back().Point.Y;
              File >> Scene.Light.back().Point.Z;
           }

        if (String == "*SCENE_LIGHT_COLOR")
           {
              File >> R;
              File >> G;
              File >> B;
              Scene.Light.back().Color = QColor::fromRgb(R, G, B);
           }

        if (String == "*SCENE_LIGHT_PARAM")
           {
              File >> Scene.Ka;
              File >> Scene.Kd;
              File >> Scene.Ks;
           }

        if (String == "*SCENE_CAMERA")
           {
              File >> Scene.Camera.Alpha;
              File >> Scene.Camera.Beta;
           }

        if (String == "*SCENE_START")
           {
              File >> R;
              File >> G;
              File >> B;
              //SetFirstBackColor(QColor::fromRgb(R, G, B));
           }

        if (String == "*SCENE_FINAL")
           {
              File >> R;
              File >> G;
              File >> B;
              //MyPicture.SetSecondBackColor(QColor::fromRgb(R, G, B));
           }

        if (String == "*SCENE_CLOCK")
           {
              Scene.Clock = true;
              File >> Scene.Hour;
              File >> Scene.Min;
              File >> Scene.Sec;
           }

        if (String == "*GEOMOBJECT")
           Scene.Models.resize(Scene.Models.size() + 1);

        if (String == "*MESH_NUMVERTEX")
           File >> Scene.Models.back().NumVertex;

        if (String == "*MESH_NUMFACES")
           File >> Scene.Models.back().NumFaces;

        if (String == "*MESH_VERTEX")
           {
              File >> String;
              File >> Vertex.Point.X;
              File >> Vertex.Point.Y;
              File >> Vertex.Point.Z;
              Scene.Models.back().Vertices.push_back(Vertex);
           }

        if (String == "*MESH_FACE")
           {
              File >> String;
              File >> String;
              File >> Face.A;
              File >> String;
              File >> Face.B;
              File >> String;
              File >> Face.C;
              Scene.Models.back().Faces.push_back(Face);
           }

        if (String == "*MESH_VERTEXNORMAL")
           {
              File >> String;

              File >> Scene.Models.back().Vertices[std::stoi(String)].Normal.X;
              File >> Scene.Models.back().Vertices[std::stoi(String)].Normal.Y;
              File >> Scene.Models.back().Vertices[std::stoi(String)].Normal.Z;
           }

        if (String == "*WIREFRAME_COLOR")
           {
              File >> R;
              File >> G;
              File >> B;
              Scene.Models.back().Color = QColor::fromRgb(R * 255, G * 255, B * 255);
           }
    }

    File.close();
}

void InitScene(TScene &Scene)
{
   Scene.Move.X = 0; Scene.Move.Y = 0; Scene.Move.Z = 0;

   Scene.Scale.X = 1; Scene.Scale.Y = 1; Scene.Scale.Z = 1;

   Scene.Rotate.X = 0; Scene.Rotate.Y = 0; Scene.Rotate.Z = 0;

  Scene.Camera.Alpha = 0; Scene.Camera.Beta  = 0;

  Scene.Ka = 0.5; Scene.Kd = 0.5; Scene.Ks = 0.2;

  MyPicture.SetFirstBackColor(Qt::red);
  MyPicture.SetSecondBackColor(Qt::blue);

  Scene.Clock = false;

  Scene.Hour = -1; Scene.Min = -1; Scene.Sec = -1;

  Load(Scene.FileName,Scene);

  if (Scene.Light.size() == 0)
     {
        TLight Light;

        Light.Point.X = 0;
        Light.Point.Y = 0;
        Light.Point.Z = 100;

        Light.Color = Qt::white;

        Scene.Light.push_back(Light);
     }
}

void MainWindow::on_buttonOpen_clicked()
{
    MyPicture = TPicture(ui->widgetDraw->width(), ui->widgetDraw->height());


    QString fileName = QFileDialog::getOpenFileName(nullptr, "Открыть файл", "", "Все файлы (*.*)");

    Scene.FileName = fileName;

    InitScene(Scene);


    Buffer.resize(ui->widgetDraw->width());
    for (int I = 0; I < ui->widgetDraw->width(); I++) Buffer[I].resize(ui->widgetDraw->height());

     MyPicture.DrawBackground();

     DrawScene(Scene);

     drawingWidget->setPicture(MyPicture);

}

