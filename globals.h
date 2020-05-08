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
		// Bullet
		int kBulletWidth = 2;
		int kBulletHeight = 5;
		float kBulletSpeed = -160;
		// Game state
		int Level = 0;										// what level are we on
		bool PAUSE = false;								// is game paused
		bool CUTSCENE = false;						// is game in a cutscene
		float CutSceneTimer = 0.0;
		constexpr float kCutSceneLength = 1.5;	// seconds pause
		float TotalTime = 0.0; 
		// Rain
		const int kRainDropCount = 200;
		//
		// Global Member Functions
		//
  }  // namespace Globals
