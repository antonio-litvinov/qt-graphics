#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"

double DegToRad(double degrees);

double RadToDeg(double radians);

T3DPoint Vector(T3DPoint Start, T3DPoint Final);

double ScalarProduct(T3DPoint A, T3DPoint B);

T3DPoint GetPoint(TVector Vector, bool Integer = false);

TVector PutPoint(T3DPoint Point);

void Sort(vector<TVertex> &Triangle);

void Bresenham(vector<vector<int> > &Intersect, int StartX, int StartY, int FinalX, int FinalY, int Status, int ExitY);

#endif // FUNCTIONS_H
