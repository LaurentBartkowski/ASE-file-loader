#include "undeformableObject.hpp"
#include "aseObject.hpp"
#include "primitives.h"

#include <iostream>
#include <fstream>

#include <string>

using namespace std;

// Constructeurs ----------------------------------------------------------------
//

aseObject::aseObject(const char *file):undeformableObject()
{
  cout<<"*** Init mesh from ASE file "<<file<<"\n";
    
  asefile.open(file, ios::in);
  if (asefile.fail())
    cout<<"!!! Erreur à l'ouverture du fichier "<<file<<"\n";
  
  numObjects=0;
  numMaterials=0;
  numVertices=0;
  numFaces=0;
  numTVertices=0;
  numTFaces=0;
  
  fgetNumObjects();
  fgetNumMaterials();

  readMaterial(1);

  readObject(1);

  compute_normals();
};

aseObject::~aseObject(void)
{
    asefile.close();
};

// Operations sur fichiers ------------------------------------------------------
//

int aseObject::rechercher_nb_occurences(const char *pattern)
{
    // recherche d'une chaine dans le fichier
    // cout << "recherche de "<<pattern<<"\n";
    char c;
    int i=0,j=0;
    char p[255];

    memset(p,'\0',255);
    
    // on 'rembobine' le fichier 
    if (!asefile.seekg(0,ifstream::beg))
        cout<<"!!! flux non repositionnable!\n";
   
    while (!asefile.eof())
    {
      asefile>>p;
      string s(p);

      if (s.compare(pattern)==0)
	j++;
    }
    
    // pour repositionner le eof a false (bit d'erreur),
    // pour pouvoir continuer
    // a faire des operations sur le ficher
    asefile.clear();
    
    return j;
};

void aseObject::rechercher_occurence(const char *pattern)
{
    // recherche d'une chaine dans le fichier
    // cout << "recherche et positionnement apres "<<pattern<<"\n";
    string s;    
    
    do {
        asefile>>s;
    } while (s.compare(pattern)!=0);    
    
    // pour repositionner le eof a false (bit d'erreur),
    // pour pouvoir continuer
    // a faire des operations sur le ficher
    if (asefile.eof())
    {
        asefile.clear();
    }
};

float aseObject::readFloat(void)
{
  float f=0.0f;
  
  asefile>>f;
  
  return f;
};

string aseObject::readString(void)
{

  string s;
  asefile>>s;
  
  return s;
};

// Operations lse objects contenus dans les fichiers  ASE -------------------------------------------------
//

int aseObject::fgetNumObjects(void)
{
    numObjects=rechercher_nb_occurences(OBJECT);
    cout<<"* "<<numObjects<<"objets "<<OBJECT<<" dans le fichier\n";
};


void aseObject::moveToObject(int numObject)
{
  // on 'rembobine' le fichier 
  if (!asefile.seekg(0,ifstream::beg))
    cout<<"!!! flux non repositionnable!\n";

  // on se deplace a l'endroit du fichier qui contient l'objet désiré
  int no=0;
  
  while (no!=numObject)
    {
      rechercher_occurence(OBJECT);
      no++;	  
    }
};

int aseObject::fgetNumMaterials(void)
{
  numMaterials=rechercher_nb_occurences(MATERIAL);
  cout<<"* "<<numMaterials<<" objets "<<MATERIAL<<" dans le fichier\n";
};

void aseObject::moveToMaterial(int numMaterial)
{
  // on 'rembobine' le fichier 
  if (!asefile.seekg(0,ifstream::beg))
    cout<<"!!! flux non repositionnable!\n";

  // on se deplace a l'endroit du fichier qui contient l'objet désiré
  int no=0;
  
  while (no!=numMaterial)
    {
      rechercher_occurence(MATERIAL);
      no++;	  
    }
};


// Lecture des fichiers ASE proprement dits--------------------------------------
//

