/* Bullet class for the bullets that the player shoots */
#pragma once

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
