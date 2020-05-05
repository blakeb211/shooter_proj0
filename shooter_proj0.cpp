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
    int widthSpacer0 = (ScreenWidth() / 10);
    switch (Globals::Level) {
      case 0:
        // dynamically allocate new enemies for the current level
        Globals::CUTSCENE = true;
        for (int i = 2; i <= 8; i++)
          enemy.emplace_back(Alien(i * widthSpacer0, 10, 10, 40, 8, 8,
                                   Behavior::side_to_side));
        break;
      case 1:
        Globals::CUTSCENE = true;
        // dynamically allocate new enemies for the current level
        for (int i = 0; i < 10; i++)
          enemy.emplace_back(Alien(i * 10, 10, i, 5 * i, 8, 8));

        break;
      case 2:
        Globals::CUTSCENE = true;
        break;
      case 3:
        Globals::CUTSCENE = true;
        break;
      default:
        break;
    };
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) {
    //
    // Toggle Pause and break;
    //
    if (m_keys[VK_ESCAPE].bPressed) {
      Globals::PAUSE = !Globals::PAUSE;
      cout << "Pause Pressed" << endl;
    }
    if (Globals::PAUSE) {
      return true;
    }
    if (Globals::CUTSCENE) {
      Globals::CutSceneTimer += fElapsedTime;
      if (Globals::CutSceneTimer >= Globals::kCutSceneLength) {
        // reset timer
        Globals::CutSceneTimer = 0;
        // reset font
        m_nFontHeight = 2;
        m_nFontWidth = 2;
        // reset cutscene flag
        Globals::CUTSCENE = false;
        return true;
      }
      // Clear Screen
      Fill(0, 0, Globals::kScreenWidth, Globals::kScreenHeight, L' ', 0);
      // Draw Message To Player
      m_nFontHeight = 16;
      m_nFontWidth = 16;
      string strLevel2 = "Get Ready! Level: " + to_string(Globals::Level);
      string strPause2 = "(Pause game with Escape Key) ";
      wstring strLevel(strLevel2.begin(), strLevel2.end());
      wstring strPause(strPause2.begin(), strPause2.end());
      DrawString(10, 8, strLevel, 95);
      DrawString(10, 12, strPause, 95);
      return true;
    }
    // Check for User Input
    //
    if (m_keys[VK_SPACE].bPressed) {
      // if there is an reusable bullet, re-use it
      if (Globals::reusable_bullet) {
        Globals::reusable_bullet->Pos[1] =
            playerPos[1] - Globals::kBulletHeight;
        Globals::reusable_bullet->Pos[0] =
            playerPos[0] + Globals::kPlayerWidth / 2;
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
    // Check Enemy-Bullet Collisions
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
        } else
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
