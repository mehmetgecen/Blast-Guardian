//-----------------------------------------------------------------
// Space Out Application
// C++ Source - SpaceOut.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "SpaceOut.h"


//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    _pGame = new GameEngine(hInstance, TEXT("Space Out"),
        TEXT("Space Out"), IDI_SPACEOUT, IDI_SPACEOUT_SM, 21 * 50, 15 * 50);
    if (_pGame == NULL)
        return FALSE;

    // Set the frame rate
    _pGame->SetFrameRate(30);

    // Store the instance handle
    _hInstance = hInstance;

    return TRUE;
}


void GameStart(HWND hWindow)
{
    // Seed the random number generator
    srand(GetTickCount64());
    seed = rand();
    srand(1136);
    // Create the offscreen device context and bitmap
    _hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
    _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
        _pGame->GetWidth(), _pGame->GetHeight());
    SelectObject(_hOffscreenDC, _hOffscreenBitmap);

    // Create and load the bitmaps
    HDC hDC = GetDC(hWindow);

    _pBackgroundBitmap = new Bitmap(hDC, IDB_BACKGROUND, _hInstance);
    granadeBitmap = new Bitmap(hDC, IDB_BOMB, _hInstance);
    wallbmp = new Bitmap(hDC, IDB_WALL, _hInstance);
    playerBitmap = new Bitmap(hDC, IDB_PLAYER, _hInstance);
    enemyBitmap = new Bitmap(hDC, IDB_ENEMY, _hInstance);
    friendBitmap = new Bitmap(hDC, IDB_FRIEND, _hInstance);
    heartBitmap = new Bitmap(hDC, IDB_HEART, _hInstance);
    keyBitmap = new Bitmap(hDC, IDB_KEY, _hInstance);
    _pSmExplosionBitmap = new Bitmap(hDC, IDB_SMEXPLOSION, _hInstance);
    _pLgExplosionBitmap = new Bitmap(hDC, IDB_LGEXPLOSION, _hInstance);
    _pGameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, _hInstance);
    _pNextLevelBitmap = new Bitmap(hDC, IDB_NEXTLEVEL, _hInstance);

    _iScore = 0;
    _iNumLives = 3;
    _iDifficulty = 0;
     // Play the background music
     //_pGame->PlayMIDISong(TEXT("Music.mid"));

     // Start the game
    NewGame();
}

void GameEnd()
{
    // Close the MIDI player for the background music
    _pGame->CloseMIDIPlayer();

    // Cleanup the offscreen device context and bitmap
    DeleteObject(_hOffscreenBitmap);
    DeleteDC(_hOffscreenDC);

    delete granadeBitmap;
    delete wallbmp;
    delete enemyBitmap;
    delete playerBitmap;
    delete friendBitmap;
    delete _pSmExplosionBitmap;
    delete _pLgExplosionBitmap;
    delete _pGameOverBitmap;

    // Cleanup the background
    delete _pBackgroundBitmap;

    // Cleanup the sprites
    _pGame->CleanupSprites();

    // Cleanup the game engine
    delete _pGame;
}

void GameActivate(HWND hWindow)
{
    // Resume the background music
    _pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
    // Pause the background music
    _pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
    // Draw the background
    _pBackgroundBitmap->Draw(hDC, 0, 0);

    // Draw the sprites
    _pGame->DrawSprites(hDC);

    // Draw the score
    TCHAR szText[64];
    RECT  rect = { 460, 0, 510, 30 };
    wsprintf(szText, "%d", _iScore);
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(255, 255, 255));
    DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

    // Draw the number of remaining lives (cars)
    for (int i = 0; i < _iNumLives; i++)
      heartBitmap->Draw(hDC, 520 + (heartBitmap->GetWidth() * i),
        10, TRUE);

        // Draw the game over message, if necessary
    if (_bGameOver)
        _pGameOverBitmap->Draw(hDC, 375, 525, TRUE);
    if (_bNextLevel)
        _pNextLevelBitmap->Draw(hDC, 375, 525, TRUE);
    char str[50];
    sprintf(str, "%d,%d", playerx, playery);
    TextOut(hDC, 0, 0, str, strlen(str));
}

