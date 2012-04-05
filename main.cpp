#include "main.h"
#include "Object.h"
#include "Enemy.h"

//Constants


//Draws the maphighlight over the correct coordinates of the minimap using mapx and mapy.
//Also changes the current item on the map. 2 of the areas don't have items.
void drawMiniMap() {
  draw_sprite(screen, minimap, 640, 0);
  switch (mapx) {
    case -1:
      switch (mapy) {
	case -1:
	  draw_sprite(screen, maphighlight, 650, 130);
	  currentObject = "NOT SWORD";
	  break;
	case 0:
	  draw_sprite(screen, maphighlight, 650, 70);
	  currentObject = "BOW";
	  break;
	case 1:
	  draw_sprite(screen, maphighlight, 650, 10);
	  currentObject = "NOT SWORD";
	  break;
      }
      break;
    case 0:
      switch (mapy) {
	case -1:
	  draw_sprite(screen, maphighlight, 710, 130);
	  currentObject = "CHEST";
	  break;
	case 0:
	  draw_sprite(screen, maphighlight, 710, 70);
	  currentObject = "SWORD";
	  break;
	case 1:
	  draw_sprite(screen, maphighlight, 710, 10);
	  currentObject = "HELMET";
	  break;
      }
      break;
    case 1:
      switch (mapy) {
	case -1:
	  draw_sprite(screen, maphighlight, 770, 130);
	  currentObject = "LEGS";
	  break;
	case 0:
	  draw_sprite(screen, maphighlight, 770, 70);
	  currentObject = "BOOTS";
	  break;
	case 1:
	  draw_sprite(screen, maphighlight, 770, 10);
	  currentObject = "SHIELD";
	  break;
      }
      break;
  }
}

//Paints the notificationsBox and messages to the screen. Jury rigged type of textbox without scrolling.
void drawNotifications(string m1) {
  draw_sprite(screen, notificationsBox, 0, 480);
  messageNew = m1;
  message4 = message3;
  message3 = message2;
  message2 = message1;
  message1 = messageNew;
  textout_centre_ex(screen, Calibri20, charName, 320, 480, makecol(1,1,1), -1);
  textout_ex(screen, Calibri16, message4.c_str(), 20, 520, makecol(0,0,0), -1);
  textout_ex(screen, Calibri16, message3.c_str(), 20, 550, makecol(0,0,0), -1);
  textout_ex(screen, Calibri16, message2.c_str(), 20, 580, makecol(0,0,0), -1);
  textout_ex(screen, Calibri16, message1.c_str(), 20, 610, makecol(0,0,0), -1);
}

//Paints the player's health, strength, and defense to the screen
void drawStats() {
  draw_sprite(screen, statsBox, 640, 200);
  stringstream tempSS;
  string tempS;
  tempSS << health;
  tempS = tempSS.str();
  textout_right_ex(screen, Impact20, tempS.c_str(), 835, 197, makecol(0,0,0), -1);
  tempSS.str("");
  tempSS << strength;
  tempS = tempSS.str();
  textout_right_ex(screen, Impact20, tempS.c_str(), 835, 230, makecol(0,0,0), -1);
  tempSS.str("");
  tempSS << defense;
  tempS = tempSS.str();
  textout_right_ex(screen, Impact20, tempS.c_str(), 835, 264, makecol(0,0,0), -1);
  tempSS.str("");
  int tempXP = level * level * 10 - XP;
  tempSS << tempXP;
  tempS = tempSS.str();
  textout_right_ex(screen, Impact20, tempS.c_str(), 835, 298, makecol(0,0,0), -1);
  tempSS.str("");
  tempSS << level;
  tempS = tempSS.str();
  textout_right_ex(screen, Impact20, tempS.c_str(), 835, 330, makecol(0,0,0), -1);
}

//Draws the player's objects by looping through the playersObjects vector, addding its value to strength or
//defense depending on the object's "type" (WEAPON or ARMOR).
void drawObjects() {
  strength = 0;
  defense = 0;
  for (int i = 0; i < playersObjects.size(); i++) {
    draw_sprite(itemsBox, playersObjects[i].objectPicture, playersObjects[i].get_x(),
        playersObjects[i].get_y());
    if (playersObjects[i].objectType == "WEAPON")
      strength += playersObjects[i].objectValue;
    else
      defense += playersObjects[i].objectValue;
  }
  draw_sprite(screen, itemsBox, 640, 370);
  drawStats();
}

