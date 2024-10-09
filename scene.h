#ifndef SCENE_H
#define SCENE_H

#include "types.h"
#include "picture.h"

class TScene
{
public:
    TScene();

    QString FileName;

    vector <TModel> Models;
    vector <TLight> Lights;

    TCamera Camera;

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

    TPicture Picture;

    void Load(QString FileName);
    void InitScene();
    void Projection();
    void FindColor();
    void SetTime();
    void Draw();

};

#endif // SCENE_H
