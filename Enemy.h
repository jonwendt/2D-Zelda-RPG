#ifndef ENEMY_H
#define ENEMY_H

#include <allegro.h>

class Enemy {
public:
  Enemy();
  virtual ~Enemy();

  int health;

  //functions
  int attackBack(int direction);
  int get_x();
  int get_y();
  int get_sx();
  int get_sy();

protected:

private:
  int x, y, sx, sy;
};

#endif // ENEMY_H