//Redraws the map to the buffer. This is done as one piece which is loaded with every map coordinate
//change to cut down on the logic that needs to be executed.
void drawMap() {
  draw_sprite(buffer, fullMap, 0, 0);
  needUpdate = true;
}

//iIitializes a small amount of random enemies, pushing them to the myEnemies vector.
void loadEnemies() {
  Enemy newEnemies[rand() % 3 + 1];
  myEnemies.clear();
  for (int i = 0; i < (sizeof newEnemies/sizeof(Enemy)); i++)
    myEnemies.push_back(newEnemies[i]);
}

//Draws each Enemy object from the myEnemies vector onto the buffer.
void drawEnemies() {
  for (int i = 0; i < myEnemies.size(); i++) {
    masked_blit(enemyPic, buffer, myEnemies[i].get_sx(), myEnemies[i].get_sy(), myEnemies[i].get_x(), myEnemies[i].get_y(), 34, 62);
  }
}

//Loads the map data from the corresponding text file in the Maps folder according to the player's mapx
//and mapy locations
void loadMap() {
  string fileName = "Maps/[";
  stringstream temp;
  temp << mapx;
  fileName = fileName + temp.str();
  temp.str("");
  temp << mapy;
  fileName = fileName + "," + temp.str() + "].txt";
  ifstream inFile(fileName.c_str());
  string tempS;

  //Reads the map text file's data into the maptiles 2D array, which will be used to draw the tiles.
  while(!inFile.eof()) {
    for (row = 0; row < 15; row++) {
      for (column = 0; column < 20; column++) {
	getline(inFile, tempS);
	tileNum = atoi(tempS.c_str());
        maptiles[row][column] = tileNum;
      }
    }
  }
  inFile.close();
  
  //Draws the correct map to a bitmap by looping through the maptiles array. Each element is a number which
  //corresponds to a single 32x32 tile on the tile sheet "Sprites/tile.bmp". It simply multiplies the element
  //by 32 to find the starting x value of which tile to draw, then uses 0 for the strating y and draws a 32x32
  //square.
  fullMap = create_bitmap(640, 480);
  for (row = 0; row < 15; row++) {
    for (column = 0; column < 20; column++) {
      masked_blit(map, fullMap, maptiles[row][column]*32, 0,
        column*32, row*32, 32, 32);
    }
  }
  
  //Each time a new map is drawn, enemies are reloaded and the map and minimap are drawn.
  loadEnemies();
  drawMap();
  drawMiniMap();
}

//Draws the character with the correct sprite and enemies to the buffer on top of the map
void drawChar() {
  drawMap();
  drawEnemies();
  masked_blit(character, buffer, sx, sy, x, y, 34, 62);
  needUpdate = true;
}

//Paints the buffer to the screen and waits
void Draw() {
  acquire_screen();
  draw_sprite(screen, buffer, 0, 0);
  release_screen();

  needUpdate = false;
  
  rest(50);
}

