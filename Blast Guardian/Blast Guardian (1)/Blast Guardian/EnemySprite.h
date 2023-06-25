//-----------------------------------------------------------------
// 
// Enemy Sprite Object
// C++ Header - AlienSprite.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Sprite.h"

//-----------------------------------------------------------------
// EnemySprite Class
//-----------------------------------------------------------------
class EnemySprite : public Sprite
{
public:
  // Constructor(s)/Destructor
          EnemySprite(Bitmap* pBitmap, RECT& rcBounds,
              BOUNDSACTION baBoundsAction = BA_STOP);
  virtual ~EnemySprite();

  //Variables
  Sprite* eSprite;
  int posX, posY;

  // General Methods
  virtual SPRITEACTION  Update();
  void SetEnemyPos(int x, int y);
  void SetVelocity(int x, int y);
  void SetPosition(int x, int y);
  int GetPosX();
  int GetPosY();
  Sprite* GetSprite();

};



