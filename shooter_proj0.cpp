// shooter_proj0.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include "globals.h"
#include "olcConsoleGameEngine.h"
#include "stdlibs.h"

struct Example : public olcConsoleGameEngine {
  bool OnUserCreate() {
    // load level, characters, sprites, etc
    Globals::reusable_bullet = 0;
    playerPos[0] = ScreenWidth() / 2;
    playerPos[1] = ScreenHeight() - (Globals::kPlayerHeight + 1);

    // create aliens
    switch (Globals::Level) {
      case 0:
        // dynamically allocate new enemies for the current level
        enemy.emplace_back(Alien(10, 10, 10, 50, 8, 8));
        enemy.emplace_back(Alien(ScreenWidth() - 10, 10, -5, 50, 8, 8));
        enemy.emplace_back(Alien(25, 10, 5, 25, 10, 10));
        enemy.emplace_back(Alien(40, 10, 5, 25, 10, 10));
        break;
		case 1:
		// dynamically allocate new enemies for the current level
        for (int i = 0; i < 10; i++)
			enemy.emplace_back(Alien(i*10, 10, 15, 40, 8, 8));

        break;
      default:
        break;
    };
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) {
    // use fElapsedTime to modulate speed of motion

    // Check for User Input
    //
    if (m_keys[VK_SPACE].bPressed) {
      // if there is an reusable bullet, re-use it
      if (Globals::reusable_bullet) {
        Globals::reusable_bullet->Pos[1] = playerPos[1] - Globals::kBulletHeight;
        Globals::reusable_bullet->Pos[0] = playerPos[0] + Globals::kPlayerWidth / 2;
        Globals::reusable_bullet->Alive = true;
        Globals::reusable_bullet = 0;
      } else
        // create new bullet
        bullet.emplace_back(playerPos[0], playerPos[1] - 1, 0, 2);
    }
    //
    // Update Player Position
    //
    if (m_keys[VK_RIGHT].bPressed || m_keys[VK_RIGHT].bHeld) {
      playerPos[0] += (Globals::kPlayerSpeed * fElapsedTime);
    }
    if (m_keys[VK_LEFT].bPressed || m_keys[VK_LEFT].bHeld) {
      playerPos[0] -= Globals::kPlayerSpeed * fElapsedTime;
    }

    //
    // Check Collisions
    //
    for (auto& e : enemy) {
      if (e.Alive)
      for (auto& b : bullet) {
        if (b.Alive)
          if (Alien::GotHit(e, b)) {
            e.Health--;
            b.Alive = false;
          }
      }
      // kill the enemy if health is gone
      if (e.Health <= 0)
        e.Alive = false;
    }

    //
    // Update Bullet Positions
    //
    if (!Globals::reusable_bullet) {
      for (auto& b : bullet) {
        // mark a re-usable bullet slot because we don't have one
        if (b.Pos[1] < 0) {
          b.Alive = false;
          Globals::reusable_bullet = &b;
        }
        else
          b.Pos[1] += Globals::kBulletSpeed * fElapsedTime;
      }
    } else {
      for (auto& b : bullet) {
        // we have a re-usable bullet slot, so update all active bullets
          b.Pos[1] += Globals::kBulletSpeed * fElapsedTime;
      }
    }

    //
    // Update Enemy Position
    //
    for (auto& e : enemy) {
      if (e.Alive)
      e.UpdatePosition(fElapsedTime);
    }
    /************************************************************************************
    // Drawing Start
    /************************************************************************************/
    // Clear Screen
    Fill(0, 0, Globals::kScreenWidth, Globals::kScreenHeight, L' ', 0);

    // Draw Player
    Fill(playerPos[0], playerPos[1], playerPos[0] + Globals::kPlayerWidth,
         playerPos[1] + Globals::kPlayerHeight, L'&', 14);
    
    // Draw Bullets
    for (auto& b : bullet) {
      if (b.Alive) 
        Fill(b.Pos[0], b.Pos[1], b.Pos[0] + Globals::kBulletWidth,
             b.Pos[1] + Globals::kBulletHeight, L'O', 60);
    }

    // Draw Enemies
	int _livingEnemyCount = 0;
    for (auto& e : enemy) {
      if (e.Alive)
	  {
		_livingEnemyCount++;
        Fill(e.Pos[0], e.Pos[1], e.Pos[0] + e.width,
           e.Pos[1] + e.height, L'T', 75);
	  }
	}
	// Progress to Next level
	if (_livingEnemyCount == 0) {
		Globals::Level++;
		enemy.clear();
		bullet.clear();
		OnUserCreate();
		Sleep(3000);
	}
    /************************************************************************************
    // Drawing End
    /************************************************************************************/
    return true;
  }

  vector<Bullet> bullet;
  float playerPos[2];
  vector<Alien> enemy;
};

int main() {
  //
  // Create game window
  //
  Example game;
  const int kWinWidth = Globals::kScreenWidth;
  game.ConstructConsole(Globals::kScreenWidth, Globals::kScreenHeight, 2, 2);
  game.Start();
  return 0;
}
