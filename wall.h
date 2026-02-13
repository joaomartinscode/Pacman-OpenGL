
#ifndef __WALL_H__
#define __WALL_H__

#include "utils.h"

typedef struct wall* Wall;

Wall newWall(
    GLfloat, GLfloat, GLfloat,
    GLfloat, GLfloat, GLfloat);

GLfloat* getWallTranslate(Wall);

void setWallType(Wall, wallType);

void deleteWall(Wall);
void drawWall(Wall);

#endif