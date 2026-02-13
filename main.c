
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "wall.h"
#include "cube.h"
#include "pacman.h"
#include "ghost.h"
#include "maps.h"
#include "utils.h"

/*
Instituto Politécnico de Portalegre
Computação Gráfica
Trabalho prático 2 - Pacman
Engenharia Informática - 2º Ano

*/

void freeWorld();
void genWorld();
void drawWorld();
void pauseGame();
void setWorldColor();
void addWall();
void removeWall();
void updateWalls();
int isTileValidForGhosts(int, int);

void myDisplayFunc();
void myReshapeFunc(int, int);
void myKeyboardFunc(char, int, int);
void mySpecialFunc(int, int, int);
void myMouseFunc(int, int, int, int);
void myMotionFunc(int, int);
void myTimerFunc(int);

Cube** mapCubes = NULL;
Pacman player = NULL;
Ghost* ghosts = NULL;
Wall* walls = NULL;
char bufferString[64];

int main(int argc, char** argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOWWIDTH, WINDOWHEIGHT);
	glutCreateWindow("- PacMan -");

	glutDisplayFunc(myDisplayFunc);
	glutReshapeFunc(myReshapeFunc);

	glutKeyboardFunc(myKeyboardFunc);
	glutSpecialFunc(mySpecialFunc);
	glutMouseFunc(myMouseFunc);
	glutMotionFunc(myMotionFunc);

	glutTimerFunc(tempo, myTimerFunc, 0);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	char* mapFile = (argc <= 1) ? "Map0.txt" : argv[1];
	printf("MAP: %s\n", mapFile);
	openMapsFile(mapFile);

	map = nextMap();

	if (map != NULL) {
		genWorld();
		printMap();
	}

	glutMainLoop();

	return 0;
}

void freeWorld() {

	if (mapCubes) {
		for (int i = 0; i < xTabSize(); i++) {
			if (mapCubes[i]) {
				for (int j = 0; j < yTabSize(); j++) {
					if (mapCubes[i][j]) deleteCube(mapCubes[i][j]);
				}
				free(mapCubes[i]);
			}
		}
		free(mapCubes);
		mapCubes = NULL;
	}

	if (walls) {
		for (int i = 0; i < validTileCount; i++) {
			if (walls[i] != NULL) {
				deleteWall(walls[i]);
			}
		}
		free(walls);
		walls = NULL;
		wallCount = 0;
	}

	if (player) {
		deletePacman(player);
		player = NULL;
	}

	int nGhosts = numGhosts();

	if (ghosts) {
		for (int i = 0; i < nGhosts; i++) {
			if (ghosts[i]) {
				deleteGhost(ghosts[i]);
			}
		}
		free(ghosts);
		ghosts = NULL;
	}

	if (ghostX) { free(ghostX); ghostX = NULL; }
	if (ghostY) { free(ghostY); ghostY = NULL; }
	if (ghostDir) { free(ghostDir); ghostDir = NULL; }

	if (validTiles) {
		free(validTiles);
		validTiles = NULL;
	}
}

void genWorld() {
	int i, j;

	freeWorld();
	freeValidTiles();

	mapCubes = malloc(xTabSize() * sizeof(Cube*));
	for (i = 0; i < xTabSize(); i++)
		mapCubes[i] = malloc(yTabSize() * sizeof(Cube*));

	validTileCount = 0;
	for (i = 0; i < xTabSize(); i++)
		for (j = 0; j < yTabSize(); j++)
			if (map[i][j] == 1)
				validTileCount++;

	validTiles = malloc(validTileCount * sizeof(Tile));
	int idx = 0;
	for (i = 0; i < xTabSize(); i++) {
		for (j = 0; j < yTabSize(); j++) {
			if (map[i][j] == 1) {
				mapCubes[i][j] = newCube(
					i * 2.0, j * 2.0, 0.0,
					1.0, 1.0, 0.3,
					0.3, 0.3, 0.3
				);
				setCubeActive(mapCubes[i][j], 1);
				validTiles[idx].x = i;
				validTiles[idx].y = j;
				validTiles[idx].visited = 0;
				idx++;
			}
			else {
				mapCubes[i][j] = NULL;
			}
		}
	}

	if (validTileCount < 1 + numGhosts()) {
		printf("ERROR: Not enough valid tiles for Pacman and ghosts\n");
		freeValidTiles();
		exit(1);
	}
	
	remainingTileCount = validTileCount;

	for (i = validTileCount - 1; i > 0; i--) {
		int r = rand() % (i + 1);
		Tile tmp = validTiles[i];
		validTiles[i] = validTiles[r];
		validTiles[r] = tmp;
	}

	walls = malloc(validTileCount * sizeof(Wall));
	for (int i = 0; i < validTileCount; i++)
		walls[i] = NULL;

	wallCount = 0;

	worldTranslate[0] = -((xTabSize() - 1) * 2.0) / 2.0;
	worldTranslate[1] = -((yTabSize() - 1) * 2.0) / 2.0;
	worldTranslate[2] = 0.0;

	pacmanX = validTiles[0].x;
	pacmanY = validTiles[0].y;

	player = newPacman(pacmanX * 2.0, pacmanY * 2.0, 1.3, 1.0, 1.0, 0.0, 9.0, 4.0);

	setTileVisited();
	setWorldColor();

	int nGhosts = numGhosts();
	ghostX = malloc(nGhosts * sizeof(GLint));
	ghostY = malloc(nGhosts * sizeof(GLint));
	ghosts = malloc(nGhosts * sizeof(Ghost));
	ghostDir = malloc(nGhosts * sizeof(Direction));

	for (i = 0; i < nGhosts; i++) {
		ghostX[i] = validTiles[i + 1].x;
		ghostY[i] = validTiles[i + 1].y;
		ghostDir[i] = NOWHERE;

		GLfloat r = 0.0, g = 0.0, b = 0.0;
		GLfloat dominant = 0.3 + ((GLfloat)rand() / RAND_MAX) * 0.7;

		switch (i % 3) {
		case 0: r = dominant; break;
		case 1: g = dominant; break;
		case 2: b = dominant; break; }

		ghosts[i] = newGhost(
			ghostX[i] * 2.0, ghostY[i] * 2.0, 1.3,
			r, g, b,
			1.0 + 3.0*r + 2.0*g +b
		);

	}
}

