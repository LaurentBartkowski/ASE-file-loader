// C++ STD
#include <iostream>
#include <stdlib.h>
using namespace std;
// SDL, OpenGL
#include <SDL.h>
#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
// Avalanche
#include "object.hpp"
#include "aseObject.hpp"
#include "primitives.h"

/*********************************************************/
#define LARGEUR 800
#define HAUTEUR 600
#define ECRAN_BPP 24
//#define FULLSCREEN
#define FOV 45.0f
#define CODE_ERREUR 1
#define CODE_OK     0
#define PI 3.141592654


/*********************************************************/
// SDL, OpenGL
long temps;
SDL_Surface *surface=NULL;

// Avalanche
aseObject *laseobject;
GLuint lasetexture[1];
SDL_Surface *sdlasetexture;
#define LASE 1
int MODE_RENDU=0;

/*********************************************************/
SDL_Surface *InitSDL()
{
  SDL_Surface *surface=NULL;
  const SDL_VideoInfo *videoInfo=NULL;

  // init
  int res=SDL_Init(SDL_INIT_VIDEO);

  if (res!=0)
    {
      cout<<"Couldn't INIT VIDEO\n";
      SDL_Quit();
    }

  // capacité hardware
  videoInfo=SDL_GetVideoInfo();
  
  if (!videoInfo)
    {
      cout<<"Couldn't get VideoInfo\n";
      SDL_Quit();
    }

  int videoParams=SDL_OPENGL|SDL_GL_DOUBLEBUFFER|SDL_HWPALETTE;

  #ifdef FULLSCREEN
  videoParams|=SDL_FULLSCREEN;
  #endif

  videoParams|=((videoInfo->hw_available)?SDL_HWSURFACE:SDL_SWSURFACE);

  if (videoInfo->blit_hw)
      videoParams|=SDL_HWACCEL;

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

  surface=SDL_SetVideoMode(LARGEUR,HAUTEUR,ECRAN_BPP,videoParams);

  if (!surface)
    {
      cout<<"Couldn't init surface\n";
      SDL_Quit();
    }

  return surface;
}


/*********************************************************/
void redimensionner(int largeur, int hauteur)
{
  largeur=max(largeur,1);
  hauteur=max(hauteur,1);

  GLfloat rapport=(GLfloat)largeur/(GLfloat)hauteur;

  glViewport(0,0,(GLint)largeur,(GLint)hauteur);  
  glMatrixMode(GL_PROJECTION);
  gluPerspective(FOV,rapport,0.1f,1000.0f);  
  glMatrixMode(GL_MODELVIEW);
}

void ViewOrtho(void)
{
  int largeur=max(LARGEUR,1);
  int hauteur=max(HAUTEUR,1);

  GLfloat rapport=(GLfloat)largeur/(GLfloat)hauteur;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0, LARGEUR , HAUTEUR , 0, -1, 1 );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void ViewPerspective()
{
  int largeur=max(LARGEUR,1);
  int hauteur=max(HAUTEUR,1);

  GLfloat rapport=(GLfloat)largeur/(GLfloat)hauteur;

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective(FOV,rapport,0.1f,1000.0f);  
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}


