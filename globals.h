#pragma once
#include "stdlibs.h"


// Forward declare Bullet so Globals can use it
struct Bullet;

namespace Globals {
// Player
const int kPlayerWidth = 4;
const int kPlayerHeight = 4;
const float kPlayerSpeed = 115;
const float kScreenWidth = 360;
const float kScreenHeight = kScreenWidth * 9 / 16;
// Bullet
int kBulletWidth = 3;
int kBulletHeight = 3;
float kBulletSpeed = -140;
Bullet* reusable_bullet;
// Game state
int Level = 0;    // what level are we on
bool PAUSE = false; // is game paused
bool CUTSCENE = false; // is game in a cutscene
float CutSceneTimer = 0.0;
const float kCutSceneLength = 5; // 10 seconds pause
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

enum class Behavior { circles, avoid_bottom_and_top, side_to_side, flocker };

struct Alien {
  Alien(float x, float y, float vx, float vy, int height, int width, Behavior b = Behavior::side_to_side) {
    Pos[0] = x;
    Pos[1] = y;
    Vel[0] = vx;
    Vel[1] = vy;
    this->width = width;
    this->height = height;
    Alive = true;
    Health = 2;
    Behavior attitude;
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
  static bool GotHit(const Alien&, const Bullet&);
};

// Static method for alien-bullet collisions
bool Alien::GotHit(const Alien& a, const Bullet& b) {
  int rightAlien = a.Pos[0] + a.width; // each enemy has own size
  int leftAlien = a.Pos[0];
  int bottomAlien = a.Pos[1] + a.height;
  int topAlien = a.Pos[1];
  int topBullet = b.Pos[1];
  int bottomBullet = b.Pos[1] + Globals::kBulletHeight; // only one size bullet
  int leftBullet = b.Pos[0];
  int rightBullet = b.Pos[0] + Globals::kBulletWidth;
  if (rightBullet > leftAlien && leftBullet < rightAlien && topBullet > topAlien && topBullet < bottomAlien){
    return true;
  } else {
    return false;
  }
}
