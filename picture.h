#ifndef PICTURE_H
#define PICTURE_H

#include "types.h"
#include "model.h"
#include "functions.h"

class TPicture
{
private:
   int Width;
   int Height;

   int XC;
   int YC;

   vector <vector <int> > Buffer;
   vector<vector<QColor> > Canvas;

public:
   TPicture();
   TPicture(int w, int h);

   int GetWidth();
   int GetHeight();

   QColor GetColor(int x, int y);

   void InitBuffer();

   void DrawBackground();

   void DrawModel(TModel Model);

   void Rasterize(TVertex A, TVertex B, TVertex C);

   void DrawTriangle(TVertex A, TVertex B, TVertex C, vector<vector<int> > Intersect, bool Bottom);
};

#endif // PICTURE_H
