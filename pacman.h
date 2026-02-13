
#ifndef __PACMAN_H__
#define __PACMAN_H__

#include "utils.h"

typedef struct pacman* Pacman;

Pacman newPacman(
    GLfloat, GLfloat, GLfloat,
    GLfloat, GLfloat, GLfloat,
    GLfloat, GLfloat);

GLint isPacmanBusy(Pacman p);

void deletePacman(Pacman);
void updatePacman(Pacman);
void orderPacman(Pacman, Direction, GLint);
void drawPacman(Pacman p);

#endif
