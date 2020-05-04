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
        bullets.emplace_back(playerPos[0], playerPos[1] - 1, 0, 2);
      // cout << setw(20) << "SPACE" << endl;
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
      for (auto& b : bullets) {
        // mark a re-usable bullet slot because we don't have one
        if (b.Pos[1] < 0)
          Globals::reusable_bullet = &b;
        else
          b.Pos[1] -= Globals::kBulletSpeed * fElapsedTime;
      }
    } else {
      for (auto& b : bullets) {
        // we have a re-usable bullet slot, so update all active bullets
          b.Pos[1] -= Globals::kBulletSpeed * fElapsedTime;
      }
    }

    // 
    // Drawing Start
    //
    // Clear Screen
    Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);

    // Draw Player
    Fill(playerPos[0], playerPos[1], playerPos[0] + Globals::kPlayerWidth,
         playerPos[1] + Globals::kPlayerHeight, L'&', 14);
    
    // Draw Bullets
    for (auto& b : bullets) {
      if (b.Pos[1] >= 0 - Globals::kPlayerHeight) 
        Fill(b.Pos[0], b.Pos[1], b.Pos[0] + Globals::kBulletWidth,
             b.Pos[1] + Globals::kBulletHeight, L'O', 60);
    }
    // 
    // Drawing End
    // 
    return true;
  }

  vector<Bullet> bullets;
  float playerPos[2];
};

int main() {
  //
  // Create game window
  //
  Example game;
  const int kWinWidth = 300;
  game.ConstructConsole(kWinWidth, kWinWidth * 9 / 16, 2, 2);
  game.Start();
  return 0;
}
