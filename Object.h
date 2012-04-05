#ifndef OBJECT_H
#define OBJECT_H

#include <allegro.h>
#include <iostream>
using namespace std;

class Object {
public:
  Object(string oN, string oT, int oV, BITMAP* oP);
  virtual ~Object();

  BITMAP* objectPicture;
  string objectName;
  string objectType;
  int objectValue;
  void set_location(int objX, int objY);

  //functions
  int get_x();
  int get_y();

protected:

private:
  int x;
  int y;
};

#endif // OBJECT_H
