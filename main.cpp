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
  // This function is called at the beginning of each level
  bool OnUserCreate() {
    Globals::TotalTime = 0.0;
    playerHealth = Globals::kLivesPerLevel; // reset player health
    playerPos[0] = ScreenWidth() / 2;
    playerPos[1] = ScreenHeight() - (Globals::kPlayerHeight*1.5);
    // clear object vectors
    enemy.clear();
    bullet.clear();
    bullet.reserve(30);          // we can reserve for vectors but not for lists
    enemy_bullet.clear();
    enemy_bullet.reserve(30);
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
    // Create Background 
    for (int i = 0; i < Globals::kRainDropCount; i++) {
      rain.push_back(Drop());
    }
    return true;
  }

  //
  // Begin Main Game Loop 
  //
  bool OnUserUpdate(float fElapsedTime) {
    //######################################################
    //############## Begin Pause Handling ###############
    //######################################################
    if (m_keys[VK_ESCAPE].bPressed) { Globals::PAUSE = !Globals::PAUSE; } // toggle pause
    if (Globals::PAUSE) {
      // Draw Color palette in pause screen, 50 colors per line, from 1 to 250
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
    //######################################################
    //############## End Pause Handling ###############
    //######################################################

    Globals::TotalTime += fElapsedTime;    // Increment total game running time

    if (Globals::CUTSCENE) {
      Globals::CutSceneTimer += fElapsedTime;
      if (Globals::CutSceneTimer >= Globals::kCutSceneLength) {
        Globals::CutSceneTimer = 0; 
        m_nFontHeight = 2;          // reset font
        m_nFontWidth = 2;
        Globals::CUTSCENE = false;  // reset flag
        return true;
      }
      Drawing::ClearScreen(*this);
      // Draw Level Start Screen 
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

    
    //######################################################
    //############## Begin User Input Handling ###############
    //######################################################
    if (m_keys[VK_SPACE].bPressed) {
        // Create Bullets 
        bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2,
            playerPos[1] - 1, 0, 2);
      bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + 2,
          playerPos[1] - 1 + 2, 0, 2);
      bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + -2,
          playerPos[1] - 1 + 2, 0, 2);
    }

    // Update Player Position
    if (m_keys[VK_RIGHT].bPressed || m_keys[VK_RIGHT].bHeld) 
      playerPos[0] += (Globals::kPlayerSpeed * fElapsedTime);
    else if (m_keys[VK_LEFT].bPressed || m_keys[VK_LEFT].bHeld) 
      playerPos[0] -= Globals::kPlayerSpeed * fElapsedTime;
    //######################################################
    //############## End User Input Handling ###############
    //######################################################

    // Update Player Bullet Positions
    for (auto & b : bullet) {
      b.Pos[1] += round(Globals::kBulletSpeed * fElapsedTime);
      bool _currAliveState = b.Alive;
      b.Alive = b.Pos[1] < 0 ? false : _currAliveState;
    }

    // Update Alive Enemy Bullet Positions
    for (auto i = 0; i < enemy_bullet.size(); i++) {
      enemy_bullet[i].Pos[1] += round(Globals::kEnemyBulletSpeed * fElapsedTime);
      bool _currAliveState = enemy_bullet[i].Alive;
      enemy_bullet[i].Alive = enemy_bullet[i].Pos[1] > Globals::kScreenHeight ? false : _currAliveState;
    }
      
    // Move dead enemy bullets to the end and remove them 
      auto last_alive_it =
        partition(enemy_bullet.begin(), enemy_bullet.end(),
            [](const Bullet& b) { return b.Alive == true; });
      if (enemy_bullet.end() - last_alive_it > 0)
        enemy_bullet.erase(last_alive_it, enemy_bullet.end());

    // Partition and count dead bullets
    auto _last_alive_it = partition(
        bullet.begin(), bullet.end(), [](const Bullet& b) { return b.Alive == true; });
        bullet.erase(_last_alive_it, bullet.end());

    // Update Enemy Position
    for (auto& e : enemy) {
      e.UpdatePosition(fElapsedTime);
      if (Alien::IsGoodToShoot(e, playerPos, fElapsedTime)) {
        enemy_bullet.emplace_back(e.Pos[0] + (e.width) / 2.0,
            e.Pos[1] + e.height + 2, 0,
            0);
      }
    }

    // Update Explosion Timers
    for (auto& ex : explosions) {
      ex.UpdateTimer(fElapsedTime);
    }
    // Update Rain position
    for (auto& d : rain) {
      d.Fall(fElapsedTime);
    }

    // Check Enemy Collisions with Player Bullets
    for (auto enemy_it = begin(enemy); enemy_it != end(enemy); enemy_it++) {
            for (auto & b : bullet) {
        if (b.Alive && enemy_it->Health > 0 && Alien::GotHit(*enemy_it, b)) {
          enemy_it->Health--;
          enemy_it->Cracked = true;
          b.Alive = false;
          // explode dead enemy 
          if (enemy_it->Health <= 0) {
            explosions.emplace_back(enemy_it->Pos[0] + enemy_it->width / 2,
                enemy_it->Pos[1] + enemy_it->height / 2.0, 0.4, -999);
            // remove dead enemy
            enemy_it = enemy.erase(enemy_it);
          }
        }
      }
    }
    // Check collision of Player with Enemy Bullets
    for(auto & b : enemy_bullet) {
      if(Globals::PlayerGotHit(playerPos,b)) {
        b.Alive = false;
        // insert bullet explosion
        playerHealth--;
      }
    }
    // Restart Level if Player Dead
    if (playerHealth <= 0) { OnUserCreate(); }
    
    // Progress to Next level
    if (enemy.size() == 0) {
      Globals::Level++;
      OnUserCreate();
    }

    //######################################################
    //################### Begin Drawing ####################
    //######################################################
    // Clear Screen
    Drawing::ClearScreen(*this);

    // Draw Rain
    for (auto& d : rain)
      Drawing::DrawRain(*this, d);

    // Draw Player
    Drawing::DrawPlayer(*this,playerPos);

    // Draw Bullets
    for (auto & b : bullet) {
      Drawing::DrawBullet(*this, b.Pos[0], b.Pos[1],
          Globals::kBulletWidth, Globals::kBulletHeight);
    }

    // Draw Enemy Bullets
    for (auto i = 0; i < enemy_bullet.size(); i++) {
      Drawing::DrawBullet(*this, enemy_bullet[i].Pos[0], enemy_bullet[i].Pos[1],
          Globals::kBulletWidth, Globals::kBulletHeight);
    }

    // Draw Enemies
    for (auto& e : enemy) {
      Drawing::DrawEnemy(*this, e);
    }
    // Draw Player Healthbar
    Drawing::DrawHealthBar(*this, playerHealth);
    // Draw Explosions
    for (auto& ex : explosions) {
      if (ex.Alive) {
        // draw circle of triangles at ex's current radius
        float radius = ex.GetRadius();
        Drawing::DrawExplosion(*this, ex.xPos0, ex.yPos0, radius);
      }
    }
    //######################################################
    //################### End Drawing ####################
    //######################################################
    return true;
  }

  vector<Bullet> bullet;
  vector<Bullet> enemy_bullet;
  float playerPos[2];
  int playerHealth;   // player's current health
  list<Alien> enemy;  
  vector<ParticleEffect> explosions;
  vector<Drop> rain;
};

int main() {
  //
  // Create game window
  //
  Example game;
  game.ConstructConsole(Globals::kScreenWidth, Globals::kScreenHeight, Globals::kPixelSize, Globals::kPixelSize);
  game.Start();
  return 0;
}
