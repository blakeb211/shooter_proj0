// shooter_proj0.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include "globals.h"
#include "olcConsoleGameEngine.h"
#include "stdlibs.h"

struct Example : public olcConsoleGameEngine {
  bool OnUserCreate() {
    // load level, characters, sprites, etc
    Globals::last_empty = 0;
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
      if (Globals::last_empty) {
        Globals::last_empty->Pos[1] = playerPos[1] - 1;
        Globals::last_empty->Pos[0] = playerPos[0];
        Globals::last_empty = 0;
      } else
        bullets.emplace_back(playerPos[0], playerPos[1] - 1, 0, 2);
      // cout << setw(20) << "SPACE" << endl;
    }
    if (m_keys[VK_RIGHT].bPressed || m_keys[VK_RIGHT].bHeld) {
      playerPos[0]++;
      // cout << setw(20) << "RIGHT" << endl;
    }
    if (m_keys[VK_LEFT].bPressed || m_keys[VK_LEFT].bHeld) {
      playerPos[0]--;
      // cout << setw(20) << "LEFT" << endl;
    }

    //
    // Update Bullet Positions
    //
    if (!Globals::last_empty) {
      for (auto& b : bullets) {
        // mark the last used bullet

        if (b.Pos[1] < 0)
          Globals::last_empty = &b;
        else
          b.Pos[1] -= Globals::kBulletSpeed;
      }
    } else {
      for (auto& b : bullets) {
        // move active bullets
        if (b.Pos[1] >= 0)
          b.Pos[1] -= Globals::kBulletSpeed;
      }
    }
 
    // Clear Screen
    Fill(0, 0, ScreenWidth(), ScreenHeight(), L';', 0);

    //
    // Draw Player
    //
    Fill(playerPos[0], playerPos[1], playerPos[0] + Globals::kPlayerWidth,
         playerPos[1] + Globals::kPlayerHeight, L'&', 14);
    //
    // Draw Bullets
    //
    for (auto& b : bullets) {
      Fill(b.Pos[0], b.Pos[1], b.Pos[0] + Globals::kBulletWidth,
           b.Pos[1] + Globals::kBulletHeight, L'O', 60);
    }
    return true;
  }

  vector<Bullet> bullets;
  int playerPos[2];
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
