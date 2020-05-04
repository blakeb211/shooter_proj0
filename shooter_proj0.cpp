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
    // If Empty_Bullet_Slot, re-use that bullet.
    // Otherwise, create a new bullet.
    if (m_keys[VK_SPACE].bPressed) {
      if (Globals::reusable_bullet) {
        Globals::reusable_bullet->Pos[1] = playerPos[1] - Globals::kBulletHeight;
        Globals::reusable_bullet->Pos[0] = playerPos[0] + Globals::kPlayerWidth / 2;
        Globals::reusable_bullet = 0;
      } else
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
    // Update Bullet Positions
    //
    if (!Globals::reusable_bullet) {
      for (auto& b : bullet) {
        // mark a re-usable bullet slot because we don't have one
        if (b.Pos[1] < 0)
          Globals::reusable_bullet = &b;
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
      if (b.Pos[1] >= 0 - Globals::kPlayerHeight) 
        Fill(b.Pos[0], b.Pos[1], b.Pos[0] + Globals::kBulletWidth,
             b.Pos[1] + Globals::kBulletHeight, L'O', 60);
    }

    // Draw Enemies
    for (auto& e : enemy) {
      cout << "e.x " << e.Pos[0] << " e.y " << e.Pos[1] << "e.width" << e.width << "e.height" << e.height <<  endl;
      Fill(e.Pos[0], e.Pos[1], e.Pos[0] + e.width,
           e.Pos[1] + e.height, L'T', 14);
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
