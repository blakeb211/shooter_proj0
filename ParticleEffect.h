/* The ParticleEffect class is used to draw 
   the explosions that happen when an enemy dies.
   The explosion is created using a timer and
   a radius that is updated as a function of the timer.
   Radius(timer) = const_1 + const_2 x radius - const_3 x radius^2
   */

#pragma once
#include "stdlibs.h"


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

        void UpdateTimer(float fElapsed) {
            _timer += fElapsed;
            if (_timer >= _maxTimer)
                Alive = false;
        }
        float GetRadius() { 
            float rand_0_3 = rand() % 3;
            return rand_0_3 + 0.5609 + 30 * _timer - 2 * _timer * _timer; 
        }

    private:
        float _maxTimer;
        float _timer;
};