//Checks if player is standing on item square (depending on map coordinates) and adds it to the playersObjects
//vector for painting the itemsBox and adding up character's strength and defense, as long as the player does
//not already have the item. It then redraws the player's items and pushes a notification.
void checkObjects() {
  hasObject = false;
  if (currentObject == "SWORD") { // at square [3,4] in map [0,0]
    if (x >= (32 * 1) && x <= (32 * 3) && y >= (32 * 1) && y <= (32 * 4)) {
      swordPic = load_bitmap("Sprites/sword.bmp", NULL);
      Object sword("Sword", "WEAPON", 50, swordPic);
      sword.set_location(27, 116);
      if (playersObjects.size() > 0) {
        for (int i = 0; i < playersObjects.size(); i++)
          if (playersObjects[i].objectName.compare(sword.objectName) == 0) hasObject = true;
      } else
	hasObject = false;
      if (hasObject == false) {
	playersObjects.push_back(sword);
        drawObjects();
        stringstream temp;
        temp << "You found an awesome sword!";
        messageNew = temp.str();
        drawNotifications(messageNew);
      }
    }
  } else if (currentObject == "CHEST") { // at square [17, 12] in map [0,-1]
    if (x >= (32 * 15) && x <= (32 * 17) && y >= (32 * 9) && y <= (32 * 12)) {
      chestPic = load_bitmap("Sprites/chest.bmp", NULL);
      Object chest("Chest", "ARMOR", 25, chestPic);
      chest.set_location(70, 107);
      if (playersObjects.size() > 0) {
        for (int i = 0; i < playersObjects.size(); i++)
          if (playersObjects[i].objectName.compare(chest.objectName) == 0) hasObject = true;
      } else
	hasObject = false;
      if (hasObject == false) {
	playersObjects.push_back(chest);
        drawObjects();
        stringstream temp;
        temp << "You found some strong chest armor!";
        messageNew = temp.str();
        drawNotifications(messageNew);
      }
    }
  } else if (currentObject == "HELMET") { // at square [13, 12] in map [0,1]
    if (x >= (32 * 11) && x <= (32 * 13) && y >= (32 * 9) && y <= (32 * 12)) {
      helmetPic = load_bitmap("Sprites/helmet.bmp", NULL);
      Object helmet("Helmet", "ARMOR", 10, helmetPic);
      helmet.set_location(78, 52);
      if (playersObjects.size() > 0) {
        for (int i = 0; i < playersObjects.size(); i++)
          if (playersObjects[i].objectName.compare(helmet.objectName) == 0) hasObject = true;
      } else
	hasObject = false;
      if (hasObject == false) {
	playersObjects.push_back(helmet);
        drawObjects();
        stringstream temp;
        temp << "You found a sturdy helmet!";
        messageNew = temp.str();
        drawNotifications(messageNew);
      }
    }
  } else if (currentObject == "BOOTS") { // at square [10, 6] in map [1,0]
    if (x >= (32 * 8) && x <= (32 * 10) && y >= (32 * 3) && y <= (32 * 6)) {
      bootsPic = load_bitmap("Sprites/boots.bmp", NULL);
      Object boots("Boots", "ARMOR", 10, bootsPic);
      boots.set_location(75, 217);
      if (playersObjects.size() > 0) {
        for (int i = 0; i < playersObjects.size(); i++)
          if (playersObjects[i].objectName.compare(boots.objectName) == 0) hasObject = true;
      } else
	hasObject = false;
      if (hasObject == false) {
	playersObjects.push_back(boots);
        drawObjects();
        stringstream temp;
        temp << "You found some snazzy looking boots!";
        messageNew = temp.str();
        drawNotifications(messageNew);
      }
    }
  } else if (currentObject == "LEGS") { // at square [3, 3] in map [1,-1]
    if (x >= (32 * 1) && x <= (32 * 3) && y >= (32 * 0) && y <= (32 * 3)) {
      legsPic = load_bitmap("Sprites/legs.bmp", NULL);
      Object legs("Legs", "ARMOR", 10, legsPic);
      legs.set_location(80, 161);
      if (playersObjects.size() > 0) {
        for (int i = 0; i < playersObjects.size(); i++)
          if (playersObjects[i].objectName.compare(legs.objectName) == 0) hasObject = true;
      } else
	hasObject = false;
      if (hasObject == false) {
	playersObjects.push_back(legs);
        drawObjects();
        stringstream temp;
        temp << "You found some great leg armor!";
        messageNew = temp.str();
        drawNotifications(messageNew);
      }
    }
  } else if (currentObject == "SHIELD") { // at square [18, 8] in map [1,1]
    if (x >= (32 * 16) && x <= (32 * 18) && y >= (32 * 5) && y <= (32 * 8)) {
      shieldPic = load_bitmap("Sprites/shield.bmp", NULL);
      Object shield("Shield", "ARMOR", 10, shieldPic);
      shield.set_location(134, 120);
      if (playersObjects.size() > 0) {
        for (int i = 0; i < playersObjects.size(); i++)
          if (playersObjects[i].objectName.compare(shield.objectName) == 0) hasObject = true;
      } else
	hasObject = false;
      if (hasObject == false) {
	playersObjects.push_back(shield);
        drawObjects();
        stringstream temp;
        temp << "You found a big shield!";
        messageNew = temp.str();
        drawNotifications(messageNew);
      }
    }
  } else if (currentObject == "BOW") { // at square [8, 6] in map [-1,0]
    if (x >= (32 * 6) && x <= (32 * 8) && y >= (32 * 3) && y <= (32 * 6)) {
      bowPic = load_bitmap("Sprites/bow.bmp", NULL);
      Object bow("Bow", "WEAPON", 25, bowPic);
      bow.set_location(16, 53);
      if (playersObjects.size() > 0) {
        for (int i = 0; i < playersObjects.size(); i++)
          if (playersObjects[i].objectName.compare(bow.objectName) == 0) hasObject = true;
      } else
	hasObject = false;
      if (hasObject == false) {
	playersObjects.push_back(bow);
        drawObjects();
        stringstream temp;
        temp << "You found a bow with plenty of arrows!";
        messageNew = temp.str();
        drawNotifications(messageNew);
      }
    }
  }
}

