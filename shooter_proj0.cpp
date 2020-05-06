// shooter_proj0.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include "globals.h"
#include "olcConsoleGameEngine.h"
#include "stdlibs.h"

struct Example : public olcConsoleGameEngine {
  bool OnUserCreate() {
    // This function is called at the beginning of each level
    // load level, characters, sprites, etc
    Globals::TotalTime = 0.0;
    Globals::reusable_bullet = 0;
    playerPos[0] = ScreenWidth() / 2;
    playerPos[1] = ScreenHeight() - (Globals::kPlayerHeight + 1);

    // create aliens
    int widthSpacer0 = (ScreenWidth() / 10);
    switch (Globals::Level) {
    case 0:
      Globals::CUTSCENE = true;
      for (int i = 2; i <= 5; i++) {
        int rot_direction = (i % 2 == 0 ? -1 : 1);
        int radius = 30;
        enemy.emplace_back(Alien(30 + i * 40, 48, radius, rot_direction, 15, 8,
                                 Behavior::circles));
      }
	  for (int i = 1; i < 7; i++) {
        int shuffle_dir = (i % 3 == 0 ? 1 : -1);
        enemy.emplace_back(Alien(i * widthSpacer0, 70, shuffle_dir*20, 8, 12,
                                 8, Behavior::side_to_side));
	  }
      break;
    case 1:
      Globals::CUTSCENE = true;
      for (int i = 1; i < 7; i++) {
        int shuffle_dir = (i % 3 == 0 ? 1 : -1);
        enemy.emplace_back(Alien(i * widthSpacer0, 10, shuffle_dir * 20, 8, 12,
                                 8, Behavior::side_to_side));
      }
      break;

    case 2:
      Globals::CUTSCENE = true;
      // create enemies
	  for (int i = 1; i < 5; i++) {
	  enemy.emplace_back(Alien(100, 50*i, 0, 0, 12,
                                 8, Behavior::side_to_side));
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
        int shuffle_dir = (i % 3 == 0 ? 1 : -1);
        enemy.emplace_back(Alien(i * widthSpacer0, 70, shuffle_dir*20, 8, 12,
                                 8, Behavior::side_to_side));
      }
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
      //cout << "Pause Pressed" << endl;
    }
    if (Globals::PAUSE) {
		// Draw Colors
	  int xbump = 0;
	  int ybump = 0;
	  for (int i = 1; i < 250; i++){
		  Fill(50+xbump,ybump,50+xbump+4,ybump+2,L'-',i);
		  xbump+=5;
		  if (i%50 == 0){
			string strColorIndex = to_string(i);
			wstring wStrColor(strColorIndex.begin(), strColorIndex.end());
			DrawString(50+xbump+5, ybump, wStrColor, 140);
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
		bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + 2, playerPos[1] - 1 + 2, 0, 2);
		bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + -2, playerPos[1] - 1 + 2, 0, 2);
      } else
        // create new bullet
        bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2, playerPos[1] - 1, 0, 2);
		bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + 2, playerPos[1] - 1 + 2, 0, 2);
		bullet.emplace_back(playerPos[0] + Globals::kPlayerWidth / 2 + -2, playerPos[1] - 1 + 2, 0, 2);
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
    for (auto &e : enemy) {
      if (e.Alive)
        for (auto &b : bullet) {
          if (b.Alive)
            if (Alien::GotHit(e, b)) {
              e.Health--;
			  // create explosion effect
			  if (e.Health == 0) 
				  explosions.emplace_back(e.Pos[0] + e.width / 2, e.Pos[1] + e.height / 2.0, 0.4, -999);
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
      for (auto &b : bullet) {
        // mark a re-usable bullet slot because we don't have one
        if (b.Pos[1] < 0) {
          b.Alive = false;
          Globals::reusable_bullet = &b;
        } else
          b.Pos[1] += round(Globals::kBulletSpeed * fElapsedTime);
      }
    } else {
      for (auto &b : bullet) {
        // we have a re-usable bullet slot, so update all active bullets
        b.Pos[1] += round(Globals::kBulletSpeed * fElapsedTime);
      }
    }

    //
    // Update Enemy Position
    //
    for (auto &e : enemy) {
      if (e.Alive)
        e.UpdatePosition(fElapsedTime);
    }
    //
    // Update Explosion Timers
    //
    for (auto &ex : explosions) {
      ex.UpdateTimer(fElapsedTime);
    }
    /************************************************************************************
                                      Drawing Start
    ************************************************************************************/
    // Clear Screen
    Fill(0, 0, Globals::kScreenWidth, Globals::kScreenHeight, L' ', 0);

    // Draw Player
    Fill(round(playerPos[0]), round(playerPos[1]), round(playerPos[0] + Globals::kPlayerWidth),
         round(playerPos[1] + Globals::kPlayerHeight), L'&', 14);

    // Draw Bullets
    for (auto &b : bullet) {
      if (b.Alive)
        Fill(round(b.Pos[0]), round(b.Pos[1]), round(b.Pos[0] + Globals::kBulletWidth),
             round(b.Pos[1] + Globals::kBulletHeight), L'O', 60);
    }

    // Draw Enemies
    int _livingEnemyCount = 0;
    for (auto &e : enemy) {
      if (e.Alive) {
        _livingEnemyCount++;
        Fill(round(e.Pos[0]), round(e.Pos[1]), round(e.Pos[0] + e.width), round(e.Pos[1] + e.height), L'T',
             75);
      }
    }
	// Draw Explosions
	for (auto &ex : explosions)
	{
		if (ex.Alive)
		{
			// draw circle of triangles at ex's current radius
			float radius = ex.GetRadius();
			float rand_0_6 = rand()%3;
			radius += rand_0_6;
			for(float theta = 0; theta < 2*Globals::M_PI; theta += 0.5 ){
				int xpos = round(ex.xPos0 + radius*cos(theta));
				int ypos = round(ex.yPos0 + radius*sin(theta));
				DrawTriangle(xpos, ypos, xpos + 2, ypos + 2, xpos -2, ypos + 2, L'*', 200);
			}
		}
	}
    /************************************************************************************
                                  Drawing End
    ************************************************************************************/
    // Progress to Next level
    if (_livingEnemyCount == 0) {
      Globals::Level++;
      enemy.clear();
      bullet.clear();
	 explosions.clear();
      OnUserCreate();
    }
    return true;
  }

  vector<Bullet> bullet;
  float playerPos[2];
  vector<Alien> enemy;
  vector<ParticleEffect> explosions;
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