void GameCycle()
{
    if (!_bGameOver && !_bNextLevel)
    {
        if (playerx == 20 && playery == 13) {
            _bNextLevel = TRUE;
        }
        bfrmEnemy -= stepEnemy;
        if (bfrmEnemy <= 0) {
            MoveEnemies();
            //EnemyAttack();
            bfrmEnemy = 50;
        }

        bfrmFriend -= stepFriend;
        if (bfrmFriend <= 0) {
            MoveFriend();
            bfrmFriend = 50;
        }

        if (++_iFireInputDelay >= fireRate && isPlanted) {
            granadeSprite->Kill();
            isPlanted = FALSE;
        }

          // Update the sprites
        _pGame->UpdateSprites();

        // Obtain a device context for repainting the game
        HWND  hWindow = _pGame->GetWindow();
        HDC   hDC = GetDC(hWindow);

        // Paint the game to the offscreen device context
        GamePaint(_hOffscreenDC);

        // Blit the offscreen bitmap to the game screen
        BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
            _hOffscreenDC, 0, 0, SRCCOPY);

        // Cleanup
        ReleaseDC(hWindow, hDC);
    }
}
void MoveEnemies()
{
    for (enemy = 0; enemy < maxenemy; enemy++)
    {   
        EnemySprite* selectedEnemySprite = allEnemies[enemy];

        // Get the current enemy position
        int enemyx = selectedEnemySprite->GetPosX();
        int enemyy = selectedEnemySprite->GetPosY();

        // Check if a valid sprite is selected
        if (selectedEnemySprite != nullptr) {
            // Determine the direction for the enemy to move
            int direction = rand() % 4;  // 0: left, 1: right, 2: up, 3: down

            // Update the enemy position based on the chosen direction
            if (direction == 0 && enemyx > 0 && map[enemyy][enemyx - 1] == 0) //enemy 1 step downwards
            {
                map[enemyy][enemyx] = 0;
                // Update the enemy position
                enemyx--;
                selectedEnemySprite->SetVelocity(-stepEnemy, 0);
            }
            else if (direction == 1 && enemyx < maxcol - 1 && map[enemyy][enemyx + 1] == 0)
            {
                map[enemyy][enemyx] = 0;
                // Update the enemy position
                enemyx++;
                selectedEnemySprite->SetVelocity(stepEnemy, 0);
            }
            else if (direction == 2 && enemyy > 0 && map[enemyy - 1][enemyx] == 0)
            {
                map[enemyy][enemyx] = 0;
                // Update the enemy position
                enemyy--;
                selectedEnemySprite->SetVelocity(0, -stepEnemy);
            }
            else if (direction == 3 && enemyy < maxrow - 1 && map[enemyy + 1][enemyx] == 0)
            {
                map[enemyy][enemyx] = 0;
                // Update the enemy position
                enemyy++;
                selectedEnemySprite->SetVelocity(0, stepEnemy);
            }
            else {
                selectedEnemySprite->SetVelocity(0, 0);
            }
            // Update the enemy position
            selectedEnemySprite->SetEnemyPos(enemyx, enemyy);
        }
    }
}

