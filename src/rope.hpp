#ifndef ROPE_ROPE_HPP
#define ROPE_ROPE_HPP

#include "rl.hpp"

inline constexpr rl::Vector2 vec2(float x, float y) { return {x, y}; }

struct Point {
  rl::Vector2 pos;
  rl::Vector2 last_pos;
  bool locked;

  constexpr Point(rl::Vector2 pos, rl::Vector2 last_pos, bool locked)
      : pos(pos), last_pos(last_pos), locked(locked) {}
  constexpr Point(rl::Vector2 pos, bool locked)
      : pos(pos), last_pos(pos), locked(locked) {}
};

struct Link {
  size_t ia;
  size_t ib;
  float length;

  inline constexpr Link(size_t ia, size_t ib, float length)
      : ia(ia), ib(ib), length(length) {}
  inline constexpr Link(size_t ia, size_t ib, Point a, Point b)
      : ia(ia), ib(ib), length(rl::Vector2Distance(a.pos, b.pos)) {}
};

#endif // ROPE_ROPE_HPP
