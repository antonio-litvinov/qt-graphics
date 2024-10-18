#include "picture.h"

TPicture::TPicture() {}

TPicture::TPicture(int w, int h)
{
   Width = w;
   Height = h;

   XC = Width / 2;
   YC = Height / 2;

   Canvas = vector<vector<QColor> >(Width, vector<QColor>(Height));
}

int TPicture::GetWidth()
{
   return Width;
}

int TPicture::GetHeight()
{
   return Height;
}

QColor TPicture::GetColor(int x, int y)
{
   return Canvas[x][y];
}

void TPicture::DrawBackground()
{
   QColor Color;
   double R2, r2, K;

   int R = Width * 0.6;

   QColor teal(0, 70, 70);

   QColor Start = teal;
   QColor Final = Qt::black;

   R2 = R * R;

   for(int X = 0; X < Width; X++)
      for(int Y = 0; Y < Height; Y++)
          Canvas[X][Y] = Start;
   for(int X = 0; X <= R; X++)
      for(int Y = 0; Y <= X; Y++)
      {
            r2 = X * X + Y * Y;

            if( r2 > R2 ) break;

            K = 1 - r2 / R2;


            int R = (int)(K * Final.red() + (1 - K) * Start.red());
            int G = (int)(K * Final.green() + (1 - K) * Start.green());
            int B = (int)(K * Final.blue() + (1 - K) * Start.blue());


            Color = QColor::fromRgb(R, G, B);

            if ((XC + X) < Width && (YC + Y) < Height)
                Canvas[XC + X][YC + Y] = Color;

            if ((XC + X) < Width && (YC - Y) >= 0)
                Canvas[XC + X][YC - Y] = Color;

            if ((YC + Y) < Height && (XC - X) >= 0)
                Canvas[XC - X][YC + Y] = Color;

            if ((XC - X) >= 0 && (YC - Y) >= 0)
                Canvas[XC - X][YC - Y] = Color;

            if ((XC + Y) < Width && (YC + X) < Height)
                Canvas[XC + Y][YC + X] = Color;

            if ((XC + Y) < Width && (YC - X) >= 0)
                Canvas[XC + Y][YC - X] = Color;

            if ((YC + X) < Height && (XC - Y) >= 0)
                Canvas[XC - Y][YC + X] = Color;

            if ((XC - Y) >= 0 && (YC - X) >= 0)
                Canvas[XC - Y][YC - X] = Color;

         }
}

void TPicture::InitBuffer()
{
    Buffer.resize(Width);
    for (int I = 0; I < Width; I++) Buffer[I].resize(Height);

    for (int I = 0; I < Width; I++)
       for (unsigned J = 0; J < Height; J++)
          Buffer[I][J] = -1000;
}

void TPicture::DrawTriangle(TVertex A, TVertex B, TVertex C, vector<vector<int> > Intersect, bool Bottom)
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

               if ((XC + X < Width)  &&
                   (YC - Y < Height) &&
                   (XC + X > 0) && (YC - Y > 0))
                  if (ResultZ >= Buffer[XC + X][YC - Y] && (ResultZ >= -1000))
                     {
                        Canvas[XC + X][YC - Y] = QColor::fromRgb(ResultR,ResultG,ResultB);
                        Buffer[XC + X][YC - Y] = ResultZ;
                     }
            }

         if (Bottom) Y--;
         else        Y++;
      }
}

void TPicture::Rasterize(TVertex A, TVertex B, TVertex C)
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

void TPicture::DrawModel(TModel Model)
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