void drawWorld() {
	int i, j;
	for (i = 0; i < xTabSize(); i++)
		for (j = 0; j < yTabSize(); j++)
			if (mapCubes[i][j] != NULL)
				drawCube(mapCubes[i][j]);

	drawPacman(player);

	for (i = 0; i < numGhosts(); i++)
		drawGhost(ghosts[i]);
	for (i = 0; i < validTileCount; i++)
		if(walls[i] != NULL) drawWall(walls[i]);
}

void pauseGame() {
	if (pause) { pause = 0; glutTimerFunc(tempo, myTimerFunc, 0); }
	else pause = 1;
}

void setWorldColor() {
	GLfloat* tempArray = NULL;

	for (int i = 0; i < validTileCount; i++) {
		if (validTiles[i].visited == 1) {
			for (int j = 0; j < (xTabSize() * yTabSize()); j++) {
				if (mapCubes[j / yTabSize()][j % yTabSize()] == NULL) continue;

				int tx = validTiles[i].x;
				int ty = validTiles[i].y;

				if (mapCubes[tx][ty] != NULL) {
					setCubeColor(mapCubes[tx][ty], 0.6, 0.6, 0.6);
				}
			}
		}
	}
}

void addWall() {
	for (int i = 0; i < validTileCount; i++) {
		if (walls[i] == NULL) {
			walls[i] = newWall(pacmanX*2.0, pacmanY*2.0, 0.0, 0.6, 0.3, 0.0);
			wallCount++;
			return;
		}
	}
}

void removeWall() {
	GLfloat wx = pacmanX * 2.0;
	GLfloat wy = pacmanY * 2.0;

	for (int i = 0; i < validTileCount; i++) {
		if (walls[i] == NULL) continue;

		GLfloat* t = getWalltranslate(walls[i]);

		if (t[0] == wx && t[1] == wy) {
			deleteWall(walls[i]);
			walls[i] = NULL;
			return;
		}
	}
}

void updateWalls() {
	for (int i = 0; i < validTileCount; i++) {
		if (walls[i] == NULL) continue;

		GLfloat* t = getWalltranslate(walls[i]);
		GLfloat x = t[0];
		GLfloat y = t[1];

		int N = 0, S = 0, E = 0, W = 0;

		for (int j = 0; j < validTileCount; j++) {
			if (i == j || walls[j] == NULL) continue;

			GLfloat* ot = getWalltranslate(walls[j]);

			if (ot[0] == x && ot[1] == y + 2.0) N = 1;
			if (ot[0] == x && ot[1] == y - 2.0) S = 1;
			if (ot[0] == x + 2.0 && ot[1] == y) E = 1;
			if (ot[0] == x - 2.0 && ot[1] == y) W = 1;
		}

		wallType type;

		if (N && S && E && W) type = WC;

		else if (!N && S && E && W) type = WTD;
		else if (N && !S && E && W) type = WTU;
		else if (N && S && !E && W) type = WTR;
		else if (N && S && E && !W) type = WTL;

		else if (N && S && !E && !W) type = WV;
		else if (!N && !S && E && W) type = WH;

		else if (!N && S && E && !W) type = WTLC;
		else if (!N && S && !E && W) type = WTRC;
		else if (N && !S && E && !W) type = WBLC;
		else if (N && !S && !E && W) type = WBRC;

		else if (N && !S && !E && !W) type = WU;
		else if (!N && S && !E && !W) type = WD;
		else if (!N && !S && E && !W) type = WR;
		else if (!N && !S && !E && W) type = WL;

		else type = WDOT;

		setWallType(walls[i], type);
	}
}

