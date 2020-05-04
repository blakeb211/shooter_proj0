// shooter_proj0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdlibs.h"
#include "olcConsoleGameEngine.h"


struct Bullet {
  Bullet() = default;
  Bullet(float x, float y, float vx, float vy) { 
    Pos[0] = x;
    Pos[1] = y;
    Vel[0] = vx;
    Vel[1] = vy;
    Width = 2;
    Height = 2;
  }
  float Pos[2];
  float Vel[2];
  int Width, Height;
};

struct Example : public olcConsoleGameEngine {
  bool OnUserCreate() {
    // load level, characters, sprites, etc.
    playerPos[0] = ScreenWidth() / 2;
    playerPos[1] = ScreenHeight() - 5;
    return true; 
  }

  bool OnUserUpdate(float fElapsedTime) { 
    // use fElapsedTime to modulate speed of motion
    //
    // Update Player Position
    //
    if (m_keys[VK_SPACE].bPressed) {
      bullets.emplace_back(playerPos[0], playerPos[1] - 1, 0, 2);
      cout << "new bullet created" << endl;
      //cout << setw(20) << "SPACE" << endl;
    }
    if (m_keys[VK_RIGHT].bPressed || m_keys[VK_RIGHT].bHeld) {
      playerPos[0]++;
      //cout << setw(20) << "RIGHT" << endl;
    }
    if (m_keys[VK_LEFT].bPressed || m_keys[VK_LEFT].bHeld) {
      playerPos[0]--;
      //cout << setw(20) << "LEFT" << endl;
    }
    //
    // Update Bullet Positions
    //
    for (auto& b : bullets) {
      b.Pos[1] -= b.Vel[1];
    }
    // Clear Screen
    Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ',
         BG_WHITE);
    //
    // Draw Player
    //
    Fill(playerPos[0], playerPos[1], playerPos[0]+4,
         playerPos[1] + 4, L'&', 14);
    //
    // Draw Bullets
    //
    for (auto& b : bullets) {
      Fill(b.Pos[0], b.Pos[1], b.Pos[0] + b.Width, b.Pos[1] + b.Height, L'%',
           FG_CYAN);
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
