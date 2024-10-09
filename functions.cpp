#include "functions.h"

double DegToRad(double degrees) {
    return degrees * M_PI / 180.0;
}

double RadToDeg(double radians) {
    return radians * 180.0 / M_PI;
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
