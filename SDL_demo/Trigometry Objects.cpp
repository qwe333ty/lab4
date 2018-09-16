#include <iostream>
#include <vector>
#include <SDL.h> 

using namespace std;

#include "Trigometry Objects.h"

#define SCREEN_LENGTH 1080
#define SCREEN_HEIGHT 1080

#define START_X 465.0
#define START_Y 400.0
#define START_Z -300.0

#define TOP 0
#define BOT 1
#define BACK 2
#define RIGHT 3
#define FRONT 4
#define LEFT 5

extern SDL_Renderer *renderer;
extern ZBuff *zBuff;
extern Point *light;
extern float n;

float xQ = 200.0f;
float yQ = 50.0f;
float zQ = 2 * n * yQ;

#define len 80

#pragma hdrstop

Point::Point(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::Point() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point::Point(Point *a, float deltaX, float deltaY, float deltaZ) {
	this->x = a->x + deltaX;
	this->y = a->y + deltaY;
	this->z = a->z + deltaZ;
}

void Point::copy(Point *point) {
	this->x = point->x;
	this->y = point->y;
	this->z = point->z;
}

void Point::copy(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Facet::Facet() {
	coordinates = new float*[4];
	for (int i = 0; i < 4; i++) {
		coordinates[i] = new float[4];
	}
	for (int i = 0; i<4; i++)
		for (int j = 0; j < 4; j++) {
			coordinates[i][j] = 0.0f;
		}
}

Facet::Facet(Point *a, Point *b, Point *c, Point *d) {
	coordinates = new float*[4];
	for (int i = 0; i < 4; i++) {
		coordinates[i] = new float[4];
	}

	this->normal = new Point();

	Point A;
	Point B;
	Point C;
	Point D;

	A.x = coordinates[0][0] = a->x;
	A.y = coordinates[0][1] = a->y;
	A.z = coordinates[0][2] = a->z;
	coordinates[0][3] = 1.0f;

	B.x = coordinates[1][0] = b->x;
	B.y = coordinates[1][1] = b->y;
	B.z = coordinates[1][2] = b->z;
	coordinates[1][3] = 1.0f;

	C.x = coordinates[2][0] = c->x;
	C.y = coordinates[2][1] = c->y;
	C.z = coordinates[2][2] = c->z;
	coordinates[2][3] = 1.0f;

	coordinates[3][0] = d->x;
	coordinates[3][1] = d->y;
	coordinates[3][2] = d->z;
	coordinates[3][3] = 1.0f;

	center = new Point(-xQ / 2, 0, -zQ / 2);

	square = new float*[4];
	for (int i = 0; i < 4; i++) {
		square[i] = new float[4];
	}
	square[0][0] = 0.0f;
	square[0][1] = c->y;
	square[0][2] = 0.0f;
	square[0][3] = 1.0f;

	square[1][0] = 0.0f;
	square[1][1] = c->y;
	square[1][2] = -zQ;
	square[1][3] = 1.0f;

	square[2][0] = -xQ;
	square[2][1] = c->y;
	square[2][2] = -zQ;
	square[2][3] = 1.0f;

	square[3][0] = -xQ;
	square[3][1] = c->y;
	square[3][2] = 0.0f;
	square[3][3] = 1.0f;
}

Facet::Facet(float xA, float yA, float zA, float xB, float yB, float zB, float xC, float yC, float zC, float xD, float yD, float zD) {
	coordinates = new float*[4];
	for (int i = 0; i < 4; i++) {
		coordinates[i] = new float[4];
	}

	this->normal = new Point();

	Point a;
	Point b;
	Point c;
	Point d;

	a.x = coordinates[0][0] = xA;
	a.y = coordinates[0][1] = yA;
	a.z = coordinates[0][2] = zA;
	coordinates[0][3] = 1.0f;

	b.x = coordinates[1][0] = xB;
	b.y = coordinates[1][1] = yB;
	b.z = coordinates[1][2] = zB;
	coordinates[1][3] = 1.0f;

	c.x = coordinates[2][0] = xC;
	c.y = coordinates[2][1] = yC;
	c.z = coordinates[2][2] = zC;
	coordinates[2][3] = 1.0f;

	coordinates[3][0] = xD;
	coordinates[3][1] = yD;
	coordinates[3][2] = zD;
	coordinates[3][3] = 1.0f;

	center = new Point(-xQ / 2, 0, -zQ / 2);

	square = new float*[4];
	for (int i = 0; i < 4; i++) {
		square[i] = new float[4];
	}
	square[0][0] = 0.0f;
	square[0][1] = 0.0f;
	square[0][2] = 0.0f;
	square[0][3] = 1.0f;

	square[1][0] = 0.0f;
	square[1][1] = 0.0f;
	square[1][2] = -zQ;
	square[1][3] = 1.0f;

	square[2][0] = -xQ;
	square[2][1] = 0.0f;
	square[2][2] = -zQ;
	square[2][3] = 1.0f;

	square[3][0] = -xQ;
	square[3][1] = 0.0f;
	square[3][2] = 0.0f;
	square[3][3] = 1.0f;
}


Facet::~Facet() {
	for (int i = 0; i < 4; i++)
		delete[] coordinates[i];
	delete[] coordinates;

	delete normal;
}


/*
case 1:	SDL_SetRenderDrawColor(renderer, 255, 153, 51, 0); q++;break;
case 2: SDL_SetRenderDrawColor(renderer, 153, 102, 255, 0); q++;break;
case 3: SDL_SetRenderDrawColor(renderer, 255, 153, 255, 0); q++;break; /*102, 255, 255,
case 4: SDL_SetRenderDrawColor(renderer, 153, 255, 255, 0); q++;break;
case 5: SDL_SetRenderDrawColor(renderer, 102, 255, 102, 0); q++;break;
case 6: SDL_SetRenderDrawColor(renderer, 204, 255, 0, 0); q++;break;
*/

void Facet::draw() {
	SDL_SetRenderDrawColor(renderer, 153, 102, 255, 0);
	SDL_RenderDrawLine(renderer, coordinates[0][0] + START_X, coordinates[0][1] + START_Y, coordinates[1][0] + START_X, coordinates[1][1] + START_Y);
	SDL_SetRenderDrawColor(renderer, 255, 153, 255, 0);
	SDL_RenderDrawLine(renderer, coordinates[1][0] + START_X, coordinates[1][1] + START_Y, coordinates[2][0] + START_X, coordinates[2][1] + START_Y);
	SDL_SetRenderDrawColor(renderer, 153, 255, 255, 0);
	SDL_RenderDrawLine(renderer, coordinates[2][0] + START_X, coordinates[2][1] + START_Y, coordinates[3][0] + START_X, coordinates[3][1] + START_Y);
	SDL_SetRenderDrawColor(renderer, 102, 255, 102, 0);
	SDL_RenderDrawLine(renderer, coordinates[3][0] + START_X, coordinates[3][1] + START_Y, coordinates[0][0] + START_X, coordinates[0][1] + START_Y);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
}

void Facet::drawLines() {

}

Parallelepiped::Parallelepiped(Facet *q, Facet *w, Facet *e, Facet *r, Facet *t, Facet *y,
	Facet *z, Facet *x, Facet *c, Facet *v)
{
	face.push_back(q);
	face.push_back(w);
	face.push_back(e);
	face.push_back(r);
	face.push_back(t);
	face.push_back(y);

	square.push_back(z);
	square.push_back(x);
	square.push_back(c);
	square.push_back(v);
}

Parallelepiped::Parallelepiped() {
	Facet *q = new Facet();
	Facet *w = new Facet();
	Facet *e = new Facet();
	Facet *r = new Facet();
	Facet *t = new Facet();
	Facet *y = new Facet();
}

Parallelepiped::~Parallelepiped()
{
}

void Parallelepiped::drawFaceLines() {
	Facet *temp;
	int q = 0;
	for (auto iterator = face.begin(); iterator != face.end(); iterator++, q++) {
		if (q == 5) {
			(*iterator)->draw();
		}
		else
			continue;
	}

}

void Parallelepiped::drawFaces(Parallelepiped *p, int counter) {
	Facet *temp;
	int secondCounter = 1;
	int q = 1;
	int qq = 1;
	//итератор по внешним граням
	for (auto iterator = face.begin(); iterator != face.end(); iterator++, qq++) {
		if (qq == 2 || qq == 6)
			continue;

		temp = *iterator;

		switch (q)
		{
		case 1:	SDL_SetRenderDrawColor(renderer, 255, 153, 51, 0); q++; break;
		case 2: SDL_SetRenderDrawColor(renderer, 153, 102, 255, 0); q++; break;
		case 3: SDL_SetRenderDrawColor(renderer, 255, 153, 255, 0); q++; break; /*102, 255, 255,*/
		case 4: SDL_SetRenderDrawColor(renderer, 153, 255, 255, 0); q++; break;
		case 5: SDL_SetRenderDrawColor(renderer, 102, 255, 102, 0); q++; break;
		case 6: SDL_SetRenderDrawColor(renderer, 204, 255, 0, 0); q++; break;

		case 7: SDL_SetRenderDrawColor(renderer, 204, 153, 51, 0); q++; break;
		case 8: SDL_SetRenderDrawColor(renderer, 153, 153, 255, 0); q++; break;
		case 9: SDL_SetRenderDrawColor(renderer, 153, 255, 204, 0); q++; break;
		case 10: SDL_SetRenderDrawColor(renderer, 153, 204, 51, 0); q++; break;
		}

		zBuff->fillFace(temp);
	}

	/*итератор по внутренним граням*/
	for (auto iterator = square.begin(); iterator != square.end(); iterator++, qq++) {
		temp = *iterator;

		switch (q)
		{
		case 1:	SDL_SetRenderDrawColor(renderer, 255, 153, 51, 0); q++; break;
		case 2: SDL_SetRenderDrawColor(renderer, 153, 102, 255, 0); q++; break;
		case 3: SDL_SetRenderDrawColor(renderer, 255, 153, 255, 0); q++; break; /*102, 255, 255,*/
		case 4: SDL_SetRenderDrawColor(renderer, 153, 255, 255, 0); q++; break;
		case 5: SDL_SetRenderDrawColor(renderer, 102, 255, 102, 0); q++; break;
		case 6: SDL_SetRenderDrawColor(renderer, 204, 255, 0, 0); q++; break;

		case 7: SDL_SetRenderDrawColor(renderer, 204, 153, 51, 0); q++; break;
		case 8: SDL_SetRenderDrawColor(renderer, 153, 153, 255, 0); q++; break;
		case 9: SDL_SetRenderDrawColor(renderer, 153, 255, 204, 0); q++; break;
		case 10: SDL_SetRenderDrawColor(renderer, 153, 204, 51, 0); q++; break;
		}

		zBuff->fillFace(temp);
	}
}

Stairs::Stairs()
{
}

Stairs::~Stairs()
{
}

float ** recalculatFaces(Facet *face, float B[4][4]) {//multiply matrixies

	float ** newCoordinates = new float*[4];
	for (int i = 0; i< 4; i++) {
		newCoordinates[i] = new float[4];
	}

	for (int i = 0; i<4; i++)
		for (int j = 0; j < 4; j++) {
			newCoordinates[i][j] = 0.0f;
		}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				newCoordinates[i][j] += face->coordinates[i][k] * B[k][j]; /*[i][k]  [k][j]*/
			}
		}
	}

	for (int i = 0; i < 4; i++)
		delete[] face->coordinates[i];
	delete[] face->coordinates;

	return newCoordinates;
}



