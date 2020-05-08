
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


void DrawEnemy(olcConsoleGameEngine& game, const float& x, const float& y, const int& width, const int& height, const Behavior& behavior) {
  game.Fill(round(x), round(y), round(x + width), round(y + height), L'T', 75);
}

 void DrawExplosion(olcConsoleGameEngine& game, const int& xPos0, const int& yPos0, const int& radius) {
  for (float theta = 0; theta < 2 * M_PI; theta += 0.5) {
    int xpos = xPos0 + radius * cos(theta);
    int ypos = yPos0 + radius * sin(theta);
    game.DrawTriangle(xpos, ypos, xpos + 2, ypos + 2, xpos - 2, ypos + 2, L'*', 196);
  }
}


}  // namespace Drawing
