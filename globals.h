// globals.h - Contains the Globals namespace used to hold globally 
//             used variables and free functions

#pragma once
#include "stdlibs.h"
#include "Bullet.h"
namespace Globals {
  // Player
  const int kPlayerWidth = 10;
  const int kPlayerHeight = 6;
  const float kPlayerSpeed = 165;
  constexpr float kScreenWidth = 500;
  constexpr float kScreenHeight = kScreenWidth * 9 / 16;
  constexpr int kSpacer = kScreenWidth / 15;
  const int kLivesPerLevel = 3;
  // Bullet
  constexpr int kBulletSpeed = -160;
  constexpr int kBulletWidth = 2;
  constexpr int kBulletHeight = 5;
  const float kEnemyReloadingTime = 0.7;
  // HealthBar
  const int kHealthBarRadius = 5;
  // Game state
  int Level = 0;							// what level are we on
  bool PAUSE = false;	    				// is game paused
  bool CUTSCENE = false;					// is game in a cutscene
  float CutSceneTimer = 0.0;
  constexpr float kCutSceneLength = 2.8;	// seconds pause
  float TotalTime = 0.0; 
  const int kPixelSize = 2;
  // Rain
  const int kRainDropCount = 165;

  // Member functions
 float Distance(float x, float y, float x2, float y2) {
    return sqrt(pow(x - x2,  2) + pow(y - y2, 2));
  }
} // namespace Globals
