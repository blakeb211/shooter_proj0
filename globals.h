#pragma once
#include <vector>

struct Bullet {
  Bullet() = default;
  Bullet(float x, float y, float vx, float vy) {
    Pos[0] = x;
    Pos[1] = y;
    Vel[0] = vx;
    Vel[1] = vy;
  }
  float Pos[2];
  float Vel[2];
};

struct Alien {
  Alien(float x, float y, float vx, float vy, int height = 5, int width = 5) {
    Pos[0] = x;
    Pos[1] = y;
    Vel[0] = vx;
    Vel[1] = vy;
    this->width = width;
    this->height = height;
  }
  float Pos[2];
  float Vel[2];
  int height, width;
  void UpdatePosition(float fElapsed, int screenWidth, int screenHeight) {
    Pos[0] += Vel[0] * fElapsed;
    Pos[1] += Vel[1] * fElapsed;
    // wrap around if go off screne
    if (Pos[0] < 0)
      Pos[0] += screenWidth;
    else if (Pos[0] > screenWidth)
      Pos[0] -= screenWidth;
    else if (Pos[1] > screenHeight)
      Pos[1] -= screenHeight;
    else if (Pos[1] < 0) {
      Pos[1] += screenHeight;
    }
  }
};

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
  } // namespace Globals
