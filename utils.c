
#include "utils.h"
#include "maps.h"

GLfloat vertices[][3] = {
    { -1.0, -1.0, -1.0},
    {  1.0, -1.0, -1.0},
    {  1.0,  1.0, -1.0},
    { -1.0,  1.0, -1.0},
    { -1.0, -1.0,  1.0},
    {  1.0, -1.0,  1.0},
    {  1.0,  1.0,  1.0},
    { -1.0,  1.0,  1.0}
};
GLfloat normals[][3] = {
    { -1.0, -1.0, -1.0},
    {  1.0, -1.0, -1.0},
    {  1.0,  1.0, -1.0},
    { -1.0,  1.0, -1.0},
    { -1.0, -1.0,  1.0},
    {  1.0, -1.0,  1.0},
    {  1.0,  1.0,  1.0},
    { -1.0,  1.0,  1.0}
};
GLint faces[6][4] = {
    {0, 3, 2, 1},
    {2, 3, 7, 6},
    {0, 4, 7, 3},
    {1, 2, 6, 5},
    {4, 5, 6, 7},
    {0, 1, 5, 4}
};

GLint tempo = 1000 / FPS;

GLfloat worldTranslate[3] = { 0.0, 0.0, 0.0 };
GLfloat worldScale[3] = { 1.0, 1.0, 1.0 };
GLfloat worldTheta[3] = { 0.0, 0.0, 0.0 };

GLfloat camRadius = defaultCamRadius;
GLfloat camYaw = defaultCamYaw;
GLfloat camPitch = defaultCamPitch;
GLfloat camRoll = defaultCamRoll;

GLint mouseDragging = 0;
GLint firstMouseX = 0;
GLint firstMouseY = 0;
GLint lastMouseX = 0;
GLint lastMouseY = 0;
GLfloat mouseSensitivity = 0.2;

char** map = NULL;
GLint pacmanX = 0;
GLint pacmanY = 0;

GLint* ghostX = NULL;
GLint* ghostY = NULL;
Direction* ghostDir = NULL;

GLint wallCount = 0;
GLint wallMode = 0;

Tile* validTiles = NULL;
int validTileCount = 0;
int remainingTileCount = 0;

GLint pause = 1;
GameState game = RUNNING;

void freeValidTiles(void) {
    if (validTiles) {
        free(validTiles);
        validTiles = NULL;
    }
    validTileCount = 0;
}

void drawWorldAxis(void) {
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 5.0);
    glEnd();
}

float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float wrapAngle(float angle) {
    while (angle >= 360.0) angle -= 360.0;
    while (angle < 0.0) angle += 360.0;
    return angle;
}

int isTileValid(int x, int y) {
    if (map == NULL) return 0;
    if (x < 0 || x >= xTabSize()) return 0;
    if (y < 0 || y >= yTabSize()) return 0;
    return map[x][y] == 1;
}

void drawText(char* string, int x, int y) {
    glPushAttrib(GL_ENABLE_BIT); 
    glDisable(GL_LIGHTING);     
    glDisable(GL_TEXTURE_2D);    
    glDisable(GL_DEPTH_TEST);    

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 1000);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(x, y);

    for (char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
}

void checkGameState() {
    game = WIN;

    for (int i = 0; i < numGhosts(); i++) {
        if(pacmanX == ghostX[i] && pacmanY == ghostY[i]) {
            game = LOSE;
            pause = 1;
            return;
        }
    }
    
    for (int i = 0; i < validTileCount; i++) {
        if (validTiles[i].visited == 0) {
            game = RUNNING;
        }
    }

    if (game == WIN) pause = 1;
}

void setTileVisited() {
    for (int i = 0; i < validTileCount; i++) {
        if (pacmanX == validTiles[i].x && pacmanY == validTiles[i].y) {
            if (validTiles[i].visited == 0) {
                validTiles[i].visited = 1;
                remainingTileCount--;
            }
        }
    }
}