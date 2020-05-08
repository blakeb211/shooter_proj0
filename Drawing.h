
#include "globals.h"
#include "Alien.h"
#include "Drop.h"
#include "olcConsoleGameEngine.h"

namespace Drawing {

void ClearScreen(olcConsoleGameEngine& game) {
  game.Fill(0, 0, Globals::kScreenWidth, Globals::kScreenHeight, L' ', 0);
}

void DrawRain(olcConsoleGameEngine& game, Drop & d) {
  game.Fill(round(d.x), round(d.y), round(d.x + d.dropWidth),
       round(d.y + d.dropHeight), L'!', d.dropColor);
}

void DrawPlayer(olcConsoleGameEngine& game, float * playerPos) {
  game.Fill(round(playerPos[0]), round(playerPos[1]),
       round(playerPos[0] + Globals::kPlayerWidth),
       round(playerPos[1] + Globals::kPlayerHeight), L'&', 175);
}

void DrawBullet(olcConsoleGameEngine& game, float xpos, float ypos, int width, int height) {
  game.Fill(round(xpos), round(ypos),
       round(xpos + width),
       round(ypos + height), L'O', 60);
}


void DrawEnemy(olcConsoleGameEngine& game, Alien & e) {

  game.Fill(round(e.Pos[0]), round(e.Pos[1]), round(e.Pos[0] + e.width), round(e.Pos[1] + e.height), L'T', 165);
  if (e.Cracked) {
    // these values could be cached
    int leftCrackX = e.Pos[0];
    int leftCrackY = e.Pos[1] + e.height / 2 + 2;
    int topCrackX = e.Pos[0] + e.width / 4 - 1;
    int topCrackY = e.Pos[1];
    int rightCrackX = e.Pos[0] + e.width;
    int rightCrackY = e.Pos[1] + e.height - 2;
    int bottomCrackX = e.Pos[0] + e.width;
    int bottomCrackY = e.Pos[1] + e.height;
    int centerX = e.Pos[0] + e.width / 2;
    int centerY = e.Pos[1] + e.height / 2;
    game.DrawLine(leftCrackX, leftCrackY, centerX, centerY, L'-', 1);
    game.DrawLine(centerX, centerY, topCrackX, topCrackY, L'|', 1);
    game.DrawLine(centerX, centerY, bottomCrackX, bottomCrackY, L'|', 1);
    game.DrawLine(centerX + 2, centerY + 2, rightCrackX, rightCrackY, L'-', 1); 
    game.DrawLine(centerX + 3, centerY, rightCrackX, e.Pos[1], L'-', 1); 
    game.DrawLine(centerX + 3, centerY, centerX - 1, e.Pos[1] + e.height, L'-', 1); 
  }

}

 void DrawExplosion(olcConsoleGameEngine& game, const int& xPos0, const int& yPos0, const int& radius) {
  for (float theta = 0; theta < 2 * M_PI; theta += 0.5) {
    int xpos = xPos0 + radius * cos(theta);
    int ypos = yPos0 + radius * sin(theta);
    game.DrawTriangle(xpos, ypos, xpos + 2, ypos + 2, xpos - 2, ypos + 2, L'*', 196);
  }
}


}  // namespace Drawing
