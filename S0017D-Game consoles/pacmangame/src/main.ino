#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>
#include "sprite_assets.h"
#define wall 23
#define floor 0
#define right 0
#define left 1
#define up 2
#define down 3


struct tile
{
  int x;
  int y;
  double distance;
};
int paintMap[17][29] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
                        {1,2,1,1,1,0,1,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1,2,1},
                        {1,0,1,1,1,0,1,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1},
                        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,1,0,1,1,1,0,1},
                        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
                        {1,1,1,1,1,0,1,0,1,1,0,1,1,1,9,1,1,1,0,1,1,0,1,0,1,1,1,1,1},
                        {1,1,1,1,1,0,0,0,1,1,0,1,1,3,3,3,1,1,0,1,1,0,0,0,1,1,1,1,1},
                        {1,1,1,1,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,1,0,1,1,1,1,1},
                        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
                        {1,0,1,1,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,1,0,1,1,1,0,1},
                        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,1,0,1,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1},
                        {1,2,1,1,1,0,1,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1,2,1},
                        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};



int score = 0;
int frame = 0;
int scatterFrame = 0;

//---Pacman vars
int pacRight[3] = {0,1,2};
int pacLeft[3] = {14,15,2};
int pacUp[3] = {28,29,2};
int pacDown[3] = {42,43,2};
int pacDir = left;
int pacAnim = 0;
int pacTimes = 1;
int pacX = 14;
int pacY = 10;
int lives = 3;
int ySpeed = 0;
int xSpeed = 0;

//--shared spooky stuff
int ghostAnim = 0;
bool scatter = false;
int ghostScatterAnim[2] = {64, 65};

//---Blinky vars
int blinkX = 14;
int blinkY = 6;
int blinkRight[2] = {56,57};
int blinkLeft[2] = {58,59};
int blinkUp[2] = {60,61};
int blinkDown[2] = {62,63};
int blinkDir = left;
int blinkYSpeed = 0;
int blinkXSpeed = -1;
tile blinkScatter;

//---inky vars
int pinkX = 14;
int pinkY = 8;
int pinkRight[2] = {70,71};
int pinkLeft[2] = {72,73};
int pinkUp[2] = {74,75};
int pinkDown[2] = {76,77};
int pinkDir = down;
int pinkYSpeed = -1;
int pinkXSpeed = 0;
tile pinkScatter;

//---inky vars
int inkX = 13;
int inkY = 8;
int inkRight[2] = {84,85};
int inkLeft[2] = {86,87};
int inkUp[2] = {88,89};
int inkDown[2] = {90,91};
int inkDir = up;
int inkYSpeed = -1;
int inkXSpeed = 0;
bool inkyActive = false;
tile inkScatter;

//---clyde vars
int clydeX = 15;
int clydeY = 8;
int clydeRight[2] = {98,99};
int clydeLeft[2] = {100,101};
int clydeUp[2] = {102,103};
int clydeDown[2] = {104,105};
int clydeDir = up;
int clydeYSpeed = -1;
int clydeXSpeed = 0;
bool clydeActive = false;
tile clydeScatter;

void setup()
{
  GD.begin();
  LOAD_ASSETS();
  blinkScatter.x = 28;
  blinkScatter.y = -2;

  pinkScatter.x = 1;
  pinkScatter.y = -2;

  inkScatter.x = 28;
  inkScatter.y = 20;

  clydeScatter.x = 0;
  clydeScatter.y = 20;
}

