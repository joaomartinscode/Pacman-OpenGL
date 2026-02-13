
#ifndef __MAPS_H__
#define __MAPS_H__

void openMapsFile(char* name);
void closeMapsFile();

int numberOfMaps();

char** nextMap();
char** getMap();

int xTabSize();
int yTabSize();
int numGhosts();

void printMap();

#endif