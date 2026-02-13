
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cube.h"
#include "utils.h"

struct cube {
    GLfloat translate[3];
    GLfloat scale[3];
    GLfloat theta[3];
    GLfloat color[3];
    GLint active;
};

Cube newCube(
    GLfloat tx, GLfloat ty, GLfloat tz,
    GLfloat sx, GLfloat sy, GLfloat sz,
    GLfloat r, GLfloat g, GLfloat b) {

    Cube c = malloc(sizeof(struct cube));
    c->translate[0] = tx;
    c->translate[1] = ty;
    c->translate[2] = tz;
    c->scale[0] = sx;
    c->scale[1] = sy;
    c->scale[2] = sz;
    c->theta[0] = 0.0;
    c->theta[1] = 0.0;
    c->theta[2] = 0.0;
    c->color[0] = r;
    c->color[1] = g;
    c->color[2] = b;
    c->active = 0;
    return c;
}

void setCubeColor(Cube c, GLfloat r, GLfloat g, GLfloat b) { c->color[0] = r; c->color[1] = g; c->color[2] = b; }
void setCubeActive(Cube c, GLint active) { c->active = active; }

void deleteCube(Cube c) { free(c); }

void polygon(int v0, int v1, int v2, int v3, Cube c) {
    glColor3fv(c->color);
    glBegin(GL_POLYGON);
    glNormal3fv(normals[v0]);
    glVertex3fv(vertices[v0]);
    glNormal3fv(normals[v1]);
    glVertex3fv(vertices[v1]);
    glNormal3fv(normals[v2]);
    glVertex3fv(vertices[v2]);
    glNormal3fv(normals[v3]);
    glVertex3fv(vertices[v3]);
    glEnd();
}

void drawCube(Cube c) {

    glPushMatrix();

    glTranslatef(c->translate[0], c->translate[1], c->translate[2]);
    glRotatef(c->theta[0], 1, 0, 0);
    glRotatef(c->theta[1], 0, 1, 0);
    glRotatef(c->theta[2], 0, 0, 1);
    glScalef(c->scale[0], c->scale[1], c->scale[2]);
    glColor3fv(c->color);

    polygon(0, 3, 2, 1, c);
    polygon(2, 3, 7, 6, c);
    polygon(0, 4, 7, 3, c);
    polygon(1, 2, 6, 5, c);
    polygon(4, 5, 6, 7, c);
    polygon(0, 1, 5, 4, c);

    glColor3f(1.0 - c->color[0], 1.0 - c->color[1], 1.0 - c->color[2]);
    glLineWidth(3.0);

    if(c->active) {
        for (int f = 0; f < 6; f++) {
            glBegin(GL_LINE_LOOP);
            for (int v = 0; v < 4; v++)
                glVertex3fv(vertices[faces[f][v]]);
            glEnd();
        }
    }

    glLineWidth(1.0);

    glPopMatrix();
}