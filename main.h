#ifndef MAIN_H
#define MAIN_H

#include <allegro.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "Object.h"
#include "Enemy.h"

using namespace std;

BITMAP* buffer;
BITMAP* character;
BITMAP* map;
BITMAP* fullMap;
BITMAP* minimap;
BITMAP* maphighlight;
BITMAP* itemsBox;
BITMAP* statsBox;
BITMAP* notificationsBox;
BITMAP* enemyPic;
BITMAP* swordPic;
BITMAP* chestPic;
BITMAP* helmetPic;
BITMAP* legsPic;
BITMAP* bootsPic;
BITMAP* shieldPic;
BITMAP* bowPic;

int tileNum;
char charName[50] = "Player name";

bool needUpdate = true;
bool hasObject = false;
bool isAttacking = false;

int x = 320, y = 240, sx = 34, sy, cx, cy; //char's x coord, char's y coord, x pixel of the sprite image, y pixel of sprite image (chooses which sprite to draw of the chunk), change in x, change in y (number of pixels sprite will move)
int csx = 34; //used to cycle through walk animation

int row, column;
int mapx = 0, mapy = 0;
int maptiles[15][20];

int enemyNum;

vector<Object> playersObjects;
vector<Enemy> myEnemies;

int health = 100, strength = 0, defense = 0, XP = 0, level = 1;

FONT *Impact20;
FONT *Calibri20;
FONT *Calibri16;
FONT *font;

string currentObject = "SWORD";
string message1 = "";
string message2 = "";
string message3 = "";
string message4 = "";
string messageNew;

class main {

public:
  main();
  virtual ~main();

protected:
  //Signal handlers:
  void loadObjects();
  void drawMap();
  void drawMiniMap();
  void loadMap();
  void drawChar();
  void drawExtras();
  void Draw();
  void timer_handler();
  void loadChar();
  void newChar();
  void saveChar();
  void login();
  DIALOG chooseLoginDialog[];
  DIALOG createCharDialog[];
  DIALOG loginCharDialog[];

  //Member widgets:
};

#endif // MAIN_H
