//-----------------------------------------------------------------
// Space Out Application
// C++ Header - SpaceOut.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "EnemySprite.h"
#include <stack>
#include <queue>

class coord {
public:
	int x;
	int y;
	int dist;
	coord* prev;

public:
	coord::coord(int x, int y) {
		this->x = x;
		this->y = y;
		dist = 0;
		prev = NULL;
	}

	coord::coord(int x, int y, int dist, coord* prev) {
		this->x = x;
		this->y = y;
		this->dist = dist;
		this->prev = prev;
	}
};


coord** path;
//Map Variables
int maxrow = 15, maxcol = 21;//(15-3)/2=6-------(21-3)/2
int** map;
int wall, maxwall = ((maxrow - 3) / 2) * ((maxcol - 3) / 2);//6x9
//Key Variables
int keyX, keyY;
//Bomb Variables

BOOL isPlanted;
//Granade Variables
double granadex = 0, granadey = 0;
int bombx, bomby;
//int bfrmgranade = 0
int granadestep = 10;
//POINT granadeVelocity;
//Player Variables
int angle = 0;
int playerx = 1, playery = 1, bfrm = 0, step = 10;
int fireRate = 25;
//Enemy variables
int bfrmEnemy = 50, stepEnemy = 5,enemyNumber = 5;
int* enemyDirection = new int[enemyNumber];
int enemy, maxenemy = 5;
EnemySprite** allEnemies;
//Friend Variables
int friendx, friendy, bfrmFriend = 50, stepFriend = 2;

HDC hDC;
//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE         _hInstance;
GameEngine*       _pGame;
HDC               _hOffscreenDC;
HBITMAP           _hOffscreenBitmap;
Bitmap*			  _pBackgroundBitmap;
Bitmap*           _pSmExplosionBitmap;
Bitmap*           _pLgExplosionBitmap;
Bitmap*           _pGameOverBitmap;
Bitmap*			  _pNextLevelBitmap;
Bitmap*			  keyBitmap;
Bitmap*			  enemyBitmap;
Bitmap*			  playerBitmap;
Bitmap*			  friendBitmap;
Bitmap*			  wallbmp;
Bitmap*			  granadeBitmap;
Bitmap*			  heartBitmap;
Sprite*			  keySprite;
Sprite*			  enemySprite;
Sprite*			  playerSprite;
Sprite*			  friendSprite;
Sprite*			  granadeSprite;
Sprite*			  wallSprite;


int               _iFireInputDelay;
int               _iNumLives, _iScore, _iDifficulty;
int				  seed;
BOOL              _bGameOver,_bNextLevel;

//-----------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------
void NewGame();
void AddEnemy();
void CreateMap();
void MoveEnemies();
int findpathBFS(coord* c1, coord* c2, int** map, int maxrow, int maxcol, int empty, coord** path);
void MoveFriend();