void MoveFriend() 
{
    friendSprite->SetVelocity(0, 0);
    int friendDirection = rand() % 4;
    if (friendDirection == 0 && friendx > 0 && map[friendy][friendx - 1] == 0)
    {
        map[friendy][friendx] = 0;
        // Move left 
        friendx--;
        friendSprite->SetVelocity(-stepFriend, 0);
    }
    else if (friendDirection == 1 && friendx < maxcol - 1 && map[friendy][friendx + 1] == 0)
    {
        map[friendy][friendx] = 0;
        // Move right
        friendx++; 
        friendSprite->SetVelocity(stepFriend, 0);
    }
    else if (friendDirection == 2 && friendy > 0 && map[friendy - 1][friendx] == 0) {

        map[friendy][friendx] = 0;
        //Mover Upward
        friendy--; 
        friendSprite->SetVelocity(0, -stepFriend);
    }
    else if (friendDirection == 3 && friendy < maxrow - 1 && map[friendy + 1][friendx] == 0) {
        map[friendy][friendx] = 0;
        //Move Downward
        friendy++;
        friendSprite->SetVelocity(0, stepFriend);
    }
    else {
        friendSprite->SetVelocity(0, 0);
    }
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

void HandleKeys()
{
    if (!_bGameOver)
    {
        RECT  rcBounds = { 0, 0, 21 * 50, 15 * 50 };
        bfrm -= step;
        if (bfrm <= 0) {
            playerSprite->SetVelocity(0, 0);
            
            if (playerx > 0 && map[playery][playerx - 1] == 0 && GetAsyncKeyState(VK_LEFT) < 0)
            {
                map[playery][playerx] = 0;
                // Move left 
                playerx--;
                playerSprite->SetVelocity(-step, 0); bfrm = 50;
                angle = 1;
            }
            else if (playerx < maxcol - 1 && map[playery][playerx + 1] == 0 && GetAsyncKeyState(VK_RIGHT) < 0)
            {
                map[playery][playerx] = 0;
                // Move right
                playerx++; playerSprite->SetVelocity(step, 0); bfrm = 50;
                angle = 2;
            }
            else if (playery > 0 && map[playery - 1][playerx] == 0 && GetAsyncKeyState(VK_UP) < 0) {

                map[playery][playerx] = 0;
                //Mover Upward
                playery--; playerSprite->SetVelocity(0, -step); bfrm = 50;
                angle = 3;
            }
            else if (playery < maxrow - 1 && map[playery + 1][playerx] == 0 && GetAsyncKeyState(VK_DOWN) < 0) {
                map[playery][playerx] = 0;
                //Move Downward
                playery++; playerSprite->SetVelocity(0, step); bfrm = 50;
                angle = 4;
            }

            // Fire missiles based upon spacebar presses
            else if (!isPlanted && (++_iFireInputDelay > fireRate) && GetAsyncKeyState(VK_SPACE) < 0)
            {
                granadeSprite = new Sprite(granadeBitmap, rcBounds, BA_DIE);
                granadeSprite->SetVelocity(0, 0);
                // Create a new missile sprite
                RECT  rcPos = playerSprite->GetPosition();
                if (angle == 0) {//Down firing
                    granadex = rcPos.left + 25;
                    granadey = rcPos.bottom;
                    granadeSprite->SetPosition(granadex, granadey);
                    granadeSprite->SetVelocity(0, granadestep);
                }
                else if (angle == 1) {//Left firing
                    granadex = rcPos.left;
                    granadey = rcPos.bottom - 25;
                    granadeSprite->SetPosition(granadex, granadey);
                    granadeSprite->SetVelocity(-granadestep, 0);
                }
                else if (angle == 2) {//Right firing
                    granadex = rcPos.right;
                    granadey = rcPos.top + 25;
                    granadeSprite->SetPosition(granadex, granadey);
                    granadeSprite->SetVelocity(granadestep, 0);
                }
                else if (angle == 3) {//Up firing
                    granadex = rcPos.left + 25;
                    granadey = rcPos.top;
                    granadeSprite->SetPosition(granadex, granadey);
                    granadeSprite->SetVelocity(0, -granadestep);

                }
                else if (angle == 4) {//Down firing
                    granadex = rcPos.left + 25;
                    granadey = rcPos.bottom;
                    granadeSprite->SetPosition(granadex, granadey);
                    granadeSprite->SetVelocity(0, granadestep);

                }

                _pGame->AddSprite(granadeSprite);
                isPlanted = TRUE;
                // Play the missile (fire) sound
                PlaySound((LPCSTR)IDW_MISSILE, _hInstance, SND_ASYNC |
                    SND_RESOURCE | SND_NOSTOP);

                // Reset the input delay
                _iFireInputDelay = 0;
            }
        }
    }

    // Start a new game based upon an Enter (Return) key press
    if (_bGameOver && (GetAsyncKeyState(VK_RETURN) < 0)) {
        // Start a new game

        _iDifficulty = 0;
        maxenemy = 5;
        _iNumLives = 3;
        NewGame();
    }

    if (_bNextLevel && (GetAsyncKeyState(VK_RETURN) < 0)) {
        _iDifficulty += 2;
        NewGame();
    }
}

void RechargeEnemy(EnemySprite* sprite)
{
    RECT rcBounds = { 0, 0, 21 * 50, 15 * 50 };
    while (true) {
        int enemyx = rand() % maxcol;
        int enemyy = rand() % maxrow;

        if (map[enemyy][enemyx] == 0) {
            
            EnemySprite* eSprite = sprite;
            eSprite->SetEnemyPos(enemyx, enemyy);
            eSprite->SetPosition(enemyx * 50, enemyy * 50);

            // Update the map
            map[enemyy][enemyx] = 99;
            break;
        }
    }

}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
    // See if a player missile and an alien have collided
    Bitmap* pHitter = pSpriteHitter->GetBitmap();
    Bitmap* pHittee = pSpriteHittee->GetBitmap();
    if (((pHitter == wallbmp || pHitter == enemyBitmap) && (pHittee == granadeBitmap)) ||
        ((pHittee == wallbmp || pHittee == enemyBitmap) && (pHitter == granadeBitmap)))
    {
        // Play the small explosion sound
        PlaySound((LPCSTR)IDW_LGEXPLODE, _hInstance, SND_ASYNC |
            SND_RESOURCE);

        // Kill both sprites
        if ((pHitter == enemyBitmap && pHittee == granadeBitmap) || (pHitter == granadeBitmap && pHittee == enemyBitmap)) {
            RECT hitte = pSpriteHittee->GetPosition();

            //check enemy sprite for killing
            if (pHitter == enemyBitmap)
                pSpriteHittee->Kill();
            else
                pSpriteHitter->Kill();

            //enemyNumber--;
            for (size_t i = 0; i < maxenemy; i++)
            {
                if (pSpriteHitter == allEnemies[i]->GetSprite() || pSpriteHittee == allEnemies[i]->GetSprite()) {
                    RechargeEnemy(allEnemies[i]);
                    break;
                }
            }
            _iScore += 25;

            if (enemyNumber == 0)
            {
                _bNextLevel = true;
            }

            // Create a large explosion sprite at the alien's position
            RECT rcBounds = { 0, 0, 21 * 50, 15 * 50 };
            RECT rcPos;
            if (pHitter == granadeBitmap)
            {
                rcPos = pSpriteHittee->GetPosition();
                map[rcPos.top / 50][rcPos.left / 50] = 0;
            }
            else {
                rcPos = pSpriteHitter->GetPosition();
                map[rcPos.top / 50][rcPos.left / 50] = 0;
            }
            //map[rcPos.top % 50][rcPos.left % 50] = 0;
            Sprite* pSprite = new Sprite(_pLgExplosionBitmap, rcBounds);
            pSprite->SetNumFrames(8, TRUE);

            pSprite->SetPosition(rcPos.left, rcPos.top);
            
            isPlanted = FALSE;
            _iFireInputDelay = 30;
            _pGame->AddSprite(pSprite);
        }
        else {
            if (pHitter == granadeBitmap)
            {
                pSpriteHitter->Kill();
            }
            else pSpriteHittee->Kill();
            
            isPlanted = FALSE;
            _iFireInputDelay = 30;
        }
    }
    else if(((pHitter == playerBitmap || pHitter == friendBitmap) && (pHittee == enemyBitmap)) || ((pHittee == playerBitmap|| pHittee == friendBitmap) && (pHitter == enemyBitmap)))
    {
        RECT hitte = pSpriteHittee->GetPosition();
        

        if ((pHitter == playerBitmap && pHittee == enemyBitmap) || (pHitter == enemyBitmap && pHittee == playerBitmap)) {
            playerSprite->SetPosition(50, 50);
            playerx = 1;
            playery = 1;
            playerSprite->SetVelocity(0, 0);
            bfrm = 0;
            angle = 0;
            if (--_iNumLives == 0)
            {
                // Play the game over sound
                PlaySound((LPCSTR)IDW_GAMEOVER, _hInstance, SND_ASYNC |
                    SND_RESOURCE);
                _bGameOver = TRUE;
            }
        }
        else 
        {
            if (pHitter == playerBitmap || pHitter == friendBitmap)
            {
                pSpriteHitter->Kill();
            }
            else pSpriteHittee->Kill();

            // Play the game over sound
            PlaySound((LPCSTR)IDW_GAMEOVER, _hInstance, SND_ASYNC |
                SND_RESOURCE);
            _bGameOver = TRUE;
        }

        
    }
    else if ((pHitter == playerBitmap && pHittee == keyBitmap) || (pHitter == keyBitmap && pHittee == playerBitmap)) {
        if (pHitter == keyBitmap)
        {
            pSpriteHitter->Kill();
        }
        else pSpriteHittee->Kill();
        Sprite* exitSprite = _pGame->IsPointInSprite(20 * 50, 13 * 50);
        map[13][20] = 0;
        exitSprite->Kill();
    }

    return FALSE;
}

