#pragma once

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


namespace Globals {
  // Player 
  const int kPlayerWidth = 4;
  const int kPlayerHeight = 4;
  // Bullet
  const int kBulletWidth = 3;
  const int kBulletHeight = 3;
  const int kBulletSpeed = 1;
  Bullet* last_empty;
} // namespace Globals
