#include <io.h>
#include <iostream>
#include <Windows.h>
#include <SDL.h> 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector>

#include "Trigometry Objects.h"
#include "matrix.h"

#define SCREEN_LENGTH 920
#define SCREEN_HEIGHT 850

#define START_X 465.0
#define START_Y 400.0
#define START_Z -300.0
#define MU 5.0

#define deg(number) M_PI*(double)(number)/180.0

using namespace std;

#pragma hdrstop

typedef Point Vector;

SDL_Renderer  *renderer;
ZBuff *zBuff = new ZBuff();
Point *light = new Point(0.0f, 0.0f, 1.0f);
float n = 3.0;

int main(int argc, char *argv[]) {
	SDL_Window    *window;
	SDL_Event      event;
	SDL_CreateWindowAndRenderer(SCREEN_LENGTH, SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

	float a = 200.0f; // ширина 250
	float b = 50.0f; // толщина 50
	float c = 2.0*n*b; // длина 300
	float k = 60.0f;
	//cout << "Enter n: ";
	//cin >> n;

	Point *aB = new Point(0.0f, 0.0f, 0.0f);
	Point *bB = new Point(0.0f, 0.0f, -c);
	Point *cB = new Point(a, 0.0f, -c);
	Point *dB = new Point(a, 0.0f, 0.0f);
	Point *aT = new Point(0.0f, -b, 0.0f);
	Point *bT = new Point(0.0f, -b, -c);
	Point *cT = new Point(a, -b, -c);
	Point *dT = new Point(a, -b, 0.0f);

	Point *aN = new Point(a / 2.0 + k / 2.0, 0.0f, -c / (2.0*n) + k / 2.0);
	Point *bN = new Point(a / 2.0 + k / 2.0, 0.0f, -c / (2.0*n) - k / 2.0);
	Point *cN = new Point(a / 2.0 - k / 2.0, 0.0f, -c / (2.0*n) - k / 2.0);
	Point *dN = new Point(a / 2.0 - k / 2.0, 0.0f, -c / (2.0*n) + k / 2.0);

	Point *aV = new Point(a / 2.0 + k / 2.0, -b, -c / (2.0*n) + k / 2.0);
	Point *bV = new Point(a / 2.0 + k / 2.0, -b, -c / (2.0*n) - k / 2.0);
	Point *cV = new Point(a / 2.0 - k / 2.0, -b, -c / (2.0*n) - k / 2.0);
	Point *dV = new Point(a / 2.0 - k / 2.0, -b, -c / (2.0*n) + k / 2.0);

	Stairs *stairs = new Stairs();
	for (float i = 0; i < n; i++) {
		stairs->objects.push_back(new Parallelepiped(
			//права€
			new Facet(new Point(aB, 0.0f, -b*i, 0.0f), new Point(bB, 0.0f, -b*i, (c / n)*i), new Point(bT, 0.0f, -b*i, (c / n)*i), new Point(aT, 0.0f, -b*i, 0.0f)),
			//верхн€€
			new Facet(new Point(aT, 0.0f, -b*i, 0.0f), new Point(bT, 0.0f, -b*i, (c / n)*i), new Point(cT, 0.0f, -b*i, (c / n)*i), new Point(dT, 0.0f, -b*i, 0.0f)),
			//фронтальна€
			new Facet(new Point(cT, 0.0f, -b*i, (c / n)*i), new Point(bT, 0.0f, -b*i, (c / n)*i), new Point(bB, 0.0f, -b*i, (c / n)*i), new Point(cB, 0.0f, -b*i, (c / n)*i)),
			//лева€
			new Facet(new Point(cT, 0.0f, -b*i, (c / n)*i), new Point(dT, 0.0f, -b*i, 0.0f), new Point(dB, 0.0f, -b*i, 0.0f), new Point(cB, 0.0f, -b*i, (c / n)*i)),
			//задн€€ грань
			new Facet(new Point(aB, 0.0f, -b*i, 0.0f), new Point(aT, 0.0f, -b*i, 0.0f), new Point(dT, 0.0f, -b*i, 0.0f), new Point(dB, 0.0f, -b*i, 0.0f)),
			//нижн€€
			new Facet(new Point(aB, 0.0f, -b*i, 0.0f), new Point(bB, 0.0f, -b*i, (c / n)*i), new Point(cB, 0.0f, -b*i, (c / n)*i), new Point(dB, 0.0f, -b*i, 0.0f)),

			/*внутренние*/
			//права€
			new Facet(new Point(aN, 0.0f, -b*i, 0.0f), new Point(bN, 0.0f, -b*i, 0.0f), new Point(bV, 0.0f, -b*i, 0.0f), new Point(aV, 0.0f, -b*i, 0.0f)),
			//передн€€
			new Facet(new Point(bN, 0.0f, -b*i, 0.0f), new Point(cN, 0.0f, -b*i, 0.0f), new Point(cV, 0.0f, -b*i, 0.0f), new Point(bV, 0.0f, -b*i, 0.0f)),
			//лева€
			new Facet(new Point(cN, 0.0f, -b*i, 0.0f), new Point(dN, 0.0f, -b*i, 0.0f), new Point(dV, 0.0f, -b*i, 0.0f), new Point(cV, 0.0f, -b*i, 0.0f)),
			//дальн€€
			new Facet(new Point(aN, 0.0f, -b*i, 0.0f), new Point(dN, 0.0f, -b*i, 0.0f), new Point(dV, 0.0f, -b*i, 0.0f), new Point(aV, 0.0f, -b*i, 0.0f))
		));
	}

	int colour = 1;
	int counter = 1;


	for (auto iterator = stairs->objects.begin(); iterator != stairs->objects.end(); iterator++, counter++) {
		auto temp = iterator;
		(*iterator)->drawFaces(*temp, counter);
	}
	zBuff->clearBuff();
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	colour = 1;

	SDL_RenderPresent(renderer);
	bool done = false;
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_x:
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
					SDL_RenderClear(renderer);
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
					for (auto iteratorStairs = stairs->objects.begin(); iteratorStairs != stairs->objects.end(); iteratorStairs++) {
						for (auto iterator = (*iteratorStairs)->face.begin(); iterator != (*iteratorStairs)->face.end(); iterator++) {
							(*iterator)->coordinates = recalculatFaces((*iterator), rotateMatrix_X);
						}
						for (auto iterator = (*iteratorStairs)->square.begin(); iterator != (*iteratorStairs)->square.end(); iterator++) {
							(*iterator)->coordinates = recalculatFaces((*iterator), rotateMatrix_X);
						}
					}
					counter = 1;
					for (auto iterator = stairs->objects.begin(); iterator != stairs->objects.end(); iterator++, counter++) {
						auto temp = iterator;
						(*iterator)->drawFaces(*temp, counter);
					}
					zBuff->clearBuff();
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
					colour = 1;
					zBuff->clearBuff();
					SDL_RenderPresent(renderer);
					break;
				case SDLK_y:
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
					SDL_RenderClear(renderer);
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
					for (auto iteratorStairs = stairs->objects.begin(); iteratorStairs != stairs->objects.end(); iteratorStairs++) {
						for (auto iterator = (*iteratorStairs)->face.begin(); iterator != (*iteratorStairs)->face.end(); iterator++) {
							(*iterator)->coordinates = recalculatFaces((*iterator), rotateMatrix_Y);
						}
						for (auto iterator = (*iteratorStairs)->square.begin(); iterator != (*iteratorStairs)->square.end(); iterator++) {
							(*iterator)->coordinates = recalculatFaces((*iterator), rotateMatrix_Y);
						}
					}
					counter = 1;
					for (auto iterator = stairs->objects.begin(); iterator != stairs->objects.end(); iterator++, counter++) {
						auto temp = iterator;
						(*iterator)->drawFaces(*temp, counter);
					}

					zBuff->clearBuff();
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
					colour = 1;
					zBuff->clearBuff();
					SDL_RenderPresent(renderer);
					break;
				case SDLK_z:
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
					SDL_RenderClear(renderer);
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
					for (auto iteratorStairs = stairs->objects.begin(); iteratorStairs != stairs->objects.end(); iteratorStairs++) {
						for (auto iterator = (*iteratorStairs)->face.begin(); iterator != (*iteratorStairs)->face.end(); iterator++) {
							(*iterator)->coordinates = recalculatFaces((*iterator), rotateMatrix_Z);
						}
						for (auto iterator = (*iteratorStairs)->square.begin(); iterator != (*iteratorStairs)->square.end(); iterator++) {
							(*iterator)->coordinates = recalculatFaces((*iterator), rotateMatrix_Z);
						}
					}
					counter = 1;
					/*for (auto iterator = stairs->objects.begin(); iterator != stairs->objects.end(); iterator++, counter++) {
					//if (iterator != stairs->objects.begin() && iterator != stairs->objects.end()) {
					auto temp = iterator;
					if (iterator != stairs->objects.begin()) {
					temp = --iterator;
					iterator++;
					}
					(*iterator)->drawFaces(*temp, counter);
					//printf("%d\n", counter);
					//SDL_RenderPresent(renderer);
					//}
					}*/
					for (auto iterator = stairs->objects.begin(); iterator != stairs->objects.end(); iterator++, counter++) {
						auto temp = iterator;
						(*iterator)->drawFaces(*temp, counter);
					}
					zBuff->clearBuff();
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
					zBuff->clearBuff();
					colour = 1;
					SDL_RenderPresent(renderer);
					break;
				}
				break;
			}
		}

	}
	delete zBuff;
	delete light;
	delete aB;
	delete bB;
	delete cB;
	delete dB;
	delete aT;
	delete bT;
	delete cT;
	delete dT;
	return 0;
}
//
//for (int i = 0; i < n; i++) {
//	circle.coord[i].x = (sin((2 * M_PI / n)*i))*R + x center;
//	circle.coord[i].x = (cos((2 * M_PI / n)*i))*R + y center;
//}
//circle.coor[n] = coord[0];