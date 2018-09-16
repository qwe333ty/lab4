#include <iostream>
#include <vector>
#include <SDL.h> 

using namespace std;

#include "matrix.h"
#include "Trigometry Objects.h"

#define SCREEN_LENGTH 1200
#define SCREEN_HEIGHT 800

#define START_X 465.0
#define START_Y 400.0

extern SDL_Renderer *renderer;

#pragma hdrstop

//создаем на начальном этапе, заполняем его до цикла while а после сравниваем и выводим, и
//если надо, то заносим нужное значение в ячейку zbuff
ZBuff::ZBuff() {
	buff = new int*[SCREEN_LENGTH];
	for (register int i = 0; i < SCREEN_LENGTH; i++) {
		buff[i] = new int[SCREEN_LENGTH];
	}

	for (register int i = 0; i < SCREEN_LENGTH; i++)
		for (register int j = 0; j < SCREEN_LENGTH; j++)
			buff[i][j] = std::numeric_limits<float>::max();
}

ZBuff::~ZBuff() {
	for (register int i = 0; i < SCREEN_LENGTH; i++) {
		delete[] buff[i];
	}
	delete[] buff;
}

void ZBuff::fillFace(Facet *face) {
	Point *a = new Point(face->coordinates[0][0] + START_X, face->coordinates[0][1] + START_Y, face->coordinates[0][2]);
	Point *b = new Point(face->coordinates[1][0] + START_X, face->coordinates[1][1] + START_Y, face->coordinates[1][2]);
	Point *c = new Point(face->coordinates[2][0] + START_X, face->coordinates[2][1] + START_Y, face->coordinates[2][2]);
	Point *d = new Point(face->coordinates[3][0] + START_X, face->coordinates[3][1] + START_Y, face->coordinates[3][2]);

	bool flag = true;
#pragma region FIRST

	/*заполнение 1й половины*/
	if ((int)a->y == (int)b->y && (int)a->y == (int)c->y) {
		delete a;
		delete b;
		delete c;
		delete d;
		return;
	}
	/*фиксим погрешность*/
	a->x = (int)(a->x + 0.5);
	a->z = (int)(a->z + 0.5);
	a->y = (int)(a->y + 0.5);

	b->x = (int)(b->x + 0.5);
	b->y = (int)(b->y + 0.5);
	b->z = (int)(b->z + 0.5);

	c->x = (int)(c->x + 0.5);
	c->y = (int)(c->y + 0.5);
	c->z = (int)(c->z + 0.5);

	if (a->y > b->y) std::swap(a, b);
	if (a->y > c->y) std::swap(a, c);
	if (b->y > c->y) std::swap(b, c);

	int totalHeight = c->y - a->y;
	/*1*/
	for (int y = a->y; y <= b->y; y++) {
		int segmentHeight = b->y - a->y + 1;
		float alpha = (float)(y - a->y) / totalHeight;
		float beta = (float)(y - a->y) / segmentHeight;

		if (alpha == 0 || beta == 0) {
			continue;
		}

		Point *A = new Point();
		A->y = a->y + (c->y - a->y)*alpha;
		A->x = a->x + (c->x - a->x)*alpha;
		A->z = a->z + (c->z - a->z)*alpha;
		Point *B = new Point();
		B->y = a->y + (b->y - a->y)*beta;
		B->x = a->x + (b->x - a->x)*beta;
		B->z = a->z + (b->z - a->z)*beta;

		if ((int)A->x > (int)B->x) std::swap(A, B);

		for (int j = A->x; j <= B->x + 1; j++)
		{
			double phi = B->x == A->x ? 1. : (j - A->x) / (B->x - A->x);

			Point *P = new Point();
			P->x = A->x + (B->x - A->x)*phi;
			P->y = A->y + (B->y - A->y)*phi;
			P->z = A->z + (B->z - A->z)*phi;

			//SDL_RenderDrawPoint(renderer, j, y);
			if ((j < 1200 && y < 1200) && (j >= 0 && y >= 0)) {
				if (buff[j][y] < P->z) {
					buff[j][y] = P->z;
					SDL_RenderDrawPoint(renderer, j, y);
				}
			}
			delete P;
		}
		delete A;
		delete B;
	}
	/*2*/
	for (int y = b->y; y <= c->y; y++) {
		int segmentHeight = c->y - b->y + 1;
		float alpha = (float)(y - a->y) / totalHeight;
		float beta = (float)(y - b->y) / segmentHeight;

		if (alpha == 0 || beta == 0) {
			continue;
		}

		Point *A = new Point();
		A->y = a->y + (c->y - a->y)*alpha;
		A->x = a->x + (c->x - a->x)*alpha;
		A->z = a->z + (c->z - a->z)*alpha;
		Point *B = new Point();
		B->y = b->y + (c->y - b->y)*beta;
		B->x = b->x + (c->x - b->x)*beta;
		B->z = b->z + (c->z - b->z)*beta;

		if ((int)A->x > (int)B->x) std::swap(A, B);

		for (int j = A->x; j <= B->x; j++)
		{
			double phi = B->x == A->x ? 1. : (j - A->x) / (B->x - A->x);

			Point *P = new Point();
			P->x = A->x + (B->x - A->x)*phi;
			P->y = A->y + (B->y - A->y)*phi;
			P->z = A->z + (B->z - A->z)*phi;

			//SDL_RenderDrawPoint(renderer, j, y);
			if ((j < 1200 && y < 1200) && (j >= 0 && y >= 0)) {
				if (buff[j][y] < P->z) {
					buff[j][y] = P->z;
					SDL_RenderDrawPoint(renderer, j, y);
				}
			}
			delete P;
		}
		delete A;
		delete B;
	}

#pragma endregion

	a->copy(face->coordinates[0][0] + START_X, face->coordinates[0][1] + START_Y, face->coordinates[0][2]);
	b->copy(face->coordinates[3][0] + START_X, face->coordinates[3][1] + START_Y, face->coordinates[3][2]);
	c->copy(face->coordinates[2][0] + START_X, face->coordinates[2][1] + START_Y, face->coordinates[2][2]);

	a->x = (int)(a->x + 0.5);
	a->z = (int)(a->z + 0.5);
	a->y = (int)(a->y + 0.5);

	b->x = (int)(b->x + 0.5);
	b->y = (int)(b->y + 0.5);
	b->z = (int)(b->z + 0.5);

	c->x = (int)(c->x + 0.5);
	c->y = (int)(c->y + 0.5);
	c->z = (int)(c->z + 0.5);

	/*заполнение 2й половины*/
	if ((int)a->y == (int)b->y && (int)a->y == (int)c->y) {
		delete a;
		delete b;
		delete c;
		delete d;
		return;
	}
	/*фиксим погрешность*/
	a->x = (int)(a->x + 0.5);
	a->z = (int)(a->z + 0.5);
	a->y = (int)(a->y + 0.5);

	b->x = (int)(b->x + 0.5);
	b->y = (int)(b->y + 0.5);
	b->z = (int)(b->z + 0.5);

	c->x = (int)(c->x + 0.5);
	c->y = (int)(c->y + 0.5);
	c->z = (int)(c->z + 0.5);

	if (a->y > b->y) std::swap(a, b);
	if (a->y > c->y) std::swap(a, c);
	if (b->y > c->y) std::swap(b, c);

	totalHeight = c->y - a->y;
	/*1*/
	for (int y = a->y; y <= b->y; y++) {
		int segmentHeight = b->y - a->y + 1;
		float alpha = (float)(y - a->y) / totalHeight;
		float beta = (float)(y - a->y) / segmentHeight;

		if (alpha == 0 || beta == 0) {
			continue;
		}

		Point *A = new Point();
		A->y = a->y + (c->y - a->y)*alpha;
		A->x = a->x + (c->x - a->x)*alpha;
		A->z = a->z + (c->z - a->z)*alpha;
		Point *B = new Point();
		B->y = a->y + (b->y - a->y)*beta;
		B->x = a->x + (b->x - a->x)*beta;
		B->z = a->z + (b->z - a->z)*beta;

		if ((int)A->x > (int)B->x) std::swap(A, B);

		for (int j = A->x; j <= B->x; j++)
		{
			double phi = B->x == A->x ? 1. : (j - A->x) / (B->x - A->x);

			Point *P = new Point();
			P->x = A->x + (B->x - A->x)*phi;
			P->y = A->y + (B->y - A->y)*phi;
			P->z = A->z + (B->z - A->z)*phi;

			//SDL_RenderDrawPoint(renderer, j, y);
			if ((j < 1200 && y < 1200) && (j >= 0 && y >= 0)) {
				if (buff[j][y] < P->z) {
					buff[j][y] = P->z;
					SDL_RenderDrawPoint(renderer, j, y);
				}
			}
			delete P;
		}
		delete A;
		delete B;
	}
	/*2*/
	for (int y = b->y; y <= c->y; y++) {
		int segmentHeight = c->y - b->y + 1;
		float alpha = (float)(y - a->y) / totalHeight;
		float beta = (float)(y - b->y) / segmentHeight;

		if (alpha == 0 || beta == 0) {
			continue;
		}

		Point *A = new Point();
		A->y = a->y + (c->y - a->y)*alpha;
		A->x = a->x + (c->x - a->x)*alpha;
		A->z = a->z + (c->z - a->z)*alpha;
		Point *B = new Point();
		B->y = b->y + (c->y - b->y)*beta;
		B->x = b->x + (c->x - b->x)*beta;
		B->z = b->z + (c->z - b->z)*beta;

		if ((int)A->x > (int)B->x) std::swap(A, B);

		for (int j = A->x; j <= B->x; j++)
		{
			double phi = B->x == A->x ? 1. : (j - A->x) / (B->x - A->x);

			Point *P = new Point();
			P->x = A->x + (B->x - A->x)*phi;
			P->y = A->y + (B->y - A->y)*phi;
			P->z = A->z + (B->z - A->z)*phi;

			//SDL_RenderDrawPoint(renderer, j, y);
			if ((j < 1200 && y < 1200) && (j >= 0 && y >= 0)) {
				if (buff[j][y] < P->z) {
					buff[j][y] = P->z;
					SDL_RenderDrawPoint(renderer, j, y);
				}
			}
			delete P;
		}
		delete A;
		delete B;
	}


	delete a;
	delete b;
	delete c;
	delete d;
	return;
}

void ZBuff::clearBuff() {
	for (register int i = 0; i < SCREEN_LENGTH; i++)
		for (register int j = 0; j < SCREEN_LENGTH; j++)
			buff[i][j] = std::numeric_limits<float>::max();
}