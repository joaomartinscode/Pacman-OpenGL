
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include "ghost.h"
#include "utils.h"
#include "cube.h"

struct ghost {
    GLfloat translate[3];
    GLfloat scale[3];
    GLfloat color[3];
    GLint show;
    GLint showBoundingBox;
    GLfloat vel;
    GLfloat rotX, rotY;
    GLfloat rotXCur, rotYCur;
    GLfloat m[16];
};

Ghost newGhost(
    GLfloat tx, GLfloat ty, GLfloat tz,
    GLfloat r, GLfloat g_, GLfloat b,
    GLfloat vel) {

    Ghost g = malloc(sizeof(struct ghost));
    g->translate[0] = tx;
    g->translate[1] = ty;
    g->translate[2] = tz;
    g->scale[0] = 1.0;
    g->scale[1] = 1.0;
    g->scale[2] = 1.0;
    g->color[0] = r;
    g->color[1] = g_;
    g->color[2] = b;
    g->show = 1;
    g->showBoundingBox = 0;
    g->vel = vel;
    g->rotX = g->rotY = 0;
    g->rotXCur = g->rotYCur = 0;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, g->m);
    glPopMatrix();

    return g;
}

GLint isGhostBusy(Ghost g) { return (g->rotXCur != 0 || g->rotYCur != 0); }

void deleteGhost(Ghost g) { free(g); }

void updateGhostMatrix(Ghost g, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRotatef(angle, x, y, z);
    glMultMatrixf(g->m);
    glGetFloatv(GL_MODELVIEW_MATRIX, g->m);
    glPopMatrix();
}

void updateGhost(Ghost g) {
    if (!g) return;

    if (g->rotX == 0 && g->rotY == 0 && g->rotXCur == 0 && g->rotYCur == 0) return;

    GLfloat stepRotation = g->vel / FPS * 90;

    if (g->rotX != 0) {
        GLfloat delta = (g->rotX > 0 ? stepRotation : -stepRotation);

        if (fabs(delta) > fabs(g->rotX)) delta = g->rotX;

        g->rotXCur += delta;
        g->rotX -= delta;

        GLfloat fraction = fabs(g->rotXCur) / 90.0;
        g->translate[1] += -(delta > 0 ? 1 : -1) * 2 * fabs(delta) / 90.0;
        g->translate[2] = 1.3 + sin(fraction * PI) * 0.5;
    }

    if (g->rotY != 0) {
        GLfloat delta = (g->rotY > 0 ? stepRotation : -stepRotation);

        if (fabs(delta) > fabs(g->rotY)) delta = g->rotY;

        g->rotYCur += delta;
        g->rotY -= delta;

        GLfloat fraction = fabs(g->rotYCur) / 90.0;
        g->translate[0] += (delta > 0 ? 1 : -1) * 2 * fabs(delta) / 90.0;
        g->translate[2] = 1.3 + sin(fraction * PI) * 0.5;
    }

    if (g->rotX == 0 && g->rotY == 0) {
        if (g->rotXCur != 0 || g->rotYCur != 0) {

            updateGhostMatrix(
                g,
                g->rotXCur + g->rotYCur,
                g->rotXCur != 0 ? 1 : 0,
                g->rotYCur != 0 ? 1 : 0,
                0
            );

            g->rotX = g->rotXCur = 0;
            g->rotY = g->rotYCur = 0;
            g->translate[2] = 1.3;
        }
    }
}

void drawGhost(Ghost g) {
    if (!g->show) return;

    glPushMatrix();
    glTranslatef(g->translate[0], g->translate[1], g->translate[2]);
    glScalef(g->scale[0], g->scale[1], g->scale[2]);

    if (g->rotXCur != 0)
        glRotatef(g->rotXCur, 1, 0, 0);
    if (g->rotYCur != 0)
        glRotatef(g->rotYCur, 0, 1, 0);

    Cube c = newCube(0, 0, 0, 1, 1, 1, g->color[0], g->color[1], g->color[2]);
    setCubeActive(c, 1);
    drawCube(c);
    deleteCube(c);

    glPopMatrix();
}

void orderGhost(Ghost g, Direction dir) {
    switch (dir) {
    case UP:    g->rotX = -90; break;
    case DOWN:  g->rotX = 90; break;
    case LEFT:  g->rotY = -90; break;
    case RIGHT: g->rotY = 90; break;
    }
}