void aseObject::readObject(int numObject)
{
  cout<<"\nLecture de l'objet "<<numObject<<"\n";

  moveToObject(numObject);

  // recherche par mot clef puis lecture de la valeur
  while (!asefile.eof())
    {
      string keyword(readString());
      //cout<<"Keyword read: "<<keyword<<"\n";
      
      if (keyword.compare(NUM_VERTEX)==0)
	asefile>>numVertices;
      
      else if (keyword.compare(NUM_FACES)==0)
	asefile>>numFaces;
      
      else if (keyword.compare(NUM_TVERTEX)==0)
	asefile>>numTVertices;
      
      else if (keyword.compare(OBJECT)==0)
	return;
    }
  
  if (asefile.eof())
    {
      asefile.clear();
    }
  
  numTFaces=rechercher_nb_occurences(TFACE);
  
  cout<<"\tNombre de Vertices: "<<numVertices<<"\n";
  cout<<"\tNombre de Faces: "<<numFaces<<"\n";
  cout<<"\tNombre de TVertices: "<<numTVertices<<"\n";
  cout<<"\tNombre de TFaces: "<<numTFaces<<"\n";
  
  if (asefile.eof())
    {
      asefile.clear();
    }
  
  readData(MATERIAL_ID,numObject);
  readData(VERTEX,numObject);
  readData(FACE,numObject);
  readData(TVERTEX,numObject);
  readData(TFACE,numObject);
};

void aseObject::readData(string attributeId, int numObject)
{
  moveToObject(numObject);
  
  while (!asefile.eof())
    {
      string keyword(readString());

      if (keyword.compare(OBJECT)==0)
	      return;

      else if (keyword.compare(MATERIAL_ID)==0)
	    {
	  	      if (attributeId.compare(MATERIAL_ID)==0)
	    	  	   {
	      	        asefile>>materialRef;
	      	        cout <<"\tMaterial ref: "<<materialRef<<"\n";
	      	      	return;
	    	       }
	    }
      else if (keyword.compare(VERTEX)==0)
      {
            if (attributeId.compare(VERTEX)==0)
            {
            readVertices();
            return;
            }
      }
      else if (keyword.compare(FACE)==0)
	    {
	         if (attributeId.compare(FACE)==0)
	         {
	         readFaces();
	         return;
	         }
	    }
	    else if (keyword.compare(TVERTEX)==0)
	    {
	         if (attributeId.compare(TVERTEX)==0)
	         {
	         readTVertices();
	         return;
	         }
	    }
      else if (keyword.compare(TFACE)==0)
      {
            if (attributeId.compare(TFACE)==0)
            {
            readTFaces();
            return;
            }
       }
    }

   if (asefile.eof())
    {
      asefile.clear();
    }
};

void aseObject::readTFaces()
{
  string s;
  int n,n1,n2,n3;

  for (int i=0;i<numTFaces;i++)
  {
    asefile>>n;
    asefile>>n1;
    asefile>>n2;
    asefile>>n3;
    asefile>>s;

    add_tface(n1,n2,n3);
  }

  cout<<"\tAdded "<<get_nb_tfaces()<<" TFace\n";

  if (get_nb_tfaces()!=numTFaces)
    {
        cout<<"!!! Erreur dans le fichier\n";
        exit(1);
    }

   if (asefile.eof())
    {
      asefile.clear();
    }

};

void aseObject::readTVertices()
{
  string s;
  int n;
  float f1,f2;

  if (!get_has_texture()) toggleTexture();

  Material m=get_material(materialRef);

  for (int i=0;i<numTVertices;i++)
  {
    asefile>>n;
    asefile>>f1;
    asefile>>f2;
    asefile>>s;
    asefile>>s;
    
    add_tvertex(f1*m.utiling,f2*m.vtiling);
  }

  cout<<"\tAdded "<<get_nb_tvertices()<<" TVertices\n";

  if (get_nb_tvertices()!=numTVertices)
    {
        cout<<"!!! Erreur dans le fichier\n";
        exit(1);
    }

   if (asefile.eof())
    {
      asefile.clear();
    }

};

