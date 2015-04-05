#ifndef TEXTURES_H
#define TEXTURES_H

#include <GL/glx.h>
#include <GL/gl.h>

typedef struct {
    int width;
    int height;
    unsigned char *data;
} textureImage;

int loadBMP(char *filename, textureImage *texture);
Bool loadGLTextures(GLuint *matexture);
GLuint EmptyTexture();

#endif