//Makes sure that the player will not walk into the bounds of an enemy character. Returns false if the player
//is attempting to walk into an enemy, which stops the player from moving.
bool checkEnemyCollision() {
  for (int i = 0; i < myEnemies.size(); i++) {
    if (x + cx < myEnemies[i].get_x() + 34 && x + cx + 34 > myEnemies[i].get_x()
    && y + cy < myEnemies[i].get_y() + 62 && y + cy + 62 > myEnemies[i].get_y()) {
      enemyNum = i;
      return true;
    }
  }
  return false;
}

//Adds 10 health to the player. This is called every 10 seconds.
void healPlayer() {
  if (health < 90)
    health += 10;
  else
    health = 100;
  drawStats();
}

//Drops all of the player's items, putting their health back to 100 and moving them to the starting coords.
void die() {
  playersObjects.clear();
  x = 320, y = 240, mapx = 0, mapy = 0, health = 100;
  loadMap();
  drawObjects();
  drawNotifications("You died! You lost all of your items. Be more careful next time.");
  itemsBox = load_bitmap("Sprites/itemsBox.bmp", NULL);
  draw_sprite(screen, itemsBox, 640, 370);
}

//Increases the players level. Called when the player has enough XP to level up.
void levelUp() {
  level++;
  stringstream temp;
  temp << "Congratulations, you leveled up! You are now level " << level << ".";
  messageNew = temp.str();
  drawNotifications(messageNew);
}

//Adds XP to the player, checking if the player has enough to level up. XP is added when an enemy is killed.
void addXP(int num) {
  XP += num;
  if (XP >= level * level * 10) {
    levelUp();
    addXP(0);
}
  drawStats();
}

//Removes an enemy from the myEnemies vector, redrawing all other enemies and giving the player XP.
void killEnemy(int num) {
  myEnemies.erase(myEnemies.begin() + num);
  drawEnemies();
  needUpdate = true;
  drawNotifications("You killed an enemy, earning you 50 XP!");
  addXP(50);
}

//Prevents continuous attacking.
void limitAttack() {
  isAttacking = false;
}

//Uses cx and cy to check how far a player would move in a certain direction. Uses this data to see if an
//enemy is at that coord. If there is, then it deals damage based on the player's strength. This method
//also animates the player as if they were attacking by changing the sx value, which is the starting x of
//the sprite to draw. This method also animates the enemy by calling the attackBack method in Enemy.cpp
void checkAttack() {
  if (sy == 62 || sy == 186) { //Player is facing left/up
    cx = -10;
    cy = -10;
  } else { //Player is facing right/down
    cx = 10;
    cy = 10;
  }
  //Animates the player to attack
  if (sx != 102)
    sx = 102;
  else
    sx = 136;
  drawChar();
  if (checkEnemyCollision()) {
    myEnemies[enemyNum].health -= strength * .1 + (rand() % 10);
    if (myEnemies[enemyNum].health <= 0)
      killEnemy(enemyNum);
    //Damages the player while animating the enemy to attack by calling the attackBack method in Enemy.cpp,
    //which returns the amount of damage dealt to the enemy and orients the enemy toward the player.
    health -= myEnemies[enemyNum].attackBack(sy) - defense * .1;
    //If the player has no health, kill them.
    if (health <= 0)
      die();
    drawStats();
  }
}

