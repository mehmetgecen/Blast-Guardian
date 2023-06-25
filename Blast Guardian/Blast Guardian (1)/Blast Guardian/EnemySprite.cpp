//-----------------------------------------------------------------
// Alien Sprite Object
// C++ Source - AlienSprite.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "EnemySprite.h"

//-----------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------
//extern Bitmap* _pBlobboBitmap;
//extern Bitmap* _pBMissileBitmap;
//extern Bitmap* _pJellyBitmap;
//extern Bitmap* _pJMissileBitmap;
//extern Bitmap* _pTimmyBitmap;
//extern Bitmap* _pTMissileBitmap;
extern int     _iDifficulty;
//extern int posX, posY;
//extern int enemyDirection;

//-----------------------------------------------------------------
// AlienSprite Constructor(s)/Destructor
//-----------------------------------------------------------------
EnemySprite::EnemySprite(Bitmap* pBitmap, RECT& rcBounds,
    BOUNDSACTION baBoundsAction) : Sprite(pBitmap, rcBounds,
        baBoundsAction) {
    eSprite = new Sprite(pBitmap, rcBounds, baBoundsAction);
}

EnemySprite::~EnemySprite()
{
}

//-----------------------------------------------------------------
// AlienSprite General Methods
//-----------------------------------------------------------------
SPRITEACTION EnemySprite::Update()
{
    return 1;
}

void EnemySprite::SetEnemyPos(int x, int y) {
    posX = x;
    posY = y;
    //eSprite->SetPosition(posX, posY);
}

void EnemySprite::SetPosition(int x, int y) {
    eSprite->SetPosition(x, y);
}

void EnemySprite::SetVelocity(int x, int y) {
    eSprite->SetVelocity(x, y);
}



int EnemySprite::GetPosX() {
    return posX;
}

int EnemySprite::GetPosY() {
    return posY;
}

Sprite* EnemySprite::GetSprite(){
    return eSprite;
}

