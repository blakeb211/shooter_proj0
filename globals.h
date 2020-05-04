#pragma once
#include "stdlibs.h"


// Forward declare Bullet so Globals can use it
struct Bullet;

namespace Globals {
// Player
const int kPlayerWidth = 4;
const int kPlayerHeight = 4;
const float kPlayerSpeed = 100;
const float kScreenWidth = 360;
const float kScreenHeight = kScreenWidth * 9 / 16;
// Bullet
int kBulletWidth = 3;
int kBulletHeight = 3;
float kBulletSpeed = -120;
Bullet* reusable_bullet;
// Level - what level are we on?
int Level = 0;
}  // namespace Globals


struct Bullet {
  Bullet() = default;
  Bullet(float x, float y, float vx, float vy) {
    Pos[0] = x;
    Pos[1] = y;
    Vel[0] = vx;
    Vel[1] = vy;
    Alive = true;
  }
  float Pos[2];
  float Vel[2];
  bool Alive;
};

struct Alien {
  Alien(float x, float y, float vx, float vy, int height = 5, int width = 5) {
    Pos[0] = x;
    Pos[1] = y;
    Vel[0] = vx;
    Vel[1] = vy;
    this->width = width;
    this->height = height;
    Alive = true;
    Health = 2;
  }
  // member variables
  float Pos[2];
  float Vel[2];
  int height, width;
  bool Alive;
  int Health;
  
  // member methods
  void UpdatePosition(float fElapsed) {
    Pos[0] += Vel[0] * fElapsed;
    Pos[1] += Vel[1] * fElapsed;
    // wrap around if go off screne
    if (Pos[0] < 0)
      Pos[0] += Globals::kScreenWidth;
    else if (Pos[0] > Globals::kScreenWidth)
      Pos[0] -= Globals::kScreenWidth;
    else if (Pos[1] > Globals::kScreenHeight)
      Pos[1] -= Globals::kScreenHeight;
    else if (Pos[1] < 0) {
      Pos[1] += Globals::kScreenHeight;
    }
  }
  static bool GotHit(Alien&, Bullet&);
};

bool Alien::GotHit(Alien& a, Bullet& b) {
  int rightAlien = a.Pos[0] + a.width;
  int leftAlien = a.Pos[0];
  int bottomAlien = a.Pos[1] + a.height;
  int topAlien = a.Pos[1];
  int topBullet = b.Pos[1];
  int bottomBullet = b.Pos[1] + Globals::kBulletHeight;
  int leftBullet = b.Pos[0];
  int rightBullet = b.Pos[0] + Globals::kBulletWidth;
  if (rightBullet > leftAlien && leftBullet < rightAlien && topBullet > topAlien && topBullet < bottomAlien){
    //std::cout << "Collision Happened" << std::endl;
    return true;
  } else {
    return false;
  }
}
