#include "scene.h"
#include <fstream>
#include <QTime>

TScene::TScene()
{

}

void TScene::Projection()
{
    TVector xyz,XYZ;

    for (unsigned Index = 0; Index < Models.size(); Index++)
    {
        Models[Index].InitTransform(Camera.Alpha, Camera.Beta);

        for (int I = 0; I < Models[Index].NumVertex; I++)
        {
            xyz = PutPoint(Models[Index].Vertices[I].Point);
            XYZ = Models[Index].Multiply(xyz);
            Models[Index].Vertices[I].Point = GetPoint(XYZ, true);
        }
    }
}

void TScene::FindColor()
{
    TModel Model;

    int p = 100;

    double CosA, CosB;
    double tmp, tmp2;

    T3DPoint Light, Reflected;
    T3DPoint TempCamera, CameraPos;

    CameraPos.X = cos(Camera.Beta) * cos(Camera.Alpha);
    CameraPos.Y = cos(Camera.Beta) * sin(Camera.Alpha);
    CameraPos.Z = sin(Camera.Beta);

    int R,G,B;

    for (unsigned Index = 0; Index < Models.size(); Index++)
    {
        Model = Models[Index];
        for (int I = 0; I < Model.NumVertex; I++)
        {
            R = Model.Color.red() * Ka;
            G = Model.Color.green() * Ka;
            B = Model.Color.blue() * Ka;

            for (unsigned J = 0; J < Lights.size(); J++)
            {
                Light = Vector(Model.Vertices[I].Point, Lights[J].Point);

                CosA = ScalarProduct(Model.Vertices[I].Normal, Light);

                Reflected.X = 2 * Model.Vertices[I].Normal.X * CosA - Light.X;
                Reflected.Y = 2 * Model.Vertices[I].Normal.Y * CosA - Light.Y;
                Reflected.Z = 2 * Model.Vertices[I].Normal.Z * CosA - Light.Z;

                TempCamera = Vector(Model.Vertices[I].Point, CameraPos);
                CosB = ScalarProduct(TempCamera,Reflected);

                if (CosA > 0)
                {
                    tmp = Kd * CosA;
                    tmp2 = Ks * pow(CosB,p);
                    R += Lights[J].Color.red() * tmp +
                         Lights[0].Color.red() * tmp2;

                    G += Lights[J].Color.green() * tmp +
                         Lights[0].Color.green() * tmp2;

                    B += Lights[J].Color.blue() * tmp +
                         Lights[0].Color.blue() * tmp2;
                }

                if (R > 255) R = 255;
                if (G > 255) G = 255;
                if (B > 255) B = 255;
            }

            Model.Vertices[I].Color = QColor::fromRgb(R, G, B);
        }
        Models[Index] = Model;
    }
}

void TScene::SetTime()
{
    QTime Time = QTime::currentTime();

    double HourAngle = Time.hour() * 30 + Time.minute() / 60 * 30;
    double MinAngle = Time.minute() * 6;
    double SecAngle = Time.second() * 6;

    Models[Hour].Rotate(0,0,-HourAngle);
    Models[Min].Rotate(0,0,-MinAngle);
    Models[Sec].Rotate(0,0,-SecAngle);
}


void TScene::Draw()
{
    if ((Clock) && (Models.size() >= 3)
        && (Hour != -1) && (Min != -1) && (Sec != -1))
        SetTime();

    for (unsigned Index = 0; Index < Models.size(); Index++)
    {
        Models[Index].Move(Move.X, Move.Y, Move.Z);
        Models[Index].Scale(Scale.X, Scale.Y, Scale.Z);
        Models[Index].Rotate(Rotate.X, Rotate.Y, Rotate.Z);
    }

    Picture.InitBuffer();

    FindColor();
    Projection();

    for (unsigned Index = 0; Index < Models.size(); Index++)
        Picture.DrawModel(Models[Index]);
}

void TScene::Load(QString FileName)
{
    ifstream File(FileName.toStdString());
    string String;

    TVertex Vertex;
    TFace Face;

    double R,G,B;

    Models.clear();
    Lights.clear();

    while (File >> String)
    {
        if (String == "*SCENE_LIGHT_POINT")
        {
            Lights.resize(Lights.size() + 1);

            File >> Lights.back().Point.X;
            File >> Lights.back().Point.Y;
            File >> Lights.back().Point.Z;
        }

        if (String == "*SCENE_LIGHT_COLOR")
        {
            File >> R;
            File >> G;
            File >> B;
            Lights.back().Color = QColor::fromRgb(R, G, B);
        }

        if (String == "*SCENE_LIGHT_PARAM")
        {
            File >> Ka;
            File >> Kd;
            File >> Ks;
        }

        if (String == "*SCENE_CAMERA")
        {
            File >> Camera.Alpha;
            File >> Camera.Beta;
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
            Clock = true;
            File >> Hour;
            File >> Min;
            File >> Sec;
        }

        if (String == "*GEOMOBJECT")
            Models.resize(Models.size() + 1);

        if (String == "*MESH_NUMVERTEX")
            File >> Models.back().NumVertex;

        if (String == "*MESH_NUMFACES")
            File >> Models.back().NumFaces;

        if (String == "*MESH_VERTEX")
        {
            File >> String;
            File >> Vertex.Point.X;
            File >> Vertex.Point.Y;
            File >> Vertex.Point.Z;
            Models.back().Vertices.push_back(Vertex);
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
            Models.back().Faces.push_back(Face);
        }

        if (String == "*MESH_VERTEXNORMAL")
        {
            File >> String;

            File >> Models.back().Vertices[std::stoi(String)].Normal.X;
            File >> Models.back().Vertices[std::stoi(String)].Normal.Y;
            File >> Models.back().Vertices[std::stoi(String)].Normal.Z;
        }

        if (String == "*WIREFRAME_COLOR")
        {
            File >> R;
            File >> G;
            File >> B;
            Models.back().Color = QColor::fromRgb(R * 255, G * 255, B * 255);
        }
    }

    File.close();
}

void TScene::InitScene()
{
    Move.X = 0;
    Move.Y = 0;
    Move.Z = 0;

    Scale.X = 1;
    Scale.Y = 1;
    Scale.Z = 1;

    Rotate.X = 0;
    Rotate.Y = 0;
    Rotate.Z = 0;

    Camera.Alpha = 0;
    Camera.Beta  = 0;

    Ka = 0.5;
    Kd = 0.5; Ks = 0.2;

    Clock = false;

    Hour = -1;
    Min = -1;
    Sec = -1;

    Load(FileName);

    if (Lights.size() == 0)
    {
        TLight Light;

        Light.Point.X = 0;
        Light.Point.Y = 0;
        Light.Point.Z = 100;

        Light.Color = Qt::white;

        Lights.push_back(Light);
    }
}
