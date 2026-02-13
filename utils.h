
#ifndef __UTILS_H__
#define __UTILS_H__

#include <GL/glut.h>

#define FPS 60
#define PI 3.141592653589
#define WINDOWWIDTH 1000
#define WINDOWHEIGHT 1000
#define defaultDelta 10.0
#define defaultCamRadius 100.0
#define defaultCamYaw    0.0
#define defaultCamPitch  0.0
#define defaultCamRoll   0.0

typedef enum {
    UP, DOWN, LEFT, RIGHT, NOWHERE
} Direction;

typedef enum {
    WIN, LOSE, RUNNING
} GameState;

typedef enum {
    WTLC, WTRC, WBLC, WBRC, WH, WV, WTD, WTU, WTL, WTR, WC, WDOT, WU, WD, WL, WR
} wallType;

typedef struct {
    GLfloat x;
    GLfloat y;
    GLint visited;
} Tile;

extern GLint pacmanX;
extern GLint pacmanY;

extern GLint* ghostX;
extern GLint* ghostY;
extern Direction* ghostDir;

extern GLint wallCount;
extern GLint wallMode;

extern char** map;
extern Tile* validTiles;
extern int validTileCount;
extern int remainingTileCount;
void freeValidTiles(void);

extern GLfloat vertices[8][3];
extern GLfloat normals[8][3];
extern GLint faces[6][4];

extern GLint tempo;

extern GLfloat worldTranslate[3];
extern GLfloat worldScale[3];
extern GLfloat worldTheta[3];

extern GLfloat camRadius;
extern GLfloat camYaw;
extern GLfloat camPitch;
extern GLfloat camRoll;

extern GLint mouseDragging;
extern GLint firstMouseX;
extern GLint firstMouseY;
extern GLint lastMouseX;
extern GLint lastMouseY;
extern GLfloat mouseSensitivity;

extern GLint pause;
extern GameState game;

void drawWorldAxis(void);
float clamp(float, float, float);
float wrapAngle(float);
int isTileValid(int, int);
void drawText(char*, int, int);
void checkGameState();
void setTileVisited();
#endif