/*********************************************************/
void drawGL_Repere(void)
{
  glPushMatrix();

  glScalef(1.0f,1.0f,1.0f);
  
  glBegin(GL_LINES);

  glColor4f(1.0f,0.0f,0.0f,1.0f);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(1.0f,0.0f,0.0f);

  glColor4f(0.0f,1.0f,0.0f,1.0f);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,1.0f,0.0f);

  glColor4f(0.0f,0.0f,1.0f,1.0f);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,0.0f,1.0f);

  glEnd();

  glPopMatrix();
}
void drawGL_ase(int mode)
{
  Vertex v1,v2,v3;
  TVertex tv1,tv2,tv3;

  int i,j=laseobject->get_nb_faces();

  map<const int, Face>::iterator it2=laseobject->Faces.begin();

  switch(mode) {
   case 0:
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,lasetexture[0]);

      // texture
      glBegin(GL_TRIANGLES);
      for (i=0;i<j;i++)
      {
            v1=laseobject->get_vertex((*it2).second.coord[0]);
            v2=laseobject->get_vertex((*it2).second.coord[1]);
            v3=laseobject->get_vertex((*it2).second.coord[2]);

            tv1=laseobject->get_tvertex((*it2).second.coord[0]);
            tv2=laseobject->get_tvertex((*it2).second.coord[1]);
            tv3=laseobject->get_tvertex((*it2).second.coord[2]);

            glTexCoord2f(tv1.coord[0],tv1.coord[1]);
            glVertex3f(v1.coord[0],v1.coord[1],v1.coord[2]);

            glTexCoord2f(tv2.coord[0],tv2.coord[1]);
            glVertex3f(v2.coord[0],v2.coord[1],v2.coord[2]);
    
            glTexCoord2f(tv3.coord[0],tv3.coord[1]);
            glVertex3f(v3.coord[0],v3.coord[1],v3.coord[2]);

            it2++;
      }
      glEnd();
      glDisable(GL_TEXTURE_2D);
      
      break;
  
   case 1:
     // Wireframe
     glColor3f(1.0f,1.0f,1.0f);
     glBegin(GL_LINES);
     for (i=0;i<j;i++)
     {
          v1=laseobject->get_vertex((*it2).second.coord[0]);
          v2=laseobject->get_vertex((*it2).second.coord[1]);
          v3=laseobject->get_vertex((*it2).second.coord[2]);

          glVertex3f(v1.coord[0],v1.coord[1],v1.coord[2]);
          glVertex3f(v2.coord[0],v2.coord[1],v2.coord[2]);

          glVertex3f(v2.coord[0],v2.coord[1],v2.coord[2]);
          glVertex3f(v3.coord[0],v3.coord[1],v3.coord[2]);

          glVertex3f(v3.coord[0],v3.coord[1],v3.coord[2]);
          glVertex3f(v1.coord[0],v1.coord[1],v1.coord[2]);

          it2++;
      }
      glEnd();
      
      break;
      
   case 2:
      // surfaces pleines
      glColor3f(1.0f,1.0f,1.0f);
      glBegin(GL_TRIANGLES);
      for (i=0;i<j;i++)
      {
            v1=laseobject->get_vertex((*it2).second.coord[0]);
            v2=laseobject->get_vertex((*it2).second.coord[1]);
            v3=laseobject->get_vertex((*it2).second.coord[2]);

            glVertex3f(v1.coord[0],v1.coord[1],v1.coord[2]);
            glVertex3f(v2.coord[0],v2.coord[1],v2.coord[2]);
            glVertex3f(v3.coord[0],v3.coord[1],v3.coord[2]);

            it2++;
      }
      glEnd();
      
      break;
  }  
}


/*********************************************************/
int main(int argc, char *argv[])
{
  int fini=1;
  int frames=0;
  long tempsDebut=SDL_GetTicks();
  float fps=0.0f;
  SDL_Event evenement;
  float angle=0.0f;

  // linux
  //laseobject=new aseObject("./statue.ase");
  // windows
  laseobject=new aseObject(".\\statue.ase");
  //  laseobject->test();
  
  surface=InitSDL();
  glEnable(GL_TEXTURE_2D);	
  
  cout<<"Textures loading";
  sdlasetexture=IMG_Load(".\\media\\marble.png");
  glGenTextures (1, &lasetexture[0]);
  glBindTexture (GL_TEXTURE_2D, lasetexture[0]);

  //glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, sdlasetexture->w, sdlasetexture->h, 0, GL_RGB, GL_UNSIGNED_BYTE, sdlasetexture->pixels);    
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sdlasetexture->w, sdlasetexture->h, GL_RGB, GL_UNSIGNED_BYTE, sdlasetexture->pixels);
  
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);    
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  SDL_FreeSurface(sdlasetexture);  
  cout<<" OK\n";
  
  redimensionner(LARGEUR,HAUTEUR);
  
  while(fini!=0)
    {
      temps=SDL_GetTicks();
      frames++;
      
      while(SDL_PollEvent(&evenement))
	{
	  switch(evenement.type)
	    {
	    case SDL_KEYDOWN:
	      if (evenement.key.keysym.sym==SDLK_ESCAPE)
	      		fini=0;
   		  
   		  MODE_RENDU++;
   		  if (MODE_RENDU>2) MODE_RENDU=0;
	      break;
	    case SDL_QUIT:
	      fini=0;
	      break;
	    default:
	      break;
	    }
	}
      
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Affichage de la 3d
      ViewPerspective();
      glEnable(GL_DEPTH_TEST);
      
      glTranslatef(0.0f,0.0f,-15.0f);
      glRotatef(angle,0.0f,1.0f,0.0f);

      angle+=0.5f;
      if (angle>=360.0f) angle=0.0f;

      //drawGL_Repere();
      drawGL_ase(MODE_RENDU);


      if(SDL_GetTicks()-tempsDebut>1000)
	{
	  fps=1000.0f * (float)frames/(float)(SDL_GetTicks()-tempsDebut);
	  frames=0;
      
	  tempsDebut=SDL_GetTicks();
      
	  cout<<fps<<" fps\n";
	}

      SDL_GL_SwapBuffers();
    }

  laseobject->~aseObject();
  SDL_Quit();
  
  return CODE_OK;
}
