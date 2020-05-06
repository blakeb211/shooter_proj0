#pragma once
#include "stdlibs.h"


// Forward declare Bullet so Globals can use it
struct Bullet;

namespace Globals {
	// Player
	const int kPlayerWidth = 8;
	const int kPlayerHeight = 5;
	const float kPlayerSpeed = 165;
	constexpr float kScreenWidth = 360;
	constexpr float kScreenHeight = kScreenWidth * 9 / 16;
	const double M_PI = 3.1415926;
	// Bullet
	int kBulletWidth = 2;
	int kBulletHeight = 5;
	float kBulletSpeed = -160;
	Bullet* reusable_bullet; 
	// Game state
	int Level = 0;    // what level are we on
	bool PAUSE = false; // is game paused
	bool CUTSCENE = false; // is game in a cutscene
	float CutSceneTimer = 0.0;
	const float kCutSceneLength = 2; // seconds pause
	float TotalTime = 0.0; 
  // rain generator

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
  Alien(float x, float y, float vx, float vy, int width, int height,
        Behavior b = Behavior::side_to_side) {
    Pos[0] = x;
    PosX0 = x;
    PosY0 = y;
    Pos[1] = y;
    Vel[0] = vx;
    Vel[1] = vy;
    this->width = width;
    this->height = height;
    Alive = true;
    Health = 2;
    attitude = b;
  }

  // member variables
  float PosX0, PosY0;
  float Pos[2];
  float Vel[2];
  int height, width;
  bool Alive;
  int Health;
  Behavior attitude;
  // member methods
  void UpdatePosition(float fElapsed) {
    switch (attitude) {
    case Behavior::side_to_side:
      Pos[0] += Vel[0] * fElapsed;
      Pos[1] += Vel[1] * fElapsed;
      if (Pos[0] < 20 || Pos[0] > Globals::kScreenWidth - 20)
        Vel[0] *= -1.0;
      if (Pos[1] < 10 || Pos[1] > Globals::kScreenHeight - 35)
        Vel[1] *= -1.0;
      break;
    case Behavior::circles:
      Pos[0] =
          PosX0 + Vel[1] * Vel[0] *
                      cos(1.1 * Globals::TotalTime); // Vel[0] is the radius
      Pos[1] = PosY0 + Vel[1] * Vel[0] *
                           sin(1.1 * Globals::TotalTime); // Vel[1] is the sign
      if (Pos[0] < 0)
        Pos[0] += Globals::kScreenWidth;
      if (Pos[0] > Globals::kScreenWidth)
        Pos[0] -= Globals::kScreenWidth;
      if (Pos[1] < 0)
        Pos[1] += Globals::kScreenHeight;
      if (Pos[1] > Globals::kScreenHeight)
        Pos[1] -= Globals::kScreenHeight;
      break;
    };
  }
  static bool GotHit(const Alien &, const Bullet &);
};

// Static method for alien-bullet collisions
bool Alien::GotHit(const Alien &a, const Bullet &b) {
  int rightAlien = a.Pos[0] + a.width; // each enemy has own size
  int leftAlien = a.Pos[0];
  int bottomAlien = a.Pos[1] + a.height;
  int topAlien = a.Pos[1];
  int topBullet = b.Pos[1];
  int bottomBullet = b.Pos[1] + Globals::kBulletHeight; // only one size bullet
  int leftBullet = b.Pos[0];
  int rightBullet = b.Pos[0] + Globals::kBulletWidth;
  if (rightBullet > leftAlien && leftBullet < rightAlien &&
      topBullet > topAlien && topBullet < bottomAlien) {
    return true;
  } else {
    return false;
  }
}

struct ParticleEffect {
 public:
  ParticleEffect() = default;
  ParticleEffect(int x, int y, float duration, float param2) {
    _timer = 0;
    Alive = true;
    xPos0 = x;
    yPos0 = y;
    _maxTimer = duration;  // 2 seconds long
  }
  bool Alive;
  int xPos0;
  int yPos0;

  void UpdateTimer(float fElapsed) {
    _timer += fElapsed;
    if (_timer >= _maxTimer)
      Alive = false;
  }
  float GetRadius() { return (0.5609 + (30 * _timer) - (2 * _timer * _timer)); }

 private:
  float _maxTimer;
  float _timer;
};

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
