#pragma once
#include "globals.h"
#include "Bullet.h"

enum class Behavior { circles, side_to_side, sniper };
enum class Color { BLUE = 145, GREEN = 165 , WHITE = 245, YELLOW = 235};

struct Alien {
  Alien(float x, float y, float vx, float vy, int width, int height,
      Behavior b = Behavior::side_to_side) {
    PosX0 = x;
    PosY0 = y;
    Pos[0] = x;
    Pos[1] = y;
    Vel[0] = vx;
    Vel[1] = vy;
    this->width = width;
    this->height = height;
    Health = 2;
    attitude = b;
    Cracked = false;
    TimeSinceFired = 0;
    GoalPos[0] = 3.0 * Globals::kScreenWidth / 4.0;
    GoalPos[1] = 3.0 * Globals::kScreenHeight / 4.0;
    switch(attitude) {
      case Behavior::side_to_side:
        color = Color::GREEN;
        break;
      case Behavior::circles:
        color = Color::BLUE;
        break;
      case Behavior::sniper:
        color = Color::WHITE;
        break;
    };
  }

  // member variables
  float PosX0, PosY0;
  float Pos[2];
  float Vel[2];
  Color color;
  int height, width;
  int Health;
  bool Cracked;
  Behavior attitude;
  float GoalPos[2]; // for sniper behavior
  mutable float TimeSinceFired;

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
      case Behavior::sniper:
        if (Globals::Distance(Pos[0], Pos[1], GoalPos[0],GoalPos[1]) < 7) { 
          // create new goal pos
          float oldPosX = GoalPos[0];
          float oldPosY = GoalPos[1];
          do {
            GoalPos[0] = round(Globals::kScreenWidth / 5.0) + rand() % ((int)round(3.0 * Globals::kScreenWidth / 5.0));
            GoalPos[1] = round(Globals::kScreenHeight / 5.0) + rand() % ((int)round(3.0 * Globals::kScreenHeight / 5.0));
          } while(Globals::Distance(GoalPos[0], GoalPos[1], oldPosX, oldPosY) < Globals::kScreenHeight/ 8.0);
          assert(GoalPos[0] > 0 && GoalPos[0] < Globals::kScreenWidth && GoalPos[1] > 0 && GoalPos[1] <
              Globals::kScreenHeight);
        }
        float DirX = (GoalPos[0] - Pos[0]) / fabs(GoalPos[0] - Pos[0]);
        float DirY = (GoalPos[1] - Pos[1]) / fabs(GoalPos[1] - Pos[1]);
        Pos[0] += DirX * 16.0 * fElapsed;
        Pos[1] += DirY * 16.0 * fElapsed;
        break;
    };
  }
  float getCenterX() const { return Pos[0] + width / 2.0; }
  float getCenterY() const { return Pos[1] + height / 2.0; }
  static bool GotHit(const Alien &, const Bullet &);
  static bool IsGoodToShoot(const Alien&, const float*, const float&);
  static bool IsSniperGoodToShoot(const Alien& a, const float* playerPos, const float& fElapsed);
};

// Static method for alien-bullet collisions

bool Alien::IsGoodToShoot(const Alien& a, const float* playerPos, const float& fElapsed)
{
  // TODO: raycast to player
  if (a.TimeSinceFired > Globals::kEnemyReloadingTime && fabs(*playerPos + (Globals::kPlayerWidth / 2) -
        (a.Pos[0] + a.width / 2)) < 3.0) {
    a.TimeSinceFired = 0.0; // reset reloading timer
    return true;
  } else {
    a.TimeSinceFired += fElapsed;
    return false;
  }
}

bool Alien::IsSniperGoodToShoot(const Alien& a, const float* playerPos, const float& fElapsed)
{
  // TODO: raycast to player
  if (a.TimeSinceFired > Globals::kEnemyReloadingTime * 2.0) {
    a.TimeSinceFired = 0.0; // reset reloading timer
    return true;
  } else {
    a.TimeSinceFired += fElapsed;
    return false;
  }
}