int isTileValidForGhosts(int x, int y) {
	if (x < 0 || x >= xTabSize() || y < 0 || y >= yTabSize()) return 0;
	if (map[x][y] != 1) return 0;

	for (int i = 0; i < validTileCount; i++) {
		if (walls[i] == NULL) continue;
		GLfloat* t = getWalltranslate(walls[i]);
		int wx = (int)(t[0] / 2.0 + 0.5);
		int wy = (int)(t[1] / 2.0 + 0.5);
		if (wx == x && wy == y) return 0;
	}

	for (int j = 0; j < numGhosts(); j++) {
		if (ghostX[j] == x && ghostY[j] == y) {
			return 0;
		}
	}

	return 1;
}

void myDisplayFunc() {
	GLfloat yawRad = camYaw * PI / 180.0;
	GLfloat pitchRad = camPitch * PI / 180.0;

	GLfloat camX = camRadius * cos(pitchRad) * sin(yawRad);
	GLfloat camY = camRadius * sin(pitchRad);
	GLfloat camZ = camRadius * cos(pitchRad) * cos(yawRad);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(camRoll, 0.0, 0.0, 1.0);

	glPushMatrix();

	glTranslatef(worldTranslate[0], worldTranslate[1], worldTranslate[2]);
	glRotatef(worldTheta[0], 1.0, 0.0, 0.0);
	glRotatef(worldTheta[1], 0.0, 1.0, 0.0);
	glRotatef(worldTheta[2], 0.0, 0.0, 1.0);
	glScalef(worldScale[0], worldScale[1], worldScale[2]);

	drawWorldAxis();
	drawWorld();

	glPopMatrix();

	if(game == WIN) drawText("WIN", (WINDOWWIDTH / 2) - 30, WINDOWHEIGHT / 2 + 18);
	else if(game == LOSE) drawText("LOSE", (WINDOWWIDTH / 2) - 30, WINDOWHEIGHT / 2 + 18);
	else if(pause) drawText("PAUSE", (WINDOWWIDTH / 2) - 30, WINDOWHEIGHT / 2 + 18);

	snprintf(bufferString, sizeof(bufferString), "Score: %d", (validTileCount-remainingTileCount)*100);
	drawText(bufferString, 20, WINDOWHEIGHT/2);

	glutSwapBuffers();
}

void myReshapeFunc(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	gluPerspective(45.0, aspect, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);
}

void myKeyboardFunc(char key, int x, int y) {
	switch (key) {
	case 'q': case 'Q': camYaw -= defaultDelta; break;
	case 'e': case 'E': camYaw += defaultDelta; break;
	case 'r': case 'R':
		if (pause == 0) pauseGame();
		camRadius = defaultCamRadius;
		camYaw = defaultCamYaw;
		camPitch = defaultCamPitch;
		camRoll = defaultCamRoll;
		genWorld();
		game = RUNNING;
		break;
	case 'g': case 'G': break;
	case 'p': case 'P': if(game == RUNNING) pauseGame(); break;
	case 'w': case 'W': wallMode = !wallMode; break;
	}

	camYaw = clamp(camYaw, -85.0, 85.0);
	camPitch = clamp(camPitch, -70.0, 0.0);
	camRoll = clamp(camRoll, -85.0, 85.0);

	glutPostRedisplay();
}

void mySpecialFunc(int key, int x, int y) {
	if (isPacmanBusy(player) || pause) return;

	int tx = pacmanX;
	int ty = pacmanY;

	switch (key) {
	case GLUT_KEY_LEFT:  tx = pacmanX - 1; break;
	case GLUT_KEY_RIGHT: tx = pacmanX + 1; break;
	case GLUT_KEY_UP:    ty = pacmanY + 1; break;
	case GLUT_KEY_DOWN:  ty = pacmanY - 1; break;
	}

	GLint canMove = isTileValid(tx, ty);

	switch (key) {
	case GLUT_KEY_LEFT:  orderPacman(player, LEFT, canMove); break;
	case GLUT_KEY_RIGHT: orderPacman(player, RIGHT, canMove); break;
	case GLUT_KEY_UP:    orderPacman(player, UP, canMove); break;
	case GLUT_KEY_DOWN:  orderPacman(player, DOWN, canMove); break;
	}

	if (canMove) {
		if (wallMode) addWall();
		switch (key) {
		case GLUT_KEY_LEFT:  pacmanX--; break;
		case GLUT_KEY_RIGHT: pacmanX++; break;
		case GLUT_KEY_UP:    pacmanY++; break;
		case GLUT_KEY_DOWN:  pacmanY--; break;
		}
		removeWall();
	}

	updateWalls();

	setTileVisited();
	setWorldColor();

	glutPostRedisplay();
}

void myMouseFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseDragging = 1;
			firstMouseX = lastMouseX = x;
			firstMouseY = lastMouseY = y;
		}
		else {
			mouseDragging = 0;
		}
	}

	if (state == GLUT_DOWN) {
		switch (button) {
		case 3: camRadius -= defaultDelta; break;
		case 4: camRadius += defaultDelta; break;
		}
	}
	camRadius = clamp(camRadius, 20, 200);
	glutPostRedisplay();
}

void myMotionFunc(int x, int y) {
	if (!mouseDragging) return;

	int dx = x - lastMouseX;
	int dy = y - lastMouseY;

	if (firstMouseY < 500) dx = -dx;

	camRoll += dx * mouseSensitivity;
	camPitch += dy * mouseSensitivity;

	camYaw = clamp(camYaw, -85.0, 85.0);
	camPitch = clamp(camPitch, -70.0, 0.0);
	camRoll = clamp(camRoll, -85.0, 85.0);

	lastMouseX = x;
	lastMouseY = y;

	glutPostRedisplay();
}

void myTimerFunc(int n) {
	updatePacman(player);

	int nGhosts = numGhosts();

	for (int i = 0; i < nGhosts; i++) {
		updateGhost(ghosts[i]);
	}

	for (int i = 0; i < nGhosts; i++) {

		if (isGhostBusy(ghosts[i])) continue;

		int gx = ghostX[i];
		int gy = ghostY[i];

		if (ghostDir[i] != NOWHERE) {
			int nx = gx;
			int ny = gy;

			switch (ghostDir[i]) {
			case LEFT:  nx--; break;
			case RIGHT: nx++; break;
			case UP:    ny++; break;
			case DOWN:  ny--; break;
			default: break;
			}

			if (isTileValidForGhosts(nx, ny)) {
				orderGhost(ghosts[i], ghostDir[i]);

				switch (ghostDir[i]) {
				case LEFT:  ghostX[i]--; break;
				case RIGHT: ghostX[i]++; break;
				case UP:    ghostY[i]++; break;
				case DOWN:  ghostY[i]--; break;
				}

				continue;
			}
		}

		int px = pacmanX;
		int py = pacmanY;
		int dx = px - gx;
		int dy = py - gy;

		Direction idealDirs[4];
		int idealCount = 0;

		if (abs(dx) > abs(dy)) {
			if (dx > 0) idealDirs[idealCount++] = RIGHT;
			else if (dx < 0) idealDirs[idealCount++] = LEFT;

			if (dy > 0) idealDirs[idealCount++] = UP;
			else if (dy < 0) idealDirs[idealCount++] = DOWN;
		}
		else {
			if (dy > 0) idealDirs[idealCount++] = UP;
			else if (dy < 0) idealDirs[idealCount++] = DOWN;

			if (dx > 0) idealDirs[idealCount++] = RIGHT;
			else if (dx < 0) idealDirs[idealCount++] = LEFT;
		}

		Direction chosen = NOWHERE;

		for (int k = 0; k < idealCount; k++) {
			int nx = gx, ny = gy;
			switch (idealDirs[k]) {
			case LEFT:  nx--; break;
			case RIGHT: nx++; break;
			case UP:    ny++; break;
			case DOWN:  ny--; break;
			}

			if (isTileValidForGhosts(nx, ny)) {
				chosen = idealDirs[k];
				break;
			}
		}

		if (chosen == NOWHERE) {
			Direction allDirs[4] = { UP, DOWN, LEFT, RIGHT };
			Direction validDirs[4];
			int validCount = 0;

			for (int k = 0; k < 4; k++) {
				int nx = gx, ny = gy;
				switch (allDirs[k]) {
				case LEFT:  nx--; break;
				case RIGHT: nx++; break;
				case UP:    ny++; break;
				case DOWN:  ny--; break;
				}

				if (isTileValidForGhosts(nx, ny)) {
					validDirs[validCount++] = allDirs[k];
				}
			}

			if (validCount > 0) {
				chosen = validDirs[rand() % validCount];
			}
		}

		if (chosen != NOWHERE) {
			ghostDir[i] = chosen;
			orderGhost(ghosts[i], chosen);

			switch (chosen) {
			case LEFT:  ghostX[i]--; break;
			case RIGHT: ghostX[i]++; break;
			case UP:    ghostY[i]++; break;
			case DOWN:  ghostY[i]--; break;
			}
		}
	}

	glutPostRedisplay();
	checkGameState();
	if (!pause) glutTimerFunc(tempo, myTimerFunc, n + 1);
}