void loop()
{



  if (scatter && ((frame - scatterFrame) > 35))
  {
    scatter = false;
  }

  //Take int analog input to change pacman position
  int x = analogRead(8);
  int y = analogRead(9);
  movePac(x,y);

  //Move pacman sprite if legal
  movePacColl();
  GD.Clear();
  //Animate pacman
  animPac();
  animGhosts();
if (scatter)
{
  if (frame%2 == 0)
  {
    blinkAI();
    pinkAI();

    if (((score/10) > 15) && !inkyActive)
    {
      inkX = 14;
      inkY =  8;
      inkyActive = true;
    }
    if (((score/10) > 221/3) && !clydeActive )
    {
      clydeX = 14;
      clydeY =  8;
      clydeActive = true;
    }
    if (inkyActive)
    {
      inkAI();
    }
    if (clydeActive)
    {
      clydeAI();
    }
  }
}
else
{
  blinkAI();
  pinkAI();

  if (((score/10) > 15) && !inkyActive)
  {
    inkX = 14;
    inkY =  8;
    inkyActive = true;
  }
  if (((score/10) > 221/3) && !clydeActive )
  {
    clydeX = 14;
    clydeY =  8;
    clydeActive = true;
  }
  if (inkyActive)
  {
    inkAI();
  }
  if (clydeActive)
  {
    clydeAI();
  }
}

  GD.Begin(BITMAPS);
  //Paint the map
  setupMap();


  //paint lives
  paintLives();
  //Rotate and draw pacman
  pacRot();
  //Draw ghosts
  blinkRot();
  pinkRot();
  inkRot();
  clydeRot();

  GD.cmd_text(16*2, 8*16, 16, OPT_CENTER, "Score:");
  GD.cmd_text(2*16, 16*16, 16, OPT_CENTERX, "Lives:");
  GD.cmd_number(16*2, 9*16, 16, OPT_CENTER, score);
  //GD.cmd_text(240, 170, 31, OPT_CENTER, "Y:");
  //GD.cmd_number(301, 170, 31, OPT_CENTER, y);
  if (lives < 1)
  {
    GD.cmd_text(14*16, 8*13, 31, OPT_CENTERX, "GAME");
    GD.cmd_text(14*16, 9*15, 31, OPT_CENTERX, "OVER");
  }
  else if (score/10 == 221)
  {
    GD.cmd_text(14*16, 8*13, 31, OPT_CENTERX, "VICTORY");
  }
  delay(200);
  GD.swap();
  if (lives < 1)
  {
    delay(1000000);
  }
  else if (score/10 == 221)
  {
    delay(1000000);
  }
  frame++;
}

void reset()
{

  pacX = 14;
  pacY = 10;
  pacDir = left;
  xSpeed = 0;
  ySpeed = 0;

  blinkX = 14;
  blinkY = 6;
  blinkDir = left;
  blinkYSpeed = 0;
  blinkXSpeed = -1;
  blinkDir = left;

  //---inky vars
  pinkX = 14;
  pinkY = 8;
  pinkDir = down;
  pinkYSpeed = -1;
  pinkXSpeed = 0;
  pinkDir = down;

  //---inky vars
  inkX = 14;
  inkY = 8;
  inkYSpeed = -1;
  inkXSpeed = 0;
  inkDir = up;

  //---clyde vars
  clydeX = 14;
  clydeY = 8;
  clydeYSpeed = -1;
  clydeXSpeed = 0;
  clydeDir = up;
}


