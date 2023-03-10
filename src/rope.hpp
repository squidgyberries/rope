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
  Point *grabbed_point;
  Vector2 grabbed_offset;
  bool grabbed;
  void (*update)(Rope &, float, Vector2);

  inline Rope(std::string name, std::vector<Point> points,
              std::vector<Link> links)
      : name(name), points(points), links(links), grabbed_point(nullptr),
        grabbed_offset(vec2(0.0f, 0.0f)), grabbed(false),
        update(update = [](Rope &self, float delta_time, Vector2 mouse_pos) {
          for (Point &point : self.points) {
            if (point.state != Point::State::Free)
              continue;
            Vector2 orig_pos = point.pos;
            Vector2 diff = Vector2Subtract(point.pos, point.last_pos);
            point.pos = Vector2Add(point.pos, diff);
            Vector2 gravity = vec2(0.0f, 1.0f * delta_time * 0.06f);
            point.pos = Vector2Add(point.pos, gravity);
            point.last_pos = orig_pos;
          }

          if (self.grabbed) {
            self.grabbed_point->pos =
                Vector2Add(mouse_pos, self.grabbed_offset);
          }

          for (int i = 0; i < 5; i++) {
            for (Link &link : self.links) {
              float actual_length = Vector2Distance(self.points[link.ia].pos,
                                                    self.points[link.ib].pos);
              if (link.length == actual_length)
                continue;
              float scale = link.length / actual_length;
              if (self.points[link.ia].state != Point::State::Free) {
                Vector2 offset = Vector2Subtract(self.points[link.ib].pos,
                                                 self.points[link.ia].pos);
                Vector2 new_offset = Vector2Scale(offset, scale);
                self.points[link.ib].pos =
                    Vector2Add(self.points[link.ia].pos, new_offset);
              } else if (self.points[link.ib].state != Point::State::Free) {
                Vector2 offset = Vector2Subtract(self.points[link.ia].pos,
                                                 self.points[link.ib].pos);
                Vector2 new_offset = Vector2Scale(offset, scale);
                self.points[link.ia].pos =
                    Vector2Add(self.points[link.ib].pos, new_offset);
              } else if (self.points[link.ia].state == Point::State::Free &&
                         self.points[link.ib].state == Point::State::Free) {
                Vector2 sum = Vector2Add(self.points[link.ia].pos,
                                         self.points[link.ib].pos);
                Vector2 center = Vector2Multiply(sum, vec2(0.5f, 0.5f));
                Vector2 offset_a =
                    Vector2Subtract(self.points[link.ia].pos, center);
                Vector2 offset_b =
                    Vector2Subtract(self.points[link.ib].pos, center);
                Vector2 new_offset_a = Vector2Scale(offset_a, scale);
                Vector2 new_offset_b = Vector2Scale(offset_b, scale);
                self.points[link.ia].pos = Vector2Add(center, new_offset_a);
                self.points[link.ib].pos = Vector2Add(center, new_offset_b);
              }
            }
          }
        }) {}
  inline Rope(std::string name, std::vector<Point> points,
              std::vector<Link> links, void (*update)(Rope &, float, Vector2))
      : name(name), points(points), links(links), grabbed_point(nullptr),
        grabbed_offset(vec2(0.0f, 0.0f)), grabbed(false), update(update) {}
};

#endif // ROPE_ROPE_HPP