//Handles character movement, checking for map edges to load new maps and checks for enemy collision.
//This method also animates the character if they can move, by adding the csx value (change in starting x
//value for the source sprite) to sx (the starting x for the source sprite), which goes through the walk
//cycle in the sprite sheet.
void checkMove() {
  if (x + cx < 0 && mapx - 1 != -2) {
    mapx--;
    loadMap();
    x = buffer->w - 34;
  } else if (x + cx < 0 && mapx - 1 == -2) {
  } else if (x + cx > buffer->w - 34 && mapx + 1 != 2) {
    mapx++;
    loadMap();
    x = 0;
  } else if (x + cx > buffer->w - 34 && mapx + 1 == 2) {
  } else if (checkEnemyCollision()) {
  } else
    x += cx;

  if (y + cy < 0 && mapy + 1 != 2) {
    mapy++;
    loadMap();
    y = buffer->h - 62;
  } else if (y + cy < 0 && mapy + 1 == 2) {
  } else if (y + cy > buffer->h - 62 && mapy - 1 != -2) {
    mapy--;
    loadMap();
    y = 0;
  } else if (y + cy > buffer->h - 62 && mapy - 1 == -2) {
  } else if (checkEnemyCollision()) {
  } else
    y += cy;
  if (sx == 68)
    csx = -34;
  else if (sx == 0)
    csx = 34;
  else if (sx > 68) {
    csx = 34;
    sx = 0;
  }
  sx += csx;
  checkObjects();
  drawChar();
}

//Creates a new character, saving basic info to a new file with the name of the character.
void newChar() {
  string charFile = "Characters/";
  charFile = charFile + charName + ".txt";
  ofstream outFile(charFile.c_str());

  outFile << x << endl << y << endl << mapx << endl << mapy << endl << health << endl << strength << endl << defense << endl << XP << endl << level;
  outFile.close();
  stringstream temp;
  temp << "Welcome to Jon's game! Your new character";
  messageNew = temp.str();
  drawNotifications(messageNew);
  temp.str("");
  temp <<  "\"" << charName << "\" has been created.";
  messageNew = temp.str();
  drawNotifications(messageNew);
  temp.str("");
  temp << "When you exit the game, your character will be saved.";
  messageNew = temp.str();
  drawNotifications(messageNew);
}

//Attempts to load all character info from file. If it doesn't exist, creates a new character.
void loadChar() {
  string charFile = "Characters/";
  charFile = charFile + charName + ".txt";
  ifstream inFile(charFile.c_str());

  if (inFile) {
    //Loads the char info (location, health, combat values)
    inFile >> x >> y >> mapx >> mapy >> health >> strength >> defense >> XP >> level;

    //Loads the player's objects
    string oN;
    for (int i = 0; !(inFile.eof()); i++) {
      inFile >> oN;
      if (oN == "Sword") {
        swordPic = load_bitmap("Sprites/sword.bmp", NULL);
        Object sword("Sword", "WEAPON", 50, swordPic);
        sword.set_location(27, 116);
        playersObjects.push_back(sword);
      } else if (oN == "Helmet") {
        helmetPic = load_bitmap("Sprites/helmet.bmp", NULL);
        Object helmet("Helmet", "ARMOR", 10, helmetPic);
        helmet.set_location(78, 52);
        playersObjects.push_back(helmet);
      } else if (oN == "Chest") {
        chestPic = load_bitmap("Sprites/chest.bmp", NULL);
        Object chest("Chest", "ARMOR", 25, chestPic);
        chest.set_location(70, 107);
        playersObjects.push_back(chest);
      } else if (oN == "Legs") {
        legsPic = load_bitmap("Sprites/legs.bmp", NULL);
        Object legs("Legs", "ARMOR", 10, legsPic);
        legs.set_location(80, 161);
        playersObjects.push_back(legs);
      } else if (oN == "Boots") {
        bootsPic = load_bitmap("Sprites/boots.bmp", NULL);
        Object boots("Boots", "ARMOR", 10, bootsPic);
        boots.set_location(75, 217);
        playersObjects.push_back(boots);
      } else if (oN == "Shield") {
        shieldPic = load_bitmap("Sprites/shield.bmp", NULL);
        Object shield("Shield", "ARMOR", 10, shieldPic);
        shield.set_location(134, 120);
        playersObjects.push_back(shield);
      } else if (oN == "Bow") {
        bowPic = load_bitmap("Sprites/bow.bmp", NULL);
        Object bow("Bow", "WEAPON", 25, bowPic);
        bow.set_location(16, 53);
        playersObjects.push_back(bow);
      }
    } 
    inFile.close();
    stringstream temp;
    temp << "Welcome to Jon's game! Your character";
    messageNew = temp.str();
    drawNotifications(messageNew);
    temp.str("");
    temp << "\"" << charName << "\" has been loaded.";
    messageNew = temp.str();
    drawNotifications(messageNew);
    temp.str("");
    temp << "When you exit the game, your character will be saved.";
    messageNew = temp.str();
    drawNotifications(messageNew);
  } else
    newChar();
}

