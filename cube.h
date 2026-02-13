
#ifndef __CUBO_H__
#define __CUBO_H__

typedef struct cube* Cube;

Cube newCube(
    GLfloat, GLfloat, GLfloat,
    GLfloat, GLfloat, GLfloat,
    GLfloat, GLfloat, GLfloat);

void setCubeColor(Cube, GLfloat, GLfloat, GLfloat);
void setCubeActive(Cube, GLint);

void deleteCube(Cube);
void drawCube(Cube);

#endif