void paintLives()
{
  if (lives == 3) {
    GD.Vertex2ii(4*16, 16*16, 0, 22);
    GD.Vertex2ii(5*16, 16*16, 0, 22);
    GD.Vertex2ii(6*16, 16*16, 0, 22);
  }
  if (lives == 2) {
    GD.Vertex2ii(4*16, 16*16, 0, 22);
  }
  if (lives == 1)
  {
    GD.Vertex2ii(4*16, 16*16, 0, 22);
  }
}
void animGhosts()
{
  if (ghostAnim == 0)
    ghostAnim = 1;
  else
    ghostAnim = 0;
}
void animPac()
{
  pacAnim += 1*pacTimes;
  if (pacAnim > 1)
  {
    //pacAnim = -1;
    pacTimes = -1;
  }
  else if (pacAnim < 1)
  {
    pacTimes = 1;
  }
}
void movePacColl()
{
  if (paintMap[pacY+ySpeed][pacX+xSpeed] != 1)
  {
    pacX += 1 * xSpeed;
    pacY += 1 * ySpeed;

    if ((pacX == blinkX) && (pacY == blinkY))
    {
      if (scatter)
      {
        blinkX = 14;
        blinkY = 6;
        blinkDir = left;
        blinkYSpeed = 0;
        blinkXSpeed = -1;
        blinkDir = left;
      }
      else
      {
        lives--;
        reset();
      }
    }
    else if ((pacX == pinkX) && (pacY == pinkY))
    {
      if (scatter)
      {
        //---inky vars
        pinkX = 14;
        pinkY = 8;
        pinkDir = down;
        pinkYSpeed = -1;
        pinkXSpeed = 0;
        pinkDir = down;
      }
      else
      {
        lives--;
        reset();
      }
    }
    else if ((pacX == inkX) && (pacY == inkY))
    {
      if (scatter)
      {
        //---inky vars
        inkX = 13;
        inkY = 8;
        inkYSpeed = -1;
        inkXSpeed = 0;
        inkDir = up;
      }
      else
      {
        lives--;
        reset();
      }
    }
    else if ((pacX == clydeX) && (pacY == clydeY))
    {
      if (scatter)
      {
        //---clyde vars
        clydeX = 15;
        clydeY = 8;
        clydeYSpeed = -1;
        clydeXSpeed = 0;
        clydeDir = up;
      }
      else
      {
        lives--;
        reset();
      }
    }

    //Collect small pill
    if (paintMap[pacY][pacX] == 0) {
      paintMap[pacY][pacX] = 3;
      score += 10;
    }
    //Collect big pill --POWER UP HERE
    if (paintMap[pacY][pacX] == 2) {
      paintMap[pacY][pacX] = 3;
      scatter = true;
      scatterFrame = frame;
    }
  }
}
void pacRot()
{
  if (pacDir == right) {
    GD.Vertex2ii(16*pacX, 16*pacY, 0, pacRight[pacAnim]);
  }
  if (pacDir == left) {
    GD.Vertex2ii(16*pacX, 16*pacY, 0, pacLeft[pacAnim]);
  }
  if (pacDir == up) {
    GD.Vertex2ii(16*pacX, 16*pacY, 0, pacUp[pacAnim]);
  }
  if (pacDir == down) {
    GD.Vertex2ii(16*pacX, 16*pacY, 0, pacDown[pacAnim]);
  }
}
void blinkRot()
{

  if (scatter)
  {
    GD.Vertex2ii(16*blinkX, 16*blinkY, 0, ghostScatterAnim[ghostAnim]);
  }
  else if (blinkDir == right) {
    GD.Vertex2ii(16*blinkX, 16*blinkY, 0, blinkRight[ghostAnim]);
  }
  else if (blinkDir == left) {
    GD.Vertex2ii(16*blinkX, 16*blinkY, 0, blinkLeft[ghostAnim]);
  }
  else if (blinkDir == up) {
    GD.Vertex2ii(16*blinkX, 16*blinkY, 0, blinkUp[ghostAnim]);
  }
  else if (blinkDir == down) {
    GD.Vertex2ii(16*blinkX, 16*blinkY, 0, blinkDown[ghostAnim]);
  }
}
void pinkRot()
{
  if (scatter)
  {
    GD.Vertex2ii(16*pinkX, 16*pinkY, 0, ghostScatterAnim[ghostAnim]);
  }
  else if (pinkDir == right) {
    GD.Vertex2ii(16*pinkX, 16*pinkY, 0, pinkRight[ghostAnim]);
  }
  else if (pinkDir == left) {
    GD.Vertex2ii(16*pinkX, 16*pinkY, 0, pinkLeft[ghostAnim]);
  }
  else if (pinkDir == up) {
    GD.Vertex2ii(16*pinkX, 16*pinkY, 0, pinkUp[ghostAnim]);
  }
  else if (pinkDir == down) {
    GD.Vertex2ii(16*pinkX, 16*pinkY, 0, pinkDown[ghostAnim]);
  }
}
void inkRot()
{
  if (scatter)
  {
    GD.Vertex2ii(16*inkX, 16*inkY, 0, ghostScatterAnim[ghostAnim]);
  }
  else if (inkDir == right) {
    GD.Vertex2ii(16*inkX, 16*inkY, 0, inkRight[ghostAnim]);
  }
  else if (inkDir == left) {
    GD.Vertex2ii(16*inkX, 16*inkY, 0, inkLeft[ghostAnim]);
  }
  else if (inkDir == up) {
    GD.Vertex2ii(16*inkX, 16*inkY, 0, inkUp[ghostAnim]);
  }
  else if (inkDir == down) {
    GD.Vertex2ii(16*inkX, 16*inkY, 0, inkDown[ghostAnim]);
  }
}
void clydeRot()
{
  if (scatter)
  {
    GD.Vertex2ii(16*clydeX, 16*clydeY, 0, ghostScatterAnim[ghostAnim]);
  }
  else if (clydeDir == right) {
    GD.Vertex2ii(16*clydeX, 16*clydeY, 0, clydeRight[ghostAnim]);
  }
  else if (clydeDir == left) {
    GD.Vertex2ii(16*clydeX, 16*clydeY, 0, clydeLeft[ghostAnim]);
  }
  else if (clydeDir == up) {
    GD.Vertex2ii(16*clydeX, 16*clydeY, 0, clydeUp[ghostAnim]);
  }
  else if (clydeDir == down) {
    GD.Vertex2ii(16*clydeX, 16*clydeY, 0, clydeDown[ghostAnim]);
  }
}
void movePac(int xAnalog, int yAnalog)
{
  if (xAnalog > 600) {
    if (paintMap[pacY][pacX+1] != 1)
    {
      xSpeed = 1;
      ySpeed = 0;
      pacDir = right;
    }
    return;
  }
  if (xAnalog < 400) {
    if (paintMap[pacY][pacX-1] != 1)
    {
      xSpeed = -1;
      ySpeed = 0;
      pacDir = left;
    }
    return;
  }
  if (yAnalog > 600) {
    if ((paintMap[pacY+1][pacX] != 1) && (paintMap[pacY+1][pacX] != 9))
    {
      ySpeed = 1;
      xSpeed = 0;
      pacDir = down;
          }
    return;
  }
  if (yAnalog < 400) {
    if (paintMap[pacY-1][pacX] != 1)
    {
      ySpeed = -1;
      xSpeed = 0;
      pacDir = up;
    }
    return;
  }
}
void setupMap()
{
  for (size_t y = 0; y < 17; y++) {
    for (size_t x = 0; x < 29; x++) {
      if (paintMap[y][x] == 1)
      {
          GD.Vertex2ii(x*16, y*16, 0, wall); // wall
      }
      else if (paintMap[y][x] == 0)
      {
        GD.Vertex2ii(x*16, y*16, 0, 25); //pill
      }
      else if (paintMap[y][x] == 2)
      {
        GD.Vertex2ii(x*16, y*16, 0, 24); // power up
      }
    }
  }
}
//----------------------------------------------------------------------------------------------------------
void clydeAI()
{

  tile targetTile;
  tile nextTile;
  tile upT;
  tile downT;
  tile rightT;
  tile leftT;
  tile mostValidTile;
  int oldSpeedX = clydeXSpeed;
  int oldSpeedY = clydeYSpeed;
  int numValidPaths = 0;
  double prevdist = 0;



  tile clydeTile;
  clydeTile.x = clydeX;
  clydeTile.y = clydeY;
  tile pacTile;
  pacTile.x = pacX;
  pacTile.y = pacY;

  double clydeDist = distance(clydeTile, pacTile);

  if (clydeDist < 8 || scatter == true)
  {
    targetTile.x = clydeScatter.x;
    targetTile.y = clydeScatter.y;
  }
  else
  {
    targetTile.x = pacX;
    targetTile.y = pacY;
  }

  nextTile.x = clydeX + (1 * clydeXSpeed);
  nextTile.y = clydeY + (1 * clydeYSpeed);

  upT.x = nextTile.x;
  upT.y = nextTile.y - 1;
  upT.distance = 10000;

  downT.x = nextTile.x;
  downT.y = nextTile.y + 1;
  downT.distance = 10000;

  rightT.x = nextTile.x + 1;
  rightT.y = nextTile.y;
  rightT.distance = 10000;

  leftT.x = nextTile.x - 1;
  leftT.y = nextTile.y;
  leftT.distance = 10000;

  //Check for avalible paths and close of tile wich ghost is standing on
  if((paintMap[upT.y][upT.x] != 1))
  {
    if ((upT.x == clydeX) && (upT.y == clydeY))
    {
      upT.x = -1;
      upT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      upT.distance = distance(upT, targetTile);
    }
  }
  else
  {
    upT.x = -1;
    upT.y = -1;
  }
  if((paintMap[downT.y][downT.x] != 1) && (paintMap[downT.y][downT.x] != 9))
  {
    if ((downT.x == clydeX) && (downT.y == clydeY))
    {
      downT.x = -1;
      downT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      downT.distance = distance(downT, targetTile);
    }
  }
  else
  {
    downT.x = -1;
    downT.y = -1;
  }
  if((paintMap[rightT.y][rightT.x] != 1))
  {
    if ((rightT.x == clydeX) && (rightT.y == clydeY))
    {
      rightT.x = -1;
      rightT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      rightT.distance = distance(rightT, targetTile);
    }
  }
  else
  {
    rightT.x = -1;
    rightT.y = -1;
  }
  if((paintMap[leftT.y][leftT.x] != 1) )
  {
    if ((leftT.x == clydeX) && (leftT.y == clydeY))
    {
      leftT.x = -1;
      leftT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      leftT.distance = distance(leftT, targetTile);
    }
  }
  else
  {
    leftT.x = -1;
    leftT.y = -1;
  }

  if (numValidPaths > 1)
  {
      mostValidTile.distance = rightT.distance;
      prevdist = mostValidTile.distance;

      clydeDir = right;
      clydeXSpeed = 1;
      clydeYSpeed = 0;

      if (downT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = downT.distance;
        clydeDir = down;
        clydeXSpeed = 0;
        clydeYSpeed = 1;
      }
      if (leftT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = leftT.distance;
        clydeDir = left;
        clydeXSpeed = -1;
        clydeYSpeed = 0;
      }
      if (upT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = upT.distance;
        clydeDir = up;
        clydeXSpeed = 0;
        clydeYSpeed = -1;
      }

    }
    else
    {
      if (upT.x != -1)
      {
        clydeDir = up;
        clydeXSpeed = 0;
        clydeYSpeed = -1;
      }
      else if (leftT.x != -1)
      {
        clydeDir = left;
        clydeXSpeed = -1;
        clydeYSpeed = 0;
      }
      else if (downT.x != -1)
      {
        clydeDir = down;
        clydeXSpeed = 0;
        clydeYSpeed = 1;
      }
      else if (rightT.x != -1)
      {
        clydeDir = right;
        clydeXSpeed = 1;
        clydeYSpeed = 0;
      }
    }
    clydeX += 1 * oldSpeedX;
    clydeY += 1 * oldSpeedY;

    if ((clydeX == pacX) && (clydeY == pacY))
    {
      if (scatter)
      {
          //---clyde vars
          clydeX = 15;
          clydeY = 8;
          clydeYSpeed = -1;
          clydeXSpeed = 0;
          clydeDir = up;
      }
      else
      {
        lives--;
        reset();
      }
    }
}
//----------------------------------------------------------------------------------------------------------

