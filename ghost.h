#ifndef __GHOST_H__
#define __GHOST_H__

#include "utils.h"

typedef struct ghost* Ghost;

Ghost newGhost(
    GLfloat tx, GLfloat ty, GLfloat tz,
    GLfloat r, GLfloat g, GLfloat b,
    GLfloat vel);

GLint isGhostBusy(Ghost g);

void deleteGhost(Ghost);
void updateGhostTranslate(Ghost, GLfloat, GLfloat, GLfloat);
void updateGhostScale(Ghost, GLfloat, GLfloat, GLfloat);
void updateGhost(Ghost);
void orderGhost(Ghost, Direction);
void drawGhost(Ghost);

#endif