void aseObject::readFaces()
{
  int n1,n2,n3;
  string s;
    
  do
  {
    asefile>>s;

    if (s.compare("A:")==0) 
        asefile>>n1;

    if(s.compare("B:")==0)
        asefile>>n2;

    if(s.compare("C:")==0) 
        {
                asefile>>n3;
                add_face(n1,n2,n3);
                //cout<<"n1: "<<n1<<" n2: "<<n2<<" n3: "<<n3<<"\n";
        }

    } while (s.compare("}")!=0);
    
  cout<<"\tAdded "<<get_nb_faces()<<" Nb Faces\n";

  if (get_nb_faces()!=numFaces)
    {
        cout<<"!!! Erreur dans le fichier\n";
        exit(1);
    }

   if (asefile.eof())
    {
      asefile.clear();
    }

};

void aseObject::readVertices()
{
  int n;
  float f1,f2,f3;
  string s;

  for (int i=0;i<numVertices;i++)
    {
      asefile>>n;
      asefile>>f1;
      asefile>>f2;
      asefile>>f3;
      asefile>>s;

      add_vertex(f1,f3,-f2);
      //cout<<"f1: "<<f1<<" f2: "<<f2<<" f3: "<<f3<<"\n";
    }
  cout<<"\tAdded "<<get_nb_vertices()<<" Vertices\n";

  if (get_nb_vertices()!=numVertices)
    {
        cout<<"!!! Erreur dans le fichier\n";
        exit(1);
    }

   if (asefile.eof())
    {
      asefile.clear();
    }

};

// -------------------------------------------------------------------------------
// Lecture des materiaux


void aseObject::readMaterial(int numMaterial)
{
  Material m;

  string s;
  
  cout<<"Lecture du material "<<numMaterial<<"\n";

  moveToMaterial(numMaterial);

  while (!asefile.eof() && (s.compare(MATERIAL)!=0))
  {
    asefile>>s;

    if (s.compare(MATERIAL_COLOR)==0)
        {
        cout<<"\tcheck "<<s;
        asefile>>m.cdiffuse[0];
        asefile>>m.cdiffuse[1];
        asefile>>m.cdiffuse[2];
        cout<<": "<<m.cdiffuse[0]<<"/"<<m.cdiffuse[1]<<"/"<<m.cdiffuse[2]<<"\n";
        }

    else if (s.compare(TEXTURE)==0)
        {
        cout<<"\tcheck "<<s;
        asefile>>m.texturefile;
        cout<<": "<<m.texturefile<<"\n";
        
        }

    else if (s.compare(MATERIAL_NAME)==0)
        {
        cout<<"\tcheck "<<s;
        asefile>>m.name;
        cout<<": "<<m.name<<"\n";
        
        }
        
    else if (s.compare(UTILE)==0)
        {
        cout<<"\tcheck "<<s;
        asefile>>m.utiling;
        cout<<": "<<m.utiling<<"\n";
        }

    else if (s.compare(VTILE)==0)
        {
        cout<<"\tcheck "<<s;
        asefile>>m.vtiling;
        cout<<": "<<m.vtiling<<"\n";
        }
  }

  add_material(m);

  if (asefile.eof())
    asefile.clear();
};
// Test --------------------------------------------------------------------------
//

void aseObject::test(void)
{
  // parcourir les vertices ----- OK
/*
  int i;

  map<const int,Vertex>::iterator it=Vertices.begin();

  for (i=0;i<nb_vertices;i++)
    {
      cout<<"Vertex "<<i<<": "<<(*it).second.coord[0]<<";"<<(*it).second.coord[1]<<";"<<(*it).second.coord[2]<<"\n";
      it++;
    }

  // parcourir les faces
  map<const int,Face>::iterator it2=Faces.begin();

  cout<<"nb_faces: "<<get_nb_faces()<<"\n";
  for (i=0;i<get_nb_faces();i++)
    {
      cout<<"face "<<i<<": "<<(*it2).second.coord[0]<<";"<<(*it2).second.coord[1]<<";"<<(*it2).second.coord[2]<<"\n";
      it2++;
    }
    */
};

