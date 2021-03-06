// main.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include "Alien.h"
#include "Collision.h"
#include "Drawing.h"
#include "Drop.h"
#include "ParticleEffect.h"
#include "globals.h"
#include "olcConsoleGameEngine.h"
#include "stdlibs.h"

struct Example : public olcConsoleGameEngine {
  // This function is called at the beginning of each level
  bool OnUserCreate() {
    srand(time(NULL));
    Globals::TotalTime = 0.0;
    playerHealth = Globals::kLivesPerLevel; // reset player health
    playerPos[0] = ScreenWidth() / 2;
    playerPos[1] = ScreenHeight() - (Globals::kPlayerHeight * 2.5);
    // clear object vectors
    enemy.clear();
    bullet.clear();
    enemy_bullet.clear();
    exploding_bullet.clear();
    exploding_enemy.clear();
    rain.clear();
    bullet.reserve(30); // we can reserve for vectors but not for lists
    enemy_bullet.reserve(30);
    exploding_bullet.reserve(20);
    exploding_enemy.reserve(20);
    rain.reserve(180);
    // create aliens
    switch (Globals::Level) {
    case 0:
      Globals::CUTSCENE = true;
      for (int i = 1; i < 7; i++) {
        int shuffle_dir = 1;
        enemy.emplace_back(Alien(i * Globals::kSpacer, Globals::kSpacer * 2,
                                 shuffle_dir * 20, 8, 20, 12,
                                 Behavior::side_to_side));
      }
      for (int i = 1; i < 7; i++) {
        int shuffle_dir = -1;
        enemy.emplace_back(Alien(i * Globals::kSpacer, Globals::kSpacer * 5,
                                 shuffle_dir * 20, 8, 20, 12,
                                 Behavior::side_to_side));
      }
      for (int i = 1; i < 3; i++) {
        int shuffle_dir = -1;
        enemy.emplace_back(Alien(i * Globals::kSpacer + Globals::kSpacer * 3,
                                 Globals::kSpacer * 1, shuffle_dir * 40, 8, 20,
                                 12, Behavior::side_to_side));
      }
      for (int i = 1; i < 3; i++) {
        int shuffle_dir = +1;
        enemy.emplace_back(Alien(i * Globals::kSpacer + Globals::kSpacer * 1,
                                 Globals::kSpacer * 1, shuffle_dir * 40, 8, 20,
                                 12, Behavior::side_to_side));
      }
      break;
    case 1:
      Globals::CUTSCENE = true;
      for (int i = 2; i <= 5; i++) {
        int rot_direction = (i % 2 == 0 ? -1 : 1);
        int radius = 30;
        enemy.emplace_back(Alien(30 + i * 40, 48, radius, rot_direction, 20, 12,
                                 Behavior::circles));
      }
      for (int i = 1; i < 7; i++) {
        int shuffle_dir = (i % 3 == 0 ? 1 : -1);
        enemy.emplace_back(Alien(i * Globals::kSpacer, 70, shuffle_dir * 20, 8,
                                 20, 12, Behavior::side_to_side));
      }
      enemy.emplace_back(Alien(Globals::kScreenWidth / 3.0,
                               2.0 * Globals::kScreenHeight / 4.0, -999, -999,
                               15, 15, Behavior::sniper));
      enemy.emplace_back(Alien(Globals::kScreenWidth / 3.0,
                               3.0 * Globals::kScreenHeight / 5.0, -999, -999,
                               15, 15, Behavior::sniper));
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
        enemy.emplace_back(Alien(30 + i * 40, 48, radius, rot_direction, 15, 8,
                                 Behavior::circles));
      }
      for (int i = 1; i < 7; i++) {
        int shuffle_dir = 1;
        enemy.emplace_back(Alien(i * Globals::kSpacer, 70, shuffle_dir * 20, 8,
                                 12, 8, Behavior::side_to_side));
      }
      break;
    default:
      break;
    };
    // Create Background
    for (int i = 0; i < Globals::kRainDropCount; i++) {
      rain.push_back(Drop(Globals::Level));
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
    if (m_keys[VK_ESCAPE].bPressed) {
      Globals::PAUSE = !Globals::PAUSE;
    } // toggle pause
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

    Globals::TotalTime += fElapsedTime; // Increment total game running time

    if (Globals::CUTSCENE) {
      Globals::CutSceneTimer += fElapsedTime;
      if (Globals::CutSceneTimer >= Globals::kCutSceneLength) {
        Globals::CutSceneTimer = 0;
        m_nFontHeight = 2; // reset font
        m_nFontWidth = 2;
        Globals::CUTSCENE = false; // reset flag
        return true;
      }
      Drawing::ClearScreen(*this);
      // Draw Level Start Screen
      m_nFontHeight = 16;
      m_nFontWidth = 16;
      string strLevel = "Get Ready! Level: " + to_string(Globals::Level);
      string strPause = "(Pause game with Escape Key)";
      string strArrows = "(Move with arrow keys <-- -->)";
      string strShoot = "   (Shoot with spacebar)   ";
      wstring strLevel2(strLevel.begin(), strLevel.end());
      wstring strPause2(strPause.begin(), strPause.end());
      wstring strArrows2(strArrows.begin(), strArrows.end());
      wstring strShoot2(strShoot.begin(), strShoot.end());
      int xoffset = 10;
      DrawString(xoffset, 10, strLevel2, 90);
      DrawString(xoffset, 13, strPause2, 90);
      DrawString(xoffset, 15, strArrows2, 90);
      DrawString(xoffset, 17, strShoot2, 90);
      return true;
    }

    //######################################################
    //############## Begin User Input Handling ###############
    //######################################################
    if (m_keys[VK_SPACE].bPressed) {
      // Create Bullets
      bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + 2,
                          playerPos[1] - 1 + 2, 0, Globals::kBulletSpeed);
      bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + -2,
                          playerPos[1] - 1 + 2, 0, Globals::kBulletSpeed);
    }

    // Update Player Position
    if (m_keys[VK_RIGHT].bPressed || m_keys[VK_RIGHT].bHeld)
      playerPos[0] += (Globals::kPlayerSpeed * fElapsedTime);
    else if (m_keys[VK_LEFT].bPressed || m_keys[VK_LEFT].bHeld)
      playerPos[0] -= Globals::kPlayerSpeed * fElapsedTime;
    //######################################################
    //############## End User Input Handling ###############
    //######################################################

    for (auto &e : enemy) {
      // Update Enemy Positions
      e.UpdatePosition(fElapsedTime);
      float playerCenterX = playerPos[0] + Globals::kPlayerWidth / 2.0;
      float playerCenterY = playerPos[1];
      float enemyCenterX = e.Pos[0] + e.width / 2.0;
      float enemyCenterY = e.Pos[1] + e.height / 2.0;
      float distance = Globals::Distance(playerCenterX, playerCenterY,
                                         enemyCenterX, enemyCenterY);

      // Fire Non-Sniper shots
      if (e.attitude != Behavior::sniper &&
          Alien::IsGoodToShoot(e, playerPos, fElapsedTime)) {
        enemy_bullet.emplace_back(
            enemyCenterX + 1 * Globals::kBulletWidth, e.Pos[1] + e.height + 1,
            ((playerCenterX + 2 * Globals::kBulletWidth) / distance -
             enemyCenterX / distance) *
                110,
            +110);
        enemy_bullet.emplace_back(
            enemyCenterX - 1 * Globals::kBulletWidth, e.Pos[1] + e.height + 1,
            ((playerCenterX - 2 * Globals::kBulletWidth) / distance -
             enemyCenterX / distance) *
                110,
            +110);
      }
      // Fire Sniper Shots
      if (e.attitude == Behavior::sniper &&
          Alien::IsSniperGoodToShoot(e, playerPos, fElapsedTime)) {
        enemy_bullet.emplace_back(
            enemyCenterX, e.Pos[1] + e.height + 1,
            (playerCenterX / distance - enemyCenterX / distance) * 140,
            (playerCenterY / distance - enemyCenterY / distance) * 140);
      }
      // Check for Enemy Collisions with Bullet
      for (auto &b : bullet) {
        if (Collision::AlienGotHit(e, b)) {
          b.Alive = false;
          e.Health--;
          e.Cracked = true;
          exploding_bullet.emplace_back(b, e);
          if (e.Health <= 0) {
            exploding_enemy.emplace_back(b, e);
          }
        }
      }
    }

    //######################################################
    //################### Begin Drawing ####################
    //######################################################
    // Clear Screen
    Drawing::ClearScreen(*this);

    // Update rain
    // Draw Rain
    for (auto &d : rain) {
      d.Fall(fElapsedTime);
      Drawing::DrawRain(*this, d);
    }

    // Draw Player
    Drawing::DrawPlayer(*this, playerPos);

    // Partition and Erase Dead Player bullets
    auto _last_alive_it =
        partition(bullet.begin(), bullet.end(),
                  [](const Bullet &b) { return b.Alive == true; });
    bullet.erase(_last_alive_it, bullet.end());
    // Draw Player Bullets
    for (auto &b : bullet) {
      // Update Player Bullet Positions
      b.Pos[1] += b.Vel[1] * fElapsedTime;
      // kill player bullet if it goes off screen
      if (b.Pos[1] < 0) {
        b.Alive = false;
      }
      Drawing::DrawBullet(*this, b.Pos[0], b.Pos[1], Globals::kBulletWidth,
                          Globals::kBulletHeight);
    }

    // Partition and Erase Dead Enemy Bullets
    auto last_alive_it =
        partition(enemy_bullet.begin(), enemy_bullet.end(),
                  [](const Bullet &b) { return b.Alive == true; });
    enemy_bullet.erase(last_alive_it, enemy_bullet.end());
    // Draw Enemy Bullets
    for (auto i = 0; i < enemy_bullet.size(); i++) {
      // Update Enemy Bullet Positions
      enemy_bullet[i].Pos[1] += enemy_bullet[i].Vel[1] * fElapsedTime;
      enemy_bullet[i].Pos[0] += enemy_bullet[i].Vel[0] * fElapsedTime;
      if (enemy_bullet[i].Pos[1] > Globals::kScreenHeight) {
        enemy_bullet[i].Alive = false;
      }

      // Check for Player Collisions with Bullet
      if (enemy_bullet[i].Alive &&
          Collision::PlayerGotHit(playerPos, enemy_bullet[i])) {
        enemy_bullet[i].Alive = false;
        playerHealth--;
        exploding_bullet.emplace_back(enemy_bullet[i], playerPos);
      }
      Drawing::DrawBullet(*this, enemy_bullet[i].Pos[0], enemy_bullet[i].Pos[1],
                          Globals::kBulletWidth, Globals::kBulletHeight);
    }

    // Erase Dead Enemies
    for (auto it = begin(enemy); it != end(enemy); it++) {
      if (it->Health <= 0) {
        enemy.erase(it);
      }
    }
    // Draw Enemies
    for (auto &e : enemy) {
      Drawing::DrawEnemy(*this, e);
    }

    // Draw Player Healthbar
    Drawing::DrawHealthBar(*this, playerHealth);

    // Erase Dead Enemy Explosions
    auto _last_alive_e =
        partition(exploding_enemy.begin(), exploding_enemy.end(),
                  [](const ParticleEffect3 &pe) { return pe.Alive == true; });
    exploding_enemy.erase(_last_alive_e, exploding_enemy.end());
    // Draw Exploding Enemies
    for (auto &ex : exploding_enemy) {
      // Update exploding Enemies
      ex.Update(fElapsedTime);
      Drawing::DrawExplodingEnemy(*this, ex);
    }

    // Erase Dead Bullet Explosions
    auto _last_alive =
        partition(exploding_bullet.begin(), exploding_bullet.end(),
                  [](const ParticleEffect2 &pe) { return pe.Alive == true; });
    exploding_bullet.erase(_last_alive, exploding_bullet.end());
    // Draw Exploding Bullets
    for (auto &ex : exploding_bullet) {
      // Update exploding Bullets
      ex.Update(fElapsedTime);
      Drawing::DrawExplodingBullet(*this, ex);
    }

    //######################################################
    //################### End Drawing ####################
    //######################################################

    // Restart Level if Player Dead
    if (playerHealth <= 0) {
      OnUserCreate();
      return true;
    }

    // Progress to Next level
    if (enemy.size() == 0) {
      Globals::Level++;
      OnUserCreate();
    }

    return true;
  }
  // Game globals
  vector<Bullet> bullet;
  vector<Bullet> enemy_bullet;
  float playerPos[2];
  int playerHealth; // player's current health
  list<Alien> enemy;
  vector<ParticleEffect2> exploding_bullet;
  vector<ParticleEffect3> exploding_enemy;
  vector<Drop> rain;
};

int main() {
  //
  // Create game window
  //
  Example game;
  game.ConstructConsole(Globals::kScreenWidth, Globals::kScreenHeight,
                        Globals::kPixelSize, Globals::kPixelSize);
  game.Start();
  return 0;
}
