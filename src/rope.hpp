#ifndef ROPE_ROPE_HPP
#define ROPE_ROPE_HPP

#include "rl.hpp"

#include <string>
#include <vector>

inline constexpr Vector2 vec2(float x, float y) { return {x, y}; }

struct Point {
  enum class State { Free = 0, Locked, Animated };

  Vector2 pos;
  Vector2 last_pos;
  State state;

  constexpr Point(Vector2 pos, Vector2 last_pos, State state)
      : pos(pos), last_pos(last_pos), state(state) {}
  constexpr Point(Vector2 pos, State state)
      : pos(pos), last_pos(pos), state(state) {}
};

struct Link {
  size_t ia;
  size_t ib;
  float length;

  inline constexpr Link(size_t ia, size_t ib, float length)
      : ia(ia), ib(ib), length(length) {}
  inline constexpr Link(size_t ia, size_t ib, const Point &a, const Point &b)
      : ia(ia), ib(ib), length(Vector2Distance(a.pos, b.pos)) {}
  inline constexpr Link(size_t ia, size_t ib, const std::vector<Point> &points)
      : ia(ia), ib(ib),
        length(Vector2Distance(points[ia].pos, points[ib].pos)) {}
};

struct Rope {
  std::string name;
  std::vector<Point> points;
  std::vector<Link> links;

  inline Rope(const std::string &name,
                        const std::vector<Point> &points,
                        const std::vector<Link> &links)
      : name(name), points(points), links(links) {}
};

#endif // ROPE_ROPE_HPP
