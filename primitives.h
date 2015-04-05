#ifndef primitives_h
#define primitives_h

#include <string>
using namespace std;

typedef struct {
  float coord[3];
} Vertex;

typedef struct {
  int coord[3];
} Face;

typedef struct {
  float coord[3];
} NVertex;

typedef struct {
  float coord[3];
  float ncoord[3];
} NFace;

typedef struct {
  float coord[2];
} TVertex;

typedef struct {
  int coord[3];
} TFace;

typedef struct {
  float utiling;
  float vtiling;
  float cdiffuse[3];
  string name;
  string texturefile;
} Material;


#endif
