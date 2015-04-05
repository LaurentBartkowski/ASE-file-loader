#ifndef object_hpp
#define object_hpp

#include "primitives.h"
#include "mathlib.h"

#include <map>
using namespace std;

class Object {

public:
  

  Object();

  int nb_vertices;
  map<const int, Vertex> Vertices;

  int nb_faces;
  map<const int, Face> Faces;

  int nb_tvertices;
  map<const int, TVertex> TVertices;

  int nb_tfaces;
  map<const int, TFace> TFaces;

  int add_vertex(float v1,float v2,float v3);
  int add_face(int a, int b, int c);
  int add_tvertex(float x,float y);
  int add_tface(int a,int b,int c);
  int add_material(Material m);
  int add_nvertex(float x,float y,float z);
  int add_nface(NFace nf);
  
  Vertex get_vertex(int i);
  Face get_face(int i);
  TVertex get_tvertex(int i);
  TFace get_tface(int i);
  NVertex get_nvertex(int i);
  NFace get_nface(int i);
  Material get_material(int i);

  int get_nb_faces(void);
  int get_nb_vertices(void);
  int get_nb_tvertices(void);
  int get_nb_tfaces(void);
  int get_nb_nvertices(void);
  int get_nb_nfaces(void);
  int get_nb_materials(void);

  void test(void);

  bool toggleTexture();
  bool get_has_texture();

  void compute_normals();

private:
  int nb_nvertices;
  int nb_nfaces;
  int nb_materials;
  bool has_texture;
  
  map<const int, NVertex> NVertices;
  map<const int, NFace> NFaces;
  map<const int, Material> Materials;
};

#endif
