/* This is the Drop class. A vector of drops is created at the
beginning of each scene and "falls" to create a raining effect. */

#pragma once
#include "globals.h"

struct Drop {
  float x, y;
  static int dropWidth;
  static int dropHeight;
  int dropColor;
  int yspeed;
  Drop() {
    x = rand() % (int)Globals::kScreenWidth;
    y = rand() % (int)Globals::kScreenHeight;
    yspeed = 25 + rand() % 20; 
    dropColor = 152 + (rand() % 5);
  }
  void Fall(float fElapsed) {
    y += yspeed*fElapsed;
    if (y > Globals::kScreenHeight) {
      y = -5;
      x = rand() % (int)Globals::kScreenWidth;
    }
  }
};
int Drop::dropWidth = 1;
int Drop::dropHeight = 6;