void SpriteDying(Sprite* pSprite)
{
    // See if an alien missile sprite is dying
    if (pSprite->GetBitmap() == granadeBitmap)
    {
        // Play the small explosion sound
        PlaySound((LPCSTR)IDW_SMEXPLODE, _hInstance, SND_ASYNC |
            SND_RESOURCE | SND_NOSTOP);

        
        RECT granadePosition = pSprite->GetPosition();
        bombx = granadePosition.left / 50;
        bomby = granadePosition.top / 50;
        for (int i = bombx - 1; i < maxcol && i <= bombx + 1; i++)
        {
            if (i > 0) {
                for (int j = bomby - 1; j < maxrow && j <= bomby + 1; j++)
                {
                    if (j > 0) {
                        if (map[j][i] > 100 || map[j][i] ==99) {
                            Sprite* tSprite = _pGame->IsPointInSprite(i * 50, j * 50);
                            map[j][i] = 0;
                            tSprite->Kill();
                        }
                    }
                }
            }
        }
        granadeSprite->SetVelocity(0, 0);
        // Create a small explosion sprite at the missile's position
        RECT rcBounds = { 0, 0, 21 * 50, 15 * 50 };
        RECT rcPos = pSprite->GetPosition();
        Sprite* pSprite = new Sprite(_pSmExplosionBitmap, rcBounds);
        pSprite->SetNumFrames(8, TRUE);
        pSprite->SetPosition(rcPos.left, rcPos.top);
        _pGame->AddSprite(pSprite);
    }
}

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
void NewGame()
{
    // Clear the sprites
    _pGame->CleanupSprites();

    //Create Map
    CreateMap();

    // Randomly add aliens
    /*if ((rand() % _iDifficulty) == 0)
        AddAlien();*/

    //Create Enemysprite array
    maxenemy += _iDifficulty;
    allEnemies = new EnemySprite * [maxenemy];
    for (int i = 0; i < maxenemy; i++)
    {
        allEnemies[i] = NULL;
    }
    enemyNumber = maxenemy;
    // Create the golf ball sprites
    RECT    rcBounds = { 0, 0, 21 * 50, 15 * 50 };

    playerSprite = new Sprite(playerBitmap, rcBounds, BA_STOP);
    playerx = 1;
    playery = 1;
    bfrm = 0;
    angle = 0;
    playerSprite->SetPosition(50, 50);
    playerSprite->SetVelocity(0, 0);
    map[playery][playerx] = 98;
    _pGame->AddSprite(playerSprite);
    //Adding Friend
    do
    {
        friendx = rand() % maxcol;
        friendy = rand() % maxrow;
        if (map[friendy][friendx] == 0) {
            map[friendy][friendx] = 97;
            break;
        }
    } while (true);
    //Adding Friend Sprite
    for (int i = 0; i < maxrow; i++) {
        for (int j = 0; j < maxcol; j++) {
            if (map[i][j] == 97) {
                friendSprite = new Sprite(friendBitmap, rcBounds, BA_STOP);
                friendx = j;
                friendy = i;
                friendSprite->SetPosition(j * 50, i * 50);
                // Add the friend sprite
                _pGame->AddSprite(friendSprite);
                //map[i][j] = 0;
            }
        }
    }

    //Adding Enemies
    AddEnemy();
    // Initialize the game variables
    _iFireInputDelay = 30;
    isPlanted = FALSE;
    _bGameOver = FALSE;
    _bNextLevel = FALSE;
    // Play the background music
    //_pGame->PlayMIDISong();
}

