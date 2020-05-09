// main.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include "globals.h"
#include "Alien.h"
#include "ParticleEffect.h"
#include "Drop.h"
#include "olcConsoleGameEngine.h"
#include "stdlibs.h"
#include "Drawing.h"

struct Example : public olcConsoleGameEngine {
  bool OnUserCreate() {
    // This function is called at the beginning of each level
    // load level, characters, sprites, etc
    Globals::TotalTime = 0.0;
    playerPos[0] = ScreenWidth() / 2;
    playerPos[1] = ScreenHeight() - (Globals::kPlayerHeight + 1);
    // clear object vectors
    enemy.clear();
    bullet.clear();
    bullet.reserve(30); // we can reserve for vectors but not for lists
    explosions.clear();
    explosions.reserve(30);
    rain.clear();
    rain.reserve(200);

    // create aliens
    int widthSpacer0 = (ScreenWidth() / 15);
    switch (Globals::Level) {
      case 0:
        Globals::CUTSCENE = true;
        for (int i = 2; i <= 5; i++) {
          int rot_direction = (i % 2 == 0 ? -1 : 1);
          int radius = 30;
          enemy.emplace_back(Alien(30 + i * 40, 48, radius, rot_direction, 20,
                                   12, Behavior::circles));
        }
        for (int i = 1; i < 7; i++) {
          int shuffle_dir = (i % 3 == 0 ? 1 : -1);
          enemy.emplace_back(Alien(i * widthSpacer0, 70, shuffle_dir * 20, 8,
                                   20, 12, Behavior::side_to_side));
        }
        break;
      case 1:
        Globals::CUTSCENE = true;
        for (int i = 1; i < 7; i++) {
          int shuffle_dir = 1;
          enemy.emplace_back(Alien(i * widthSpacer0, 10, shuffle_dir * 20, 8,
                                   20, 12, Behavior::side_to_side));
        }
        break;

      case 2:
        Globals::CUTSCENE = true;
        // create enemies
        for (int i = 1; i < 5; i++) {
          enemy.emplace_back(
              Alien(100, 25 * i, 0, 0, 20, 14, Behavior::side_to_side));
        }
        break;
      case 3:
        Globals::CUTSCENE = true;
        // create enemies
        for (int i = 2; i <= 5; i++) {
          int rot_direction = (i % 2 == 0 ? -1 : 1);
          int radius = 30;
          enemy.emplace_back(Alien(30 + i * 40, 48, radius, rot_direction, 15,
                                   8, Behavior::circles));
        }
        for (int i = 1; i < 7; i++) {
          int shuffle_dir = 1;
          enemy.emplace_back(Alien(i * widthSpacer0, 70, shuffle_dir * 20, 8,
                                   12, 8, Behavior::side_to_side));
        }
        break;
      default:
        break;
    };
    //
    // Create Raindrops
    //
    for (int i = 0; i < Globals::kRainDropCount; i++) {
      rain.push_back(Drop());
    }
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) {
    //
    // Toggle Pause and break;
    //
    if (m_keys[VK_ESCAPE].bPressed) {
      Globals::PAUSE = !Globals::PAUSE;
      // cout << "Pause Pressed" << endl;
    }
    if (Globals::PAUSE) {
      // Draw Colors
      int xbump = 0;
      int ybump = 0;
      for (int i = 1; i < 250; i++) {
        Fill(50 + xbump, ybump, 50 + xbump + 4, ybump + 2, L'-', i);
        xbump += 5;
        if (i % 50 == 0) {
          string strColorIndex = to_string(i);
          wstring wStrColor(strColorIndex.begin(), strColorIndex.end());
          DrawString(50 + xbump + 5, ybump, wStrColor, 140);
          xbump = 0;
          ybump += 7;
        }
      }
      return true;
    }

    // Increment total game running time
    Globals::TotalTime += fElapsedTime;


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

      Drawing::ClearScreen(*this);
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
    
    // Partition and count dead bullets
    auto _beg_alive_it = partition(
        bullet.begin(), bullet.end(), [](const Bullet& b) { return b.Alive == false; });
    int dead_bullet_count = _beg_alive_it - bullet.begin();
    // Check for User Input
    //
    if (m_keys[VK_SPACE].bPressed) {
      // if there are 3 re-usable bullet slots, reincarnate those bullets at the end of the 
      // "dead bullets" region of the bullet vector. 

      // DDDDAAA before reincarnation
      // DAAAAAA after reincarnation
      //  ^^^    re-used bullets
      if (dead_bullet_count >= 3) {
        bullet[dead_bullet_count - 1].Pos[0] = playerPos[0] + Globals::kPlayerWidth / 2;
        bullet[dead_bullet_count - 1].Pos[1] =
            playerPos[1] - Globals::kBulletHeight;

        bullet[dead_bullet_count - 2].Pos[0] =
            playerPos[0] + Globals::kPlayerWidth / 2 + 2;
        bullet[dead_bullet_count - 2].Pos[1] = playerPos[1] - 1 + 2;
        
        bullet[dead_bullet_count - 3].Pos[0] =
            playerPos[0] + Globals::kPlayerWidth / 2 + -2;
        bullet[dead_bullet_count - 3].Pos[1] = playerPos[1] - 1 + 2;

        bullet[dead_bullet_count - 1].Alive = true;
        bullet[dead_bullet_count - 2].Alive = true;
        bullet[dead_bullet_count - 3].Alive = true;
        dead_bullet_count -= 3;
      } else
        // create new bullet
        bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2,
                            playerPos[1] - 1, 0, 2);
        bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + 2,
                          playerPos[1] - 1 + 2, 0, 2);
        bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + -2,
                          playerPos[1] - 1 + 2, 0, 2);
    }
    //
    // Update Player Position
    //
    if (m_keys[VK_RIGHT].bPressed || m_keys[VK_RIGHT].bHeld) 
        playerPos[0] += (Globals::kPlayerSpeed * fElapsedTime);
      else if (m_keys[VK_LEFT].bPressed || m_keys[VK_LEFT].bHeld) 
        playerPos[0] -= Globals::kPlayerSpeed * fElapsedTime;
    
    //
    // Update Alive Bullet Positions
    //
    // All bullets in the range [dead_bullet_count, bullet.end()] are alive 
    // and need to be updated
    for (auto i = dead_bullet_count; i < bullet.size(); i++) {
      bullet[i].Pos[1] += round(Globals::kBulletSpeed * fElapsedTime);
      bullet[i].Alive = bullet[i].Pos[1] < 0 ? false : true;
    }

    //
    // Update Enemy Position
    //
    for (auto& e : enemy) {
        e.UpdatePosition(fElapsedTime);
    }
    //
    // Update Explosion Timers
    //
    for (auto& ex : explosions) {
      ex.UpdateTimer(fElapsedTime);
    }
    //
    // Update Rain position
    //
    for (auto& d : rain) {
      d.Fall(fElapsedTime);
    }
    //
    // Check Enemy-Bullet Collisions. This step invalidates the bullet iterator
    //
    for (auto enemy_it = begin(enemy); enemy_it != end(enemy); enemy_it++) {
        for (auto i = dead_bullet_count; i < bullet.size(); i++) {
          if (Alien::GotHit(*enemy_it, bullet[i])) {
            enemy_it->Health--;
            enemy_it->Cracked = true;
            bullet[i].Alive = false;
            // create explosion effect
            if (enemy_it->Health <= 0) {
              explosions.emplace_back(enemy_it->Pos[0] + enemy_it->width / 2,
                                      enemy_it->Pos[1] + enemy_it->height / 2.0, 0.4, -999);
              // remove dead enemy
              enemy_it = enemy.erase(enemy_it);
            }
          }
        }
    }
    // Progress to Next level
    if (enemy.size() == 0) {
      Globals::Level++;
      OnUserCreate();
    }
    /************************************************************************************
                                      Drawing Start
    ************************************************************************************/

    // Clear Screen
    Drawing::ClearScreen(*this);
    
    // Draw Rain
    for (auto& d : rain)
      Drawing::DrawRain(*this, d);
 
    // Draw Player
    Drawing::DrawPlayer(*this,playerPos);

    // Draw Bullets
    for (auto i = dead_bullet_count; i < bullet.size(); i++)
      Drawing::DrawBullet(*this, bullet[i].Pos[0], bullet[i].Pos[1],
                          Globals::kBulletWidth, Globals::kBulletHeight);

    // Draw Enemies
    for (auto& e : enemy) {
        Drawing::DrawEnemy(*this, e);
    }
    // Draw Explosions
    for (auto& ex : explosions) {
      if (ex.Alive) {
			// draw circle of triangles at ex's current radius
        float radius = ex.GetRadius();
        float rand_0_3 = rand() % 3;
        radius += rand_0_3;
        Drawing::DrawExplosion(*this, ex.xPos0, ex.yPos0, radius);
      }
    }
    /************************************************************************************
                                  Drawing End
    ************************************************************************************/

    return true;
  }

  vector<Bullet> bullet;
  float playerPos[2];
  list<Alien> enemy;  // use a list for fast insertion and deletion and so we can decrease branching in the main loop
  vector<ParticleEffect> explosions;
  vector<Drop> rain;
};

int main() {
  //
  // Create game window
  //
  Example game;
  game.ConstructConsole(Globals::kScreenWidth, Globals::kScreenHeight, 2, 2);
  game.Start();
  return 0;
}
