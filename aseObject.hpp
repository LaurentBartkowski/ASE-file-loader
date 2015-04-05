#ifndef aseObject_h
#define aseObject_h

#include "undeformableObject.hpp"

#include <fstream>
#include <string>

#define OBJECT          "*GEOMOBJECT"
#define NUM_VERTEX      "*MESH_NUMVERTEX"
#define NUM_FACES       "*MESH_NUMFACES"
#define NUM_TVERTEX     "*MESH_NUMTVERTEX"
#define NUM_TFACES      "*MESH_NUMTVFACES"
#define VERTEX          "*MESH_VERTEX"
#define FACE            "*MESH_FACE"
#define NORMALS         "*MESH_NORMALS"
#define FACE_NORMAL     "*MESH_FACENORMAL"
#define NVERTEX         "*MESH_VERTEXNORMAL"
#define TVERTEX         "*MESH_TVERT"
#define TFACE           "*MESH_TFACE"
#define TEXTURE         "*BITMAP"
#define UTILE           "*UVW_U_TILING"
#define VTILE           "*UVW_V_TILING"
#define UOFFSET         "*UVW_U_OFFSET"
#define VOFFSET         "*UVW_V_OFFSET"
#define MATERIAL_ID     "*MATERIAL_REF"
#define MATERIAL_COUNT  "*MATERIAL_COUNT"
#define MATERIAL        "*MATERIAL"
#define MATERIAL_NAME   "*MATERIAL_NAME"
#define MATERIAL_COLOR  "*MATERIAL_DIFFUSE"

class aseObject:public undeformableObject
{
public:
  aseObject(const char *file);
  ~aseObject(void);
  
  void test(void);

private:
  ifstream asefile;
  int numObjects;
  int numMaterials;
  int numVertices;
  int numFaces;
  int numTVertices;
  int numTFaces;
  int materialRef;

  // operations relative aux fichiers
  int rechercher_nb_occurences(const char *pattern);
  void rechercher_occurence(const char *pattern);
  float readFloat(void);
  string readString(void);

  // operations relatives aux informations ASE
  int fgetNumObjects(void);
  int fgetNumMaterials(void);
  void moveToObject(int numObject);
  void moveToMaterial(int numMaterial);

  // lecture des objets
  void readObject(int numObject);
  void readData(string attributeId, int numObject);
  void readVertices();
  void readFaces();
  void readTVertices();
  void readTFaces();
  void readMaterial(int numMaterial);
};

#endif
