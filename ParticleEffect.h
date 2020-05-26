/* The ParticleEffect class is used to draw 
   the explosions that happen when an enemy dies.
   The explosion is created using a timer and
   a radius that is updated as a function of the timer.
   Radius(timer) = const_1 + const_2 x radius - const_3 x radius^2
   */

#pragma once
#include "stdlibs.h"
#include "globals.h"
#include "Alien.h"

struct ParticleEffect {
    public:
        ParticleEffect() = default;
        ParticleEffect(int x, int y, float duration, float param2) {
            _timer = 0;
            Alive = true;
            xPos0 = x;
            yPos0 = y;
            _maxTimer = duration;  // 2 seconds long
        }
        bool Alive;
        int xPos0;
        int yPos0;
        auto UpdateTimer(float fElapsed) -> void {
            _timer += fElapsed;
            if (_timer >= _maxTimer)
                Alive = false;
        }
        auto GetRadius() -> float { 
            float rand_0_3 = rand() % 3;
            return rand_0_3 + 0.5609 + 30 * _timer - 2 * _timer * _timer; 
        }

    private:
        float _maxTimer;
        float _timer;
};

struct Piece {
    float x, y, vx, vy;
    Piece(float a, float b, float vxx, float vyy) : x(a), y(b), vx(vxx), vy(vyy) { }
};

struct ParticleEffect2 {
    public:
        ParticleEffect2() = default;
        ParticleEffect2(const Bullet & b, const Alien& a) {
            _timer = 0;
            Alive = true;
            _maxTimer = 0.2; // duration of explosion 
            frags.reserve(Globals::kBulletWidth * Globals::kBulletHeight);
            // subdivide bullet into fragments
            centerX = b.Pos[0] + Globals::kBulletWidth / 2.0; 
            centerY = b.Pos[1] + Globals::kBulletHeight / 2.0; 
            srand(time(NULL));
            for (int x = centerX; x < centerX + Globals::kBulletWidth; x++) {
                for (int y = centerY; y < centerY + Globals::kBulletHeight; y++) {
                    if (x - a.getCenterX() > 0) {
                        frags.emplace_back((float)x, (float)y, 1 + rand() % 4, -1.0*(1 + rand() % 4));
                    } else if (x - a.getCenterX() < 0) {
                        frags.emplace_back((float)x, (float)y, -1 * (1 + rand() % 4), -1.0*(1 + rand() % 4));
                    } else {
                        frags.emplace_back((float)x, (float)y, 0,-1.0 *(1 + rand() % 4));
                    }
                }
            }
        }
        ParticleEffect2(const Bullet & b, float *playerPos) {
            _timer = 0;
            Alive = true;
            _maxTimer = 0.2; // duration of explosion 
            frags.reserve(Globals::kBulletWidth * Globals::kBulletHeight);
            // subdivide bullet into fragments
            centerX = b.Pos[0] + Globals::kBulletWidth / 2.0; 
            centerY = b.Pos[1] + Globals::kBulletHeight / 2.0; 
            for (int x = centerX; x < centerX + Globals::kBulletWidth; x++) {
                for (int y = centerY; y < centerY + Globals::kBulletHeight; y++) {
                    if (x - (*playerPos + Globals::kPlayerWidth / 2) > 0) {
                        frags.emplace_back((float)x, (float)y, 1 + rand() % 4, +1.0 * (1 + rand() % 4));
                    } else if (x - (*playerPos + Globals::kPlayerWidth / 2) < 0) {
                        frags.emplace_back((float)x, (float)y, -1 * (1 + rand() % 4), +1.0 * (1 + rand() % 4));
                    } else {
                        frags.emplace_back((float)x, (float)y, 0, +1.0 * (1 + rand() % 4));
                    }
                }
            }
        }
        std::vector<Piece> frags; // bullet fragments
        bool Alive;
        int centerX;
        int centerY;
        auto Update(float fElapsed) -> void {
            _timer += fElapsed;
            if (_timer >= _maxTimer)
                Alive = false;
            // update frag positions
            for (auto & f : frags) {
              f.x += f.vx * 15.0 * fElapsed;
              f.y += f.vy * 15.0 * fElapsed;
            }
        }
    private:
        float _maxTimer;
        float _timer;
};

// enemy disintegration particle effect
struct ParticleEffect3 {
    public:
        ParticleEffect3() = default;
        ParticleEffect3(const Bullet & b, const Alien& a) {
            _timer = 0;
            Alive = true;
            _maxTimer = 0.2; // duration of explosion 
            color = a.color;
            frags.reserve(a.width * a.height);
            // subdivide enemy into fragments
            center_b_X = b.Pos[0] + Globals::kBulletWidth / 2.0; 
            center_b_Y = b.Pos[1] + Globals::kBulletHeight / 2.0; 
            for (float x = a.Pos[0]; x < a.Pos[0] + a.width; x+=2.0) {
                for (float y = a.Pos[1]; y < a.Pos[1] + a.height; y+=2.0) {
                    float distance = Globals::Distance(x, y, center_b_X, center_b_Y); 
                    if (a.getCenterX() - center_b_X > 0) {
                        frags.emplace_back((float)x, (float)y, 1.0 / distance  +1.0 * (1 + rand() % 4), -9.0 / distance -2.0*(1 + rand() % 4));
                    } else if (a.getCenterX() - center_b_X < 0) {
                        frags.emplace_back((float)x, (float)y, 1.0 / distance -1.0 * (1 + rand() % 4), -9.0 / distance -2.0*(1 + rand() % 4));
                    } else {
                        frags.emplace_back((float)x, (float)y, (-2 + rand() % 4), -9.0 / distance -2.0 *(1 + rand() % 4));
                    }
                }
            }
        }
        std::vector<Piece> frags; // enemy pixel fragments
        bool Alive;
        int center_b_X;
        int center_b_Y;
        Color color;
        auto Update(float fElapsed) -> void {
            _timer += fElapsed;
            if (_timer >= _maxTimer)
                Alive = false;
            // update frag positions
             for (auto & f : frags) {
              f.x += f.vx * 8.0 * fElapsed;
              f.y += f.vy * 8.0 * fElapsed;
            }
        }
    private:
        float _maxTimer;
        float _timer;
};

