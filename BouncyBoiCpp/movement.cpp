#include "movement.h"
#include <cstdlib> 

//returns true if bunny hits the ground
bool moveBunny(float& row, float& speedR, float& col, float& speedC, float size) {
	row = row + speedR;
	col += speedC;
	if (row < 0) {
		row = 0;
		speedR = -speedR;
		float leftOrRight = (rand() % 3) - 1;
		speedC = speedR * leftOrRight;
	}
	if (row > HEIGHT - size) {
		row = HEIGHT - size;
		speedR = 0;
		speedC = 0;
		return true;
	}
	if (col < 0) {
		col = 0;
		speedC = -speedC;
	}
	if (col > WIDTH - size) {
		col = WIDTH - size;
		speedC = -speedC;
	}
	return false;
}

void movePlatform(float* row, float speedR, float* col, float speedC, float width, float height) {
	*row = *row + speedR;
	*col = *col + speedC;

	if (*row < 0) {
		*row = 0;
	}
	if (*row < (HEIGHT / 2) - height) {
		*row = (HEIGHT / 2) - height;
	}
	if (*col < 0) {
		*col = 0;
	}
	if (*col > WIDTH - width) {
		*col = WIDTH - width;
	}
}