//Saves character info to it's corresponding file in the "Characters" folder
void saveChar() {
  string charFile = "Characters/";
  charFile = charFile + charName + ".txt";
  ofstream outFile(charFile.c_str());

  outFile << x << endl << y << endl << mapx << endl << mapy << endl << health << endl << strength << endl << defense << endl << XP << endl << level;
  for (int i = 0; i < playersObjects.size(); i++)
    outFile << endl << playersObjects[i].objectName;
  outFile.close();
}

//Dialog box to choose login or create character (allegro)
DIALOG chooseLoginDialog[] = {
  /* (proc) (x) (y) (w) (h) (fg) (bg) (key) (flags) (d1) (d2) (dp) (dp2) (dp3) */
  {d_shadow_box_proc, 120, 152, 460, 250, 0, 16777215, 0, 0, 0, 0, NULL, NULL, NULL},
  {d_text_proc, 215, 180, 224, 48, 0, 16777215, 0, 0, 0, 0, (void*)"Welcome to Jon's game!", NULL, NULL},
  {d_text_proc, 145, 230, 224, 48, 0, 16777215, 0, 0, 0, 0, (void*)"If you already have a character, click", NULL, NULL},
  {d_text_proc, 140, 270, 168, 48, 0, 16777215, 0, 0, 0, 0, (void*)"\"Login\". To create one, click \"Create\".", NULL, NULL},
  {d_button_proc, 220, 330, 100, 35, 0, 16777215, 'p', D_EXIT, 0, 0, (void*)"Login", NULL, NULL},
  {d_button_proc, 380, 330, 100, 35, 0, 16777215, 'c', D_EXIT, 0, 0, (void*)"Create", NULL, NULL},
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL}
};

//Dialog box for create character (allegro)
DIALOG createCharDialog[] = {
  /* (proc) (x) (y) (w) (h) (fg) (bg) (key) (flags) (d1) (d2) (dp) (dp2) (dp3) */
  {d_shadow_box_proc, 120, 152, 460, 250, 0, 16777215, 0, 0, 0, 0, NULL, NULL, NULL},
  {d_text_proc, 180, 190, 224, 48, 0, 16777215, 0, 0, 0, 0, (void*)"Please choose your character's", NULL, NULL},
  {d_text_proc, 192, 220, 224, 48, 0, 16777215, 0, 0, 0, 0, (void*)"name in the textbox below.", NULL, NULL},
  {d_edit_proc, 270, 270, 175, 16, 0, 16777215, 0, D_GOTFOCUS, 50, 0, (void*)charName, NULL, NULL},
  {d_button_proc, 220, 330, 100, 35, 0, 16777215, 'p', D_EXIT, 0, 0, (void*)"Play", NULL, NULL},
  {d_button_proc, 380, 330, 100, 35, 0, 16777215, 'c', D_EXIT, 0, 0, (void*)"Cancel", NULL, NULL},
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL}
};

//Dialog box for login (allegro)
DIALOG loginCharDialog[] = {
  /* (proc) (x) (y) (w) (h) (fg) (bg) (key) (flags) (d1) (d2) (dp) (dp2) (dp3) */
  {d_shadow_box_proc, 120, 152, 460, 250, 0, 16777215, 0, 0, 0, 0, NULL, NULL, NULL},
  {d_text_proc, 185, 190, 224, 48, 0, 16777215, 0, 0, 0, 0, (void*)"Please enter your character's", NULL, NULL},
  {d_text_proc, 192, 220, 224, 48, 0, 16777215, 0, 0, 0, 0, (void*)"name in the textbox below.", NULL, NULL},
  {d_edit_proc, 270, 270, 175, 16, 0, 16777215, 0, D_GOTFOCUS, 50, 0, (void*)charName, NULL, NULL},
  {d_button_proc, 220, 330, 100, 35, 0, 16777215, 'p', D_EXIT, 0, 0, (void*)"Play", NULL, NULL},
  {d_button_proc, 380, 330, 100, 35, 0, 16777215, 'c', D_EXIT, 0, 0, (void*)"Cancel", NULL, NULL},
  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL}
};

