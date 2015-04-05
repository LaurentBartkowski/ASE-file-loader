#include "mathlib.h"

#include <math.h>

void ReduceToUnit(float vector[3])
{
  float length;

  length = (float)sqrt((vector[0]*vector[0]) + (vector[1]*vector[1]) + (vector[2]*vector[2]));
  
  if(length == 0.0f)
    length = 1.0f;
  
  vector[0] /= length;
  vector[1] /= length;
  vector[2] /= length;
}

// Normale à une face
void calcNormal(float v1[3],float v2[3],float v3[3], float out[3])
{
  float tmp1[3],tmp2[3];
  static const int x = 0;
  static const int y = 1;
  static const int z = 2;
  
  tmp1[x] = v1[x] - v2[x];
  tmp1[y] = v1[y] - v2[y];
  tmp1[z] = v1[z] - v2[z];

  tmp2[x] = v2[x] - v3[x];
  tmp2[y] = v2[y] - v3[y];
  tmp2[z] = v2[z] - v3[z];

  out[x] = tmp1[y]*tmp2[z] - tmp1[z]*tmp2[y];
  out[y] = tmp1[z]*tmp2[x] - tmp1[x]*tmp2[z];
  out[z] = tmp1[x]*tmp2[y] - tmp1[y]*tmp2[x];  

  ReduceToUnit(out);
}

