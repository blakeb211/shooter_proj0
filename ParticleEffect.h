#pragma once

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
  float GetRadius() { return (0.5609 + (30 * _timer) - (2 * _timer * _timer)); }

 private:
  float _maxTimer;
  float _timer;
};