void AddEnemy()
{
    // Create a new random enemy sprite
    RECT          rcBounds = { 0, 0, 21 * 50, 15 * 50 };
    BOOL putEnemy = TRUE;

    //Random enemy spawn
    for (enemy = 0; enemy < maxenemy; enemy++) {
        putEnemy = TRUE;
        int enemyx = rand() % maxcol;
        int enemyy = rand() % maxrow;
        for (int i = enemyx - 4; i < maxcol && i < enemyx + 4; i++)
        {
            if (i > 0) {
                for (int j = enemyy - 4; j < maxrow && j < enemyy + 4; j++)
                {
                    if (j > 0) {
                        if (map[j][i] != 0 && map[j][i] <= 98)
                            putEnemy = FALSE;
                    }
                }
            }
        }
        if (putEnemy && map[enemyy][enemyx] == 0) {
            map[enemyy][enemyx] = 99;
        }
        else enemy--;
    }
    enemy = 0;
    for (int i = 0; i < maxrow; i++) {
        for (int j = 0; j < maxcol; j++) {
            if (map[i][j] == 99) {
                EnemySprite* eSprite = new EnemySprite(enemyBitmap, rcBounds, BA_STOP);
                eSprite->SetEnemyPos(j, i);
                eSprite->SetPosition(j * 50, i * 50);
                allEnemies[enemy] = eSprite;
                // Add the enemy sprite
                _pGame->AddSprite(eSprite->GetSprite());
                //map[i][j] = 0;
                enemy++;
            }
            else if (map[i][j] <= 98)
                map[i][j] = 0;
        }
    }
}