//Loads dialog boxes and grabs user input for login
void login() {
  centre_dialog(chooseLoginDialog);
  centre_dialog(createCharDialog);
  centre_dialog(loginCharDialog);
  int ret = do_dialog(chooseLoginDialog, 4);
  if (ret == 4) {
    int ret2 = do_dialog(loginCharDialog, 3); //Login
    if (ret2 == 4) //Play
      loadChar();
    else if (ret2 == 5) //Cancel (goes back to login alert)
      login();
  } else {
    int ret2 = do_dialog(createCharDialog, 3); //Create char
    if (ret2 == 4) //Play
      newChar();
    else if (ret2 == 5) //Cancel (goes back to login alert)
      login();
  }
}

int main() {

  //Main window initialization
  allegro_init();
  set_color_depth(16);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 840, 650, 0, 0); //640, 480
  install_keyboard();
  install_mouse();

  //Fonts
  Impact20 = load_font("Sprites/Impact20.pcx", NULL, NULL);
  Calibri20 = load_font("Sprites/Calibri20.pcx", NULL, NULL);
  Calibri16 = load_font("Sprites/Calibri16.pcx", NULL, NULL);
  font = load_font("Sprites/Calibri20.pcx", NULL, NULL);
  
  //Sprites
  buffer = create_bitmap(640, 480);
  character = load_bitmap("Sprites/character.bmp", NULL);
  map = load_bitmap("Sprites/tile.bmp", NULL);
  minimap = load_bitmap("Sprites/minimap.bmp", NULL);
  maphighlight = load_bitmap("Sprites/maphighlight.bmp", NULL);
  itemsBox = load_bitmap("Sprites/itemsBox.bmp", NULL);
  statsBox = load_bitmap("Sprites/statsBox.bmp", NULL);
  notificationsBox = load_bitmap("Sprites/notificationsBox.bmp", NULL);
  enemyPic = load_bitmap("Sprites/enemyPic.bmp", NULL);

  //Prompts for login or create, grabbing username then painting GUI
  login();
  loadMap();
  drawObjects();
  drawChar();

  //Installing timers that handle character animation, attacking, and healing
  install_timer();
  LOCK_FUNCTION(checkMove);
  LOCK_FUNCTION(limitAttack);
  install_int_ex(healPlayer, SECS_TO_TIMER(10));

  //Main game loop. Runs until the user presses escape. Handles character walking, game logic and GUI
  //repainting. When a walking button is pressed, the player changes their direction and will move
  //the value of cx or cy if they can. The walking method checkMove is called 30 times a second. The
  //player can attack 2 times a second.
  while (! key [KEY_ESC]) {
    remove_int(checkMove);
    if (key [KEY_W]) {
      sy = 186;
      cx = 0;
      cy = -4;
      install_int_ex(checkMove, BPS_TO_TIMER(30));
    } else if (key [KEY_A]) {
      sy = 62;
      cx = -4;
      cy = 0;
      install_int_ex(checkMove, BPS_TO_TIMER(30));
    } else if (key [KEY_S]) {
      sy = 0;
      cx = 0;
      cy = 4;
      install_int_ex(checkMove, BPS_TO_TIMER(30));
    } else if (key [KEY_D]) {
      sy = 124;
      cx = 4;
      cy = 0;
      install_int_ex(checkMove, BPS_TO_TIMER(30));
    } else if (key [KEY_SPACE] && !(isAttacking)) {
      isAttacking = true;
      install_int_ex(limitAttack, MSEC_TO_TIMER(500));
      checkAttack();
      needUpdate = true;
    } else {
    }

    if(needUpdate)
      drawChar();
      Draw();
  }

  //When user exits game, their character is saved.
  saveChar();

  //Memory deallocation
  destroy_bitmap(character);
  destroy_bitmap(map);
  destroy_bitmap(fullMap);
  destroy_bitmap(minimap);
  destroy_bitmap(maphighlight);
  destroy_bitmap(itemsBox);
  destroy_bitmap(notificationsBox);
  destroy_bitmap(buffer);
  return 0;
}
END_OF_MAIN()
