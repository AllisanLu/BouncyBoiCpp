#ifndef MOVEMENT_H
#define MOVEMENT_H

#define WIDTH 240 * 4
#define HEIGHT 160 * 4

bool moveBunny(float& row, float& speedR, float& col, float& speedC, float size);
void movePlatform(float* row, float speedR, float* col, float speedC, float width, float height);
#endif