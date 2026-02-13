
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "pacman.h"
#include "utils.h"
#include "cube.h"

struct pacman {
    GLfloat translate[3];
    GLfloat scale[3];
    GLfloat theta[3];
    GLfloat color[3];
    GLfloat alpha;
    GLfloat vel;
    GLfloat vInterna;
    GLfloat iSwing;
    GLfloat iRot, iPos;
};

Pacman newPacman(
    GLfloat tx, GLfloat ty, GLfloat tz,
    GLfloat r, GLfloat g, GLfloat b,
    GLfloat vel, GLfloat vInterna) {

    Pacman p = malloc(sizeof(struct pacman));
    p->translate[0] = tx; p->translate[1] = ty; p->translate[2] = tz;
    p->scale[0] = p->scale[1] = p->scale[2] = 1.0;
    p->theta[0] = p->theta[1] = p->theta[2] = 0.0;
    p->color[0] = r; p->color[1] = g; p->color[2] = b;
    p->alpha = 0.0;
    p->vel = vel;
    p->vInterna = vInterna;
    p->iSwing = 0.0;
    p->iRot = 0.0;
    p->iPos = 0.0;
    return p;
}

GLint isPacmanBusy(Pacman p) { return (p->iPos > 0.0 || abs(p->iRot) > 0.0); }

void deletePacman(Pacman p) { free(p); }

void updatePacmanTranslate(Pacman p, GLfloat dx, GLfloat dy, GLfloat dz) {
    p->translate[0] += dx;
    p->translate[1] += dy;
    p->translate[2] += dz;
}

void updatePacmanAnim(Pacman p) {

    p->iSwing += p->vInterna / FPS;
    
    if (p->iSwing >= 1.0) {
        p->iSwing = 1.0;
        p->vInterna = -p->vInterna;
    }
    else if (p->iSwing <= 0.0) {
        p->iSwing = 0.0;
        p->vInterna = -p->vInterna;
    }

    if (p->iRot != 0.0) {
        GLfloat rotStep = 500.0 / FPS;

        if (p->iRot > 0.0) {
            GLfloat step = (p->iRot < rotStep) ? p->iRot : rotStep;
            p->alpha += step;
            p->iRot -= step;
        }
        else {
            GLfloat step = (fabs(p->iRot) < rotStep) ? fabs(p->iRot) : rotStep;
            p->alpha -= step;
            p->iRot += step;
        }

        p->alpha = wrapAngle(p->alpha);
    }

}

void updatePacman(Pacman p) {
    updatePacmanAnim(p);

    if (p->iRot != 0.0 || p->iPos <= 0.0)
        return;

    GLfloat moveDist = p->vel / FPS;
    if (moveDist > p->iPos) moveDist = p->iPos;

    GLfloat dx = 0.0, dy = 0.0;
    int facing = ((int)(p->alpha + 45) % 360) / 90;

    switch (facing) {
    case 0: dx = moveDist; break;
    case 1: dy = moveDist; break;
    case 2: dx = -moveDist; break;
    case 3: dy = -moveDist; break;
    }

    updatePacmanTranslate(p, dx, dy, 0.0);

    p->iPos -= moveDist;
}

void orderPacman(Pacman p, Direction dir, GLint doMove) {
    GLfloat targetAlpha = 0.0;
    switch (dir) {
    case RIGHT: targetAlpha = 0.0; break;
    case UP:    targetAlpha = 90.0;  break;
    case LEFT:  targetAlpha = 180.0; break;
    case DOWN:  targetAlpha = 270.0; break;
    }

    GLfloat delta = targetAlpha - p->alpha;
    if (delta > 180.0) delta -= 360.0;
    if (delta < -180.0) delta += 360.0;
    p->iRot = delta;
    if (doMove) {
        p->iPos = 2.0;
    }
}

void drawPacman(Pacman p) {
    GLfloat mouthAngle = p->iSwing * 30.0;

    glPushMatrix();

    glTranslatef(p->translate[0], p->translate[1], p->translate[2]);
    glRotatef(p->alpha, 0.0, 0.0, 1.0);
    glScalef(p->scale[0], p->scale[1], p->scale[2]);

    GLfloat h = 0.3;
    GLfloat w = 1.0;
    GLfloat d = 0.8;

    glPushMatrix();
    glTranslatef(-w/2.0, 0.0, 0.0);
    glRotatef(+mouthAngle, 0.0, 0.0, 1.0);
    glTranslatef(+w/2.0, +h/2.0, 0.0);
    Cube upper = newCube( 0.0, 0.0, 0.0, w, h, d, p->color[0], p->color[1], p->color[2] );
    setCubeActive(upper, 1);
    drawCube(upper);
    deleteCube(upper);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-w/2.0, 0.0, 0.0);
    glRotatef(-mouthAngle, 0.0, 0.0, 1.0);
    glTranslatef(+w/2.0, -h/2.0, 0.0);
    Cube lower = newCube( 0.0, 0.0, 0.0, w, h, d, p->color[0], p->color[1], p->color[2] );
    setCubeActive(lower, 1);
    drawCube(lower);
    deleteCube(lower);
    glPopMatrix();

    glPopMatrix();

}