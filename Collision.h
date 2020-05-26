#pragma once
#include "Alien.h"
#include "Bullet.h"

namespace Collision {
  // Calc distance 
 
  // Calculate if Alien was hit by bullet
  bool AlienGotHit(const Alien &a, const Bullet &b) {
    int rightAlien = a.Pos[0] + a.width; // each enemy has own size
    int leftAlien = a.Pos[0];
    int bottomAlien = a.Pos[1] + a.height;
    int topAlien = a.Pos[1];
    int topBullet = b.Pos[1];
    int leftBullet = b.Pos[0];
    int rightBullet = b.Pos[0] + Globals::kBulletWidth;
    if (rightBullet > leftAlien && leftBullet < rightAlien &&
        topBullet > topAlien && topBullet < bottomAlien) {
      return true;
    } else {
      return false;
    }
  }

  // Calculate if player was hit by bullet 
  // A------B
  // |      |
  // |      |
  // |      |
  // C------D
  bool PlayerGotHit(const float * playerPos, const Bullet& b) {
    // player coords
    float playerAx = playerPos[0];
    float playerAy = playerPos[1];
    float playerBx = playerAx + Globals::kPlayerWidth;
    float playerBy = playerAy; 
    float playerCx = playerAx;
    float playerCy = playerAy + Globals::kPlayerHeight; 
    float playerDx = playerBx;
    float playerDy = playerCy;
    // bullet coords
    int bulletAx = b.Pos[0];
    int bulletAy = b.Pos[1];
    float bulletBx = bulletAx + Globals::kBulletWidth;
    float bulletBy = bulletAy; 
    float bulletCx = bulletAx;
    float bulletCy = bulletAy + Globals::kBulletHeight; 
    float bulletDx = bulletBx;
    float bulletDy = bulletCy;
    // check if any of BulletABCD points is inside of player  
    if (bulletAx > playerAx && bulletAx < playerBx && bulletAy > playerAy && bulletAy < playerCy)
      return true;
    if (bulletBx > playerAx && bulletBx < playerBx && bulletBy > playerAy && bulletBy < playerCy)
      return true;
    if (bulletCx > playerAx && bulletCx < playerBx && bulletCy > playerAy && bulletCy < playerCy)
      return true;
    if (bulletDx > playerAx && bulletDx < playerBx && bulletDy > playerAy && bulletDy < playerCy)
      return true;
    return false;
  } 
  
} // end namespace Collision
