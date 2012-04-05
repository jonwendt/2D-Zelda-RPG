#include "Enemy.h"

Enemy::Enemy() {
  //initializes the enemy to a random place on the map facing a random direction
  x = rand() % 600;
  y = rand() % 420;
  sx = 34;
  sy = (rand() % 4) * 62;
  health = 100;
}

Enemy::~Enemy() {}

int Enemy::attackBack(int direction) {
  //turns towards player and animates an attack
  if (direction == 0)
    sy = 186;
  else if (direction == 62)
    sy = 124;
  else if (direction == 124)
    sy = 62;
  else
    sy = 0;
  if (sx != 136)
    sx = 136;
  else
    sx = 102;

  //damages player
  int damage = rand() % 10 + 3;
  return damage;
}

int Enemy::get_x() {
  return x;
}

int Enemy::get_y() {
  return y;
}

int Enemy::get_sx() {
  return sx;
}

int Enemy::get_sy() {
  return sy;
}