void inkAI()
{
  tile targetTile;
  tile nextTile;
  tile upT;
  tile downT;
  tile rightT;
  tile leftT;
  tile mostValidTile;
  int oldSpeedX = inkXSpeed;
  int oldSpeedY = inkYSpeed;
  int numValidPaths = 0;
  double prevdist = 0;

  if (scatter == true)
  {
    targetTile.x = inkScatter.x;
    targetTile.y = inkScatter.y;
  }
  else
  {
    targetTile.x = pacX + 2 * xSpeed;
    targetTile.y = pacY + 2 * ySpeed;
  }

  targetTile.x = targetTile.x-blinkX;
  targetTile.y = targetTile.y-blinkY;
  targetTile.x = blinkX + targetTile.x * 2;
  targetTile.y = blinkY + targetTile.y * 2;

  nextTile.x = inkX + (1 * inkXSpeed);
  nextTile.y = inkY + (1 * inkYSpeed);

  upT.x = nextTile.x;
  upT.y = nextTile.y - 1;
  upT.distance = 10000;

  downT.x = nextTile.x;
  downT.y = nextTile.y + 1;
  downT.distance = 10000;

  rightT.x = nextTile.x + 1;
  rightT.y = nextTile.y;
  rightT.distance = 10000;

  leftT.x = nextTile.x - 1;
  leftT.y = nextTile.y;
  leftT.distance = 10000;

  //Check for avalible paths and close of tile wich ghost is standing on
  if((paintMap[upT.y][upT.x] != 1))
  {
    if ((upT.x == inkX) && (upT.y == inkY))
    {
      upT.x = -1;
      upT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      upT.distance = distance(upT, targetTile);
    }
  }
  else
  {
    upT.x = -1;
    upT.y = -1;
  }
  if((paintMap[downT.y][downT.x] != 1) && (paintMap[downT.y][downT.x] != 9))
  {
    if ((downT.x == inkX) && (downT.y == inkY))
    {
      downT.x = -1;
      downT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      downT.distance = distance(downT, targetTile);
    }
  }
  else
  {
    downT.x = -1;
    downT.y = -1;
  }
  if((paintMap[rightT.y][rightT.x] != 1))
  {
    if ((rightT.x == inkX) && (rightT.y == inkY))
    {
      rightT.x = -1;
      rightT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      rightT.distance = distance(rightT, targetTile);
    }
  }
  else
  {
    rightT.x = -1;
    rightT.y = -1;
  }
  if((paintMap[leftT.y][leftT.x] != 1) )
  {
    if ((leftT.x == inkX) && (leftT.y == inkY))
    {
      leftT.x = -1;
      leftT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      leftT.distance = distance(leftT, targetTile);
    }
  }
  else
  {
    leftT.x = -1;
    leftT.y = -1;
  }

  if (numValidPaths > 1)
  {
      mostValidTile.distance = rightT.distance;
      prevdist = mostValidTile.distance;

      inkDir = right;
      inkXSpeed = 1;
      inkYSpeed = 0;

      if (downT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = downT.distance;
        inkDir = down;
        inkXSpeed = 0;
        inkYSpeed = 1;
      }
      if (leftT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = leftT.distance;
        inkDir = left;
        inkXSpeed = -1;
        inkYSpeed = 0;
      }
      if (upT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = upT.distance;
        inkDir = up;
        inkXSpeed = 0;
        inkYSpeed = -1;
      }

    }
    else
    {
      if (upT.x != -1)
      {
        inkDir = up;
        inkXSpeed = 0;
        inkYSpeed = -1;
      }
      else if (leftT.x != -1)
      {
        inkDir = left;
        inkXSpeed = -1;
        inkYSpeed = 0;
      }
      else if (downT.x != -1)
      {
        inkDir = down;
        inkXSpeed = 0;
        inkYSpeed = 1;
      }
      else if (rightT.x != -1)
      {
        inkDir = right;
        inkXSpeed = 1;
        inkYSpeed = 0;
      }
    }
    inkX += 1 * oldSpeedX;
    inkY += 1 * oldSpeedY;

    if ((inkX == pacX) && (inkY == pacY))
    {
      if (scatter)
      {
          //---inky vars
          inkX = 13;
          inkY = 8;
          inkYSpeed = -1;
          inkXSpeed = 0;
          inkDir = up;
      }
      else
      {
        lives--;
        reset();
      }
    }
}
//----------------------------------------------------------------------------------------------------------
void pinkAI()
{
  tile targetTile;
  tile nextTile;
  tile upT;
  tile downT;
  tile rightT;
  tile leftT;
  tile mostValidTile;
  int oldSpeedX = pinkXSpeed;
  int oldSpeedY = pinkYSpeed;
  int numValidPaths = 0;
  double prevdist = 0;


  if (scatter == true)
  {
    targetTile.x = pinkScatter.x;
    targetTile.y = pinkScatter.y;
  }
  else
  {
    targetTile.x = pacX + 4 * xSpeed;
    targetTile.y = pacY + 4 * ySpeed;
  }


  nextTile.x = pinkX + (1 * pinkXSpeed);
  nextTile.y = pinkY + (1 * pinkYSpeed);

  upT.x = nextTile.x;
  upT.y = nextTile.y - 1;
  upT.distance = 10000;

  downT.x = nextTile.x;
  downT.y = nextTile.y + 1;
  downT.distance = 10000;

  rightT.x = nextTile.x + 1;
  rightT.y = nextTile.y;
  rightT.distance = 10000;

  leftT.x = nextTile.x - 1;
  leftT.y = nextTile.y;
  leftT.distance = 10000;

  //Check for avalible paths and close of tile wich ghost is standing on
  if((paintMap[upT.y][upT.x] != 1))
  {
    if ((upT.x == pinkX) && (upT.y == pinkY))
    {
      upT.x = -1;
      upT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      upT.distance = distance(upT, targetTile);
    }
  }
  else
  {
    upT.x = -1;
    upT.y = -1;
  }
  if((paintMap[downT.y][downT.x] != 1) && (paintMap[downT.y][downT.x] != 9))
  {
    if ((downT.x == pinkX) && (downT.y == pinkY))
    {
      downT.x = -1;
      downT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      downT.distance = distance(downT, targetTile);
    }
  }
  else
  {
    downT.x = -1;
    downT.y = -1;
  }
  if((paintMap[rightT.y][rightT.x] != 1))
  {
    if ((rightT.x == pinkX) && (rightT.y == pinkY))
    {
      rightT.x = -1;
      rightT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      rightT.distance = distance(rightT, targetTile);
    }
  }
  else
  {
    rightT.x = -1;
    rightT.y = -1;
  }
  if((paintMap[leftT.y][leftT.x] != 1) )
  {
    if ((leftT.x == pinkX) && (leftT.y == pinkY))
    {
      leftT.x = -1;
      leftT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      leftT.distance = distance(leftT, targetTile);
    }
  }
  else
  {
    leftT.x = -1;
    leftT.y = -1;
  }

  if (numValidPaths > 1)
  {
      mostValidTile.distance = rightT.distance;
      prevdist = mostValidTile.distance;

      pinkDir = right;
      pinkXSpeed = 1;
      pinkYSpeed = 0;

      if (downT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = downT.distance;
        pinkDir = down;
        pinkXSpeed = 0;
        pinkYSpeed = 1;
      }
      if (leftT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = leftT.distance;
        pinkDir = left;
        pinkXSpeed = -1;
        pinkYSpeed = 0;
      }
      if (upT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = upT.distance;
        pinkDir = up;
        pinkXSpeed = 0;
        pinkYSpeed = -1;
      }

    }
    else
    {
      if (upT.x != -1)
      {
        pinkDir = up;
        pinkXSpeed = 0;
        pinkYSpeed = -1;
      }
      else if (leftT.x != -1)
      {
        pinkDir = left;
        pinkXSpeed = -1;
        pinkYSpeed = 0;
      }
      else if (downT.x != -1)
      {
        pinkDir = down;
        pinkXSpeed = 0;
        pinkYSpeed = 1;
      }
      else if (rightT.x != -1)
      {
        pinkDir = right;
        pinkXSpeed = 1;
        pinkYSpeed = 0;
      }
    }
    pinkX += 1 * oldSpeedX;
    pinkY += 1 * oldSpeedY;

    if ((pinkX == pacX) && (pinkY == pacY))
    {
      if (scatter)
      {
          //---inky vars
          pinkX = 14;
          pinkY = 8;
          pinkDir = down;
          pinkYSpeed = -1;
          pinkXSpeed = 0;
          pinkDir = down;
      }
      else
      {
        lives--;
        reset();
      }
    }
}

