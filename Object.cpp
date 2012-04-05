#include "Object.h"

Object::Object(string oN, string oT, int oV, BITMAP* oP) {
  objectName = oN;
  objectType = oT;
  objectValue = oV;
  objectPicture = oP;
}

Object::~Object() {}

void Object::set_location(int objX, int objY) {
  x = objX;
  y = objY;
}

int Object::get_x() {
  return x;
}

int Object::get_y() {
  return y;
}
