#pragma once

#include <io.h>
#include <iostream>
#include <Windows.h>
#include <SDL.h> 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector>

#define SCREEN_LENGTH 1080
#define SCREEN_HEIGHT 1080

#define START_X 465.0
#define START_Y 400.0
#define START_Z -300.0

#pragma hdrstop

const float deltaRad = M_PI / 36;

float rotateMatrix_Y[4][4] =
{ { cos(deltaRad),0.0f,sin(deltaRad),0.0f },
{ 0.0f,1.0f,0.0f,0.0f },
{ -sin(deltaRad),0.0f,cos(deltaRad),0.0f },
{ 0.0f,0.0f,0.0f,1.0f } };


float rotateMatrix_X[4][4] =
{ { 1.0,0.0,0.0,0.0 },
{ 0.0,cos(deltaRad),-sin(deltaRad),0.0 },
{ 0.0,sin(deltaRad),cos(deltaRad),0.0 },
{ 0.0,0.0,0.0,1.0 } };

float rotateMatrix_Z[4][4] =
{ { cos(deltaRad),-sin(deltaRad),0.0,0.0 },
{ sin(deltaRad),cos(deltaRad),0.0,0.0 },
{ 0.0,0.0,1.0,0.0 },
{ 0.0,0.0,0.0,1.0 } };

float moveMatrix[4][4] =
{ { 1.,0.,0., 20.0 },
{ 0.,1.,0., 20.0 },
{ 0.,0.,1.,20. },
{ 0.,0.,0.,1 } };


float moveToStartMatrix[4][4] =
{ { 1.,0.,0.,0. },
{ 0.,1.,0.,0. },
{ 0.,0.,1.,0. },
{ -START_X + 100,-START_Y + 100,-1.,1.0 } };

float moveToCenterMatrix[4][4] =
{ { 1.,0.,0.,0. },
{ 0.,1.,0.,0. },
{ 0.,0.,1.,0. },
{ +START_X - 100, +START_Y - 100,1.,1.0 } };
