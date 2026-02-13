
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "cube.h"
#include "wall.h"

struct wall {
    GLfloat translate[3];
    GLfloat scale[3];
    GLfloat theta[3];
    GLfloat color[3];
    GLint show, active;
    wallType type;
};

Wall newWall(
    GLfloat tx, GLfloat ty, GLfloat tz,
    GLfloat r, GLfloat g, GLfloat b) {

    Wall w = malloc(sizeof(struct wall));
    w->translate[0] = tx;
    w->translate[1] = ty;
    w->translate[2] = tz;
    w->scale[0] = 1.0;
    w->scale[1] = 1.0;
    w->scale[2] = 1.0;
    w->theta[0] = 0.0;
    w->theta[1] = 0.0;
    w->theta[2] = 0.0;
    w->color[0] = r;
    w->color[1] = g;
    w->color[2] = b;
    return w;
}

GLfloat* getWalltranslate(Wall w) { return w->translate; }

void setWallType(Wall w, wallType type) { w->type = type; }

void deleteWall(Wall w) { free(w); }

void drawWall(Wall w) {

    if (!w->show) return;

    glPushMatrix();
    glTranslatef(w->translate[0], w->translate[1], w->translate[2]);
    glScalef(w->scale[0], w->scale[1], w->scale[2]);

    Cube poste = newCube(0.0, 0.0, 1.3, 0.2, 0.2, 1, w->color[0], w->color[1], w->color[2]);
    drawCube(poste);
    deleteCube(poste);

    if (w->type== WBLC || w->type == WBRC || w->type == WV || w->type == WTU || w->type == WTL || w->type == WTR || w->type == WC || w->type == WU) {
        Cube Nposte = newCube(0.0, 0.5, 1.5, 0.1, 0.5, 0.1, w->color[0], w->color[1], w->color[2]);
        drawCube(Nposte);
        deleteCube(Nposte);
    }

    if (w->type == WTLC || w->type == WTRC || w->type == WV || w->type == WTD || w->type == WTL || w->type == WTR || w->type == WC || w->type == WD) {
        Cube Sposte = newCube(0.0, -0.5, 1.5, 0.1, 0.5, 0.1, w->color[0], w->color[1], w->color[2]);
        drawCube(Sposte);
        deleteCube(Sposte);
    }

    if (w->type == WTLC || w->type == WBLC || w->type == WH || w->type == WTD || w->type == WTU || w->type == WTL || w->type == WC || w->type == WR) {
        Cube Eposte = newCube(0.5, 0.0, 1.5, 0.5, 0.1, 0.1, w->color[0], w->color[1], w->color[2]);
        drawCube(Eposte);
        deleteCube(Eposte);
    }

    if (w->type == WTRC || w->type == WBRC || w->type == WH || w->type == WTD || w->type == WTU || w->type == WTR || w->type == WC || w->type == WL) {
        Cube Oposte = newCube(-0.5, 0.0, 1.5, 0.5, 0.1, 0.1, w->color[0], w->color[1], w->color[2]);
        drawCube(Oposte);
        deleteCube(Oposte);
    }

    glPopMatrix();
}