void CreateMap() {

    
    path = new coord * [1000];
    //array
    map = new int* [maxrow];

    RECT    rcBounds = { 0, 0, 21 * 50, 15 * 50 };
    for (int i = 0; i < maxrow; i++) map[i] = new int[maxcol];
    //base map
    for (int j = 0; j < maxrow; j++) {
        for (int k = 0; k < maxcol; k++) {
            map[j][k] = 0;
            if (j == 0 || j == maxrow - 1 || k == 0 || k == maxcol - 1) map[j][k] = 100;
            if ((j % 2 == 0 && k % 2 == 0)&& (j != 0 && j != maxrow - 1 && k != 0 && k != maxcol - 1)) map[j][k] = 101;
        }
    }
    int key = rand() % maxwall * .65;
    //additional wall
    for (wall = 0; wall < maxwall * .65; wall++) {
        int x, y, x1, x2, y1, y2;
        BOOL wallok;
        coord* c1, * c2;
        
        wallok = FALSE;
        do {
            x = (rand() % (maxcol - 2)) + 1;
            y = (rand() % (maxrow - 2)) + 1;

            if ((x % 2 == 0 && y % 2 == 1) || (x % 2 == 1 && y % 2 == 0)) {
                if (x % 2 == 1 && y % 2 == 0) { x1 = x2 = x; y1 = y - 1; y2 = y + 1; }
                if (x % 2 == 0 && y % 2 == 1) { y1 = y2 = y; x1 = x - 1; x2 = x + 1; }
                if (map[y][x] == 0) {
                    map[y][x] = 101;
                    wallok = TRUE;
                    
                    c1 = new coord(x1, y1);
                    c2 = new coord(x2, y2);
                    if (findpathBFS(c1, c2, map, maxrow, maxcol, 0, path) < 0) {
                        map[y][x] = 0;//no path from c1 to c2 so remove wall
                        wallok = FALSE;
                    }
                }
            }
        } while (wallok == FALSE);
        if (wall == key) {
            keyX = x;
            keyY = y;
            keySprite = new Sprite(keyBitmap, rcBounds, BA_STOP);
            keySprite->SetPosition(6 * 50, keyX * 50);
            keySprite->SetZOrder(0);
            _pGame->AddSprite(keySprite);
        }
    }
    //Draw Walls
    for (int j = 0; j < maxrow; j++) {
        for (int k = 0; k < maxcol; k++) {
            if (map[j][k] >= 100) {
                wallSprite = new Sprite(wallbmp, rcBounds, BA_STOP);
                wallSprite->SetPosition(k * 50, j * 50);
                _pGame->AddSprite(wallSprite);
            }
        }
    }
}

int findpathBFS(coord* c1, coord* c2, int** map, int maxrow, int maxcol, int empty, coord** path) {
    //c1:start c2:end
    int** map2;
    queue<coord*> alt;
    coord* c, * ct;

    //alloc map2 array
    map2 = new int* [maxrow];
    for (int i = 0; i < maxrow; i++) map2[i] = new int[maxcol];
    //copy map
    for (int j = 0; j < maxrow; j++) {
        for (int k = 0; k < maxcol; k++) {
            map2[j][k] = map[j][k];

        }
    }

    alt.push(c1);
    while (!alt.empty()) {
        c = (coord*)alt.front(); alt.pop();
        if (map2[c->y][c->x] != empty) continue;

        map2[c->y][c->x] = 1; //tried block

        if (distance(c, c2) <= 1) {
            int i = 0;
            while (c->prev != NULL) {
                path[i] = c;
                c = c->prev;
                i++;
            }

            return i;
        }

        if (c->x + 1 < maxcol && map2[c->y][c->x + 1] == 0) alt.push(new coord(c->x + 1, c->y, c->dist + 1, c));
        if (c->x - 1 >= 0 && map2[c->y][c->x - 1] == 0) alt.push(new coord(c->x - 1, c->y, c->dist + 1, c));
        if (c->y + 1 < maxrow && map2[c->y + 1][c->x] == 0) alt.push(new coord(c->x, c->y + 1, c->dist + 1, c));
        if (c->y - 1 >= 0 && map2[c->y - 1][c->x] == 0) alt.push(new coord(c->x, c->y - 1, c->dist + 1, c));
    }

    return -1;
}


