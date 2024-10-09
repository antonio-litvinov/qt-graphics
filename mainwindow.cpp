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


TScene Scene;
TPicture Picture;

double Transform[4][4];




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

  Picture.InitBuffer();

  FindColor(Scene);
  Projection(Scene);

  for (unsigned Index = 0; Index < Scene.Models.size(); Index++)
     Picture.DrawModel(Scene.Models[Index]);
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
              // Set start background color
           }

        if (String == "*SCENE_FINAL")
           {
              File >> R;
              File >> G;
              File >> B;
              // Set final background color
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
    Picture = TPicture(ui->widgetDraw->width(), ui->widgetDraw->height());


    QString fileName = QFileDialog::getOpenFileName(nullptr, "Открыть файл", "", "Все файлы (*.*)");

    Scene.FileName = fileName;

    InitScene(Scene);

    Picture.InitBuffer();
    Picture.DrawBackground();

    DrawScene(Scene);
    drawingWidget->setPicture(Picture);
}

