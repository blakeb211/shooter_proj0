#pragma once
#include "stdlibs.h"
#include "Bullet.h"
namespace Globals {
  // Player
  const int kPlayerWidth = 8;
  const int kPlayerHeight = 5;
  const float kPlayerSpeed = 165;
  constexpr float kScreenWidth = 360;
  constexpr float kScreenHeight = kScreenWidth * 9 / 16;
  const int kLivesPerLevel = 3;
  // Bullet
  constexpr int kBulletWidth = 2;
  constexpr int kBulletHeight = 5;
  float kBulletSpeed = -160;
  float kEnemyBulletSpeed = +90;
  const float kEnemyReloadingTime = 0.9;
  // HealthBar
  const int kHealthBarRadius = 3;
  // Game state
  int Level = 0;							// what level are we on
  bool PAUSE = false;	    				// is game paused
  bool CUTSCENE = false;					// is game in a cutscene
  float CutSceneTimer = 0.0;
  constexpr float kCutSceneLength = 1.75;	// seconds pause
  float TotalTime = 0.0; 
  const int kPixelSize = 2;
  // Rain
  const int kRainDropCount = 140;

  // Member functions
  bool PlayerGotHit(const float * playerPos, const Bullet& b) {
    int rightP = *playerPos + Globals::kPlayerWidth;
    int leftP = *playerPos;
    int bottomP = *(playerPos + 1) + Globals::kPlayerHeight;
    int topP = *(playerPos + 1);
    int topBullet = b.Pos[1];
    int bottomBullet = b.Pos[1] + Globals::kBulletHeight;
    int leftBullet = b.Pos[0];
    int rightBullet = b.Pos[0] + Globals::kBulletWidth;
    if (rightBullet > leftP && leftBullet < rightP &&
        bottomBullet > topP && bottomBullet < bottomP) {
      return true;
    } else {
      return false;
    }
  } 
} // namespace Globals