//------------------------------------------------------------------------------------------------------------------------
void blinkAI()
{
  tile targetTile;
  tile nextTile;
  tile upT;
  tile downT;
  tile rightT;
  tile leftT;
  tile mostValidTile;
  int oldSpeedX = blinkXSpeed;
  int oldSpeedY = blinkYSpeed;
  int numValidPaths = 0;
  double prevdist = 0;

  if (scatter == true)
  {
    targetTile.x = blinkScatter.x;
    targetTile.y = blinkScatter.y;
  }
  else
  {
    targetTile.x = pacX;
    targetTile.y = pacY;
  }
  nextTile.x = blinkX + (1 * blinkXSpeed);
  nextTile.y = blinkY + (1 * blinkYSpeed);

  upT.x = nextTile.x;
  upT.y = nextTile.y - 1;
  upT.distance = 10000;

  downT.x = nextTile.x;
  downT.y = nextTile.y + 1;
  downT.distance = 10000;

  rightT.x = nextTile.x + 1;
  rightT.y = nextTile.y;
  rightT.distance = 10000;

  leftT.x = nextTile.x - 1;
  leftT.y = nextTile.y;
  leftT.distance = 10000;

  //Check for avalible paths and close of tile wich ghost is standing on
  if((paintMap[upT.y][upT.x] != 1))
  {
    if ((upT.x == blinkX) && (upT.y == blinkY))
    {
      upT.x = -1;
      upT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      upT.distance = distance(upT, targetTile);
    }
  }
  else
  {
    upT.x = -1;
    upT.y = -1;
  }
  if((paintMap[downT.y][downT.x] != 1) && (paintMap[downT.y][downT.x] != 9))
  {
    if ((downT.x == blinkX) && (downT.y == blinkY))
    {
      downT.x = -1;
      downT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      downT.distance = distance(downT, targetTile);
    }
  }
  else
  {
    downT.x = -1;
    downT.y = -1;
  }
  if((paintMap[rightT.y][rightT.x] != 1))
  {
    if ((rightT.x == blinkX) && (rightT.y == blinkY))
    {
      rightT.x = -1;
      rightT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      rightT.distance = distance(rightT, targetTile);
    }
  }
  else
  {
    rightT.x = -1;
    rightT.y = -1;
  }
  if((paintMap[leftT.y][leftT.x] != 1) )
  {
    if ((leftT.x == blinkX) && (leftT.y == blinkY))
    {
      leftT.x = -1;
      leftT.y = -1;
    }
    else
    {
      numValidPaths += 1;
      leftT.distance = distance(leftT, targetTile);
    }
  }
  else
  {
    leftT.x = -1;
    leftT.y = -1;
  }

  if (numValidPaths > 1)
  {
      mostValidTile.distance = rightT.distance;
      prevdist = mostValidTile.distance;

      blinkDir = right;
      blinkXSpeed = 1;
      blinkYSpeed = 0;

      if (downT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = downT.distance;
        blinkDir = down;
        blinkXSpeed = 0;
        blinkYSpeed = 1;
      }
      if (leftT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = leftT.distance;
        blinkDir = left;
        blinkXSpeed = -1;
        blinkYSpeed = 0;
      }
      if (upT.distance <= mostValidTile.distance)
      {
        prevdist = mostValidTile.distance;
        mostValidTile.distance = upT.distance;
        blinkDir = up;
        blinkXSpeed = 0;
        blinkYSpeed = -1;
      }

    }
    else
    {
      if (upT.x != -1)
      {
        blinkDir = up;
        blinkXSpeed = 0;
        blinkYSpeed = -1;
      }
      else if (leftT.x != -1)
      {
        blinkDir = left;
        blinkXSpeed = -1;
        blinkYSpeed = 0;
      }
      else if (downT.x != -1)
      {
        blinkDir = down;
        blinkXSpeed = 0;
        blinkYSpeed = 1;
      }
      else if (rightT.x != -1)
      {
        blinkDir = right;
        blinkXSpeed = 1;
        blinkYSpeed = 0;
      }
    }
    blinkX += 1 * oldSpeedX;
    blinkY += 1 * oldSpeedY;

    if ((blinkX == pacX) && (blinkY == pacY))
    {
      if (scatter)
      {
          blinkX = 14;
          blinkY = 6;
          blinkDir = left;
          blinkYSpeed = 0;
          blinkXSpeed = -1;
          blinkDir = left;
      }
      else
      {
        lives--;
        reset();
      }
    }
}
double distance(tile from, tile to)
{
  double distx = to.x - from.x;
  double disty = to.y - from.y;
  double dist;

  dist = pow(distx, 2) + pow(disty, 2);
  dist = sqrt(dist);

  return dist;
}
