#include "object.hpp"
#include "mathlib.h"
#include <iostream>
using namespace std;

Object::Object()
{
  nb_vertices=0;
  nb_faces=0;
  nb_tvertices=0;
  nb_tfaces=0;
  nb_nvertices=0;
  nb_nfaces=0;
  nb_materials=0;

  has_texture=false;
};


// ADD ------------------------------------------------------
int Object::add_vertex(float v1,float v2, float v3)
{
  Vertex v;

  v.coord[0]=v1;
  v.coord[1]=v2;
  v.coord[2]=v3;

  Vertices[nb_vertices]=v;

  nb_vertices++;

  return nb_vertices;
};

int Object::add_material(Material m)
{
  Materials[nb_materials]=m;

  nb_materials++;

  return nb_materials;
};

// Pointe sur 3 vertices enregistrees
int Object::add_face(int a, int b, int c)
{
  Face f;

  f.coord[0]=a;
  f.coord[1]=b;
  f.coord[2]=c;

  Faces[nb_faces]=f;
  nb_faces++;

  return nb_faces++;
};

int Object::add_tvertex(float x,float y)
{
  TVertex v;

  v.coord[0]=x;
  v.coord[1]=y;

  TVertices[nb_tvertices]=v;
  nb_tvertices++;

  return nb_tvertices;
};

int Object::add_nvertex(float x,float y,float z)
{
  NVertex v;

  v.coord[0]=x;
  v.coord[1]=y;
  v.coord[2]=z;

  NVertices[nb_nvertices]=v;
  nb_nvertices++;

  return nb_nvertices;
};


int Object::add_tface(int a, int b, int c)
{
  TFace f;

  f.coord[0]=a;
  f.coord[1]=b;
  f.coord[2]=c;

  TFaces[nb_tfaces]=f;
  nb_tfaces++;

  return nb_tfaces;
};

int Object::add_nface(NFace nf)
{
  NFaces[nb_nfaces]=nf;
  nb_nfaces++;

  return nb_nfaces;
};

// GET ------------------------------------------------------

Vertex Object::get_vertex(int i)
{
  return Vertices[i];
};

Face Object::get_face(int i)
{
  return Faces[i];
};

TFace Object::get_tface(int i)
{
  return TFaces[i];
};

TVertex Object::get_tvertex(int i)
{
  return TVertices[i];
};

Material Object::get_material(int i)
{
  return Materials[i];
};

NFace Object::get_nface(int i)
{
  return NFaces[i];
};

NVertex Object::get_nvertex(int i)
{
  return NVertices[i];
};


// NB ------------------------------------------------------

int Object::get_nb_faces(void)
{
  return Faces.size();
};

int Object::get_nb_vertices(void)
{
  return Vertices.size();
};

int Object::get_nb_tvertices(void)
{
  return TVertices.size();
};

int Object::get_nb_nfaces(void)
{
  return NFaces.size();
};

int Object::get_nb_nvertices(void)
{
  return NVertices.size();
};

int Object::get_nb_tfaces(void)
{
  return TFaces.size();
};

int Object::get_nb_materials(void)
{
  return Materials.size();
};

// ADD ------------------------------------------------------

void Object::test(void)
{
  // exemple d'utilisation

  // ajout de vertices
  //  add_vertex(0.5f,1.5f,2.5f);
  //  add_vertex(3.5f,4.5f,5.5f);
  //  add_vertex(6.5f,7.5f,8.5f);

  // ajout de faces
  //  add_face(1,2,3);

  // stats
  cout<<"Nb Vertices: "<<nb_vertices<<"\n";
  cout<<"Nb Faces: "<<nb_faces<<"\n";

  // parcourir les vertices
  int i;
  map<const int,Vertex>::iterator it=Vertices.begin();

  for (i=0;i<nb_vertices;i++)
    {
      cout<<"Vertex "<<i<<": "<<(*it).second.coord[0]<<";"<<(*it).second.coord[1]<<";"<<(*it).second.coord[2]<<"\n";
      it++;
    }
};

bool Object::toggleTexture(void)
{
  has_texture=!has_texture;
  return has_texture;
};

bool Object::get_has_texture()
{
  return has_texture;
};

void Object::compute_normals()
{
  cout<<"\n** Calcul des normales\n";

  Face f;
  Vertex v1,v2,v3;
  float out[3];
  NFace nf;
  int i,j=get_nb_faces();
  map<const int,Face>::iterator it2=Faces.begin();
  
  cout<<"Nb Faces: "<<j<<"\n";  

  for (i=0;i<j;i++)
    {
      //cout<<"face "<<i<<": "<<(*it2).second.coord[0]<<";"<<(*it2).second.coord[1]<<";"<<(*it2).second.coord[2]<<"\n";

      v1=get_vertex((*it2).second.coord[0]);
      v2=get_vertex((*it2).second.coord[1]);
      v3=get_vertex((*it2).second.coord[2]);

      //cout<<i<<": "<<v1.coord[0]<<" "<<v1.coord[1]<<" "<<v1.coord[2]<<"\n";
      //cout<<i<<": "<<v2.coord[0]<<" "<<v2.coord[1]<<" "<<v2.coord[2]<<"\n";
      //cout<<i<<": "<<v3.coord[0]<<" "<<v3.coord[1]<<" "<<v3.coord[2]<<"\n";

      calcNormal(v1.coord,v2.coord,v3.coord,out);

      nf.coord[0]=out[0];
      nf.coord[1]=out[1];
      nf.coord[2]=out[2];

      //cout<<i<<": "<<nf.coord[0]<<" "<<nf.coord[1]<<" "<<nf.coord[2]<<"\n";

      add_nface(nf);

      it2++;
    }

  cout<<"Added "<<get_nb_nfaces()<<" normals faces\n\n";
};
