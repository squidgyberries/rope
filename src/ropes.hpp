#ifndef ROPE_ROPES_HPP
#define ROPE_ROPES_HPP

#include "rope.hpp"

inline std::vector<Rope> getRopes() {
  // clang-format off
  // From Sebastian Lague's AI video game idea video on YouTube
  const std::vector<Point> lague_points = {
    Point(vec2(400.0f, 200.0f), Point::State::Locked),
    Point(vec2(450.0f, 200.0f)),
    Point(vec2(500.0f, 200.0f)),
    Point(vec2(550.0f, 200.0f)),
    Point(vec2(600.0f, 200.0f)),
    Point(vec2(635.3553390594f, 164.6446609406f)),
    Point(vec2(635.3553390594f, 235.3553390594f)),
    Point(vec2(670.7106781188f, 200.0f))
  };
  const std::vector<Link> lague_links = {
    Link(0, 1, lague_points),
    Link(1, 2, lague_points),
    Link(2, 3, lague_points),
    Link(3, 4, lague_points),
    Link(4, 5, lague_points),
    Link(4, 6, lague_points),
    Link(5, 7, lague_points),
    Link(6, 7, lague_points),
    Link(5, 6, lague_points)
  };

  const std::vector<Point> up_points = {
    Point(vec2(400.0f, 300.0f), Point::State::Locked),
    Point(vec2(400.0f, 250.0f)),
    Point(vec2(400.0f, 200.0f)),
    Point(vec2(400.0f, 150.0f)),
    Point(vec2(400.0f, 100.0f))
  };
  const std::vector<Link> up_links = {
    Link(0, 1, up_points),
    Link(1, 2, up_points),
    Link(2, 3, up_points),
    Link(3, 4, up_points)
  };

  const std::vector<Point> v_points = {
    Point(vec2(400.0f, 300.0f), Point::State::Locked),

    // Right
    Point(vec2(425.0f, 250.0f)),
    Point(vec2(450.0f, 200.0f)),
    Point(vec2(475.0f, 150.0f)),
    Point(vec2(500.0f, 100.0f)),

    // Left
    Point(vec2(375.0f, 250.0f)),
    Point(vec2(350.0f, 200.0f)),
    Point(vec2(325.0f, 150.0f)),
    Point(vec2(300.0f, 100.0f))
  };
  const std::vector<Link> v_links = {
    // Right
    Link(0, 1, v_points),
    Link(1, 2, v_points),
    Link(2, 3, v_points),
    Link(3, 4, v_points),

    // Left
    Link(0, 5, v_points),
    Link(5, 6, v_points),
    Link(6, 7, v_points),
    Link(7, 8, v_points)
  };

  const std::vector<Point> x_points = {
    Point(vec2(400.0f, 300.0f), Point::State::Locked),

    // Top left
    Point(vec2(350.0f, 250.0f)),
    Point(vec2(300.0f, 200.0f)),
    Point(vec2(250.0f, 150.0f)),
    Point(vec2(200.0f, 100.0f)),

    // Bottom left
    Point(vec2(350.0f, 350.0f)),
    Point(vec2(300.0f, 400.0f)),
    Point(vec2(250.0f, 450.0f)),
    Point(vec2(200.0f, 500.0f)),

    // Bottom right
    Point(vec2(450.0f, 350.0f)),
    Point(vec2(500.0f, 400.0f)),
    Point(vec2(550.0f, 450.0f)),
    Point(vec2(600.0f, 500.0f)),

    // Top right
    Point(vec2(450.0f, 250.0f)),
    Point(vec2(500.0f, 200.0f)),
    Point(vec2(550.0f, 150.0f)),
    Point(vec2(600.0f, 100.0f))
  };
  const std::vector<Link> x_links = {
    // Top left
    Link(0, 1, x_points),
    Link(1, 2, x_points),
    Link(2, 3, x_points),
    Link(3, 4, x_points),

    // Bottom left
    Link(0, 5, x_points),
    Link(5, 6, x_points),
    Link(6, 7, x_points),
    Link(7, 8, x_points),

    // Bottom right
    Link(0, 9, x_points),
    Link(9, 10, x_points),
    Link(10, 11, x_points),
    Link(11, 12, x_points),

    // Top right
    Link(0, 13, x_points),
    Link(13, 14, x_points),
    Link(14, 15, x_points),
    Link(15, 16, x_points)
  };

  const std::vector<Point> x_slack_points = {
    Point(vec2(400.0f, 300.0f)),

    // Top left
    Point(vec2(350.0f, 250.0f)),
    Point(vec2(300.0f, 200.0f)),
    Point(vec2(250.0f, 150.0f)),
    Point(vec2(200.0f, 100.0f), Point::State::Locked),

    // Bottom left
    Point(vec2(350.0f, 350.0f)),
    Point(vec2(300.0f, 400.0f)),
    Point(vec2(250.0f, 450.0f)),
    Point(vec2(200.0f, 500.0f), Point::State::Locked),

    // Bottom right
    Point(vec2(450.0f, 350.0f)),
    Point(vec2(500.0f, 400.0f)),
    Point(vec2(550.0f, 450.0f)),
    Point(vec2(600.0f, 500.0f), Point::State::Locked),

    // Top right
    Point(vec2(450.0f, 250.0f)),
    Point(vec2(500.0f, 200.0f)),
    Point(vec2(550.0f, 150.0f)),
    Point(vec2(600.0f, 100.0f), Point::State::Locked)
  };
  const std::vector<Link> x_slack_links = {
    // Top left
    Link(0, 1, 75.0f),
    Link(1, 2, 75.0f),
    Link(2, 3, 75.0f),
    Link(3, 4, 75.0f),

    // Bottom left
    Link(0, 5, 75.0f),
    Link(5, 6, 75.0f),
    Link(6, 7, 75.0f),
    Link(7, 8, 75.0f),

    // Bottom right
    Link(0, 9, 75.0f),
    Link(9, 10, 75.0f),
    Link(10, 11, 75.0f),
    Link(11, 12, 75.0f),

    // Top right
    Link(0, 13, 75.0f),
    Link(13, 14, 75.0f),
    Link(14, 15, 75.0f),
    Link(15, 16, 75.0f)
  };
  const std::vector<Point> upside_down_v_points = {
    Point(vec2(400.0f, 300.0f), Point::State::Locked),

    // Right
    Point(vec2(425.0f, 350.0f)),
    Point(vec2(450.0f, 400.0f)),
    Point(vec2(475.0f, 450.0f)),
    Point(vec2(500.0f, 500.0f)),

    // Left
    Point(vec2(375.0f, 350.0f)),
    Point(vec2(350.0f, 400.0f)),
    Point(vec2(325.0f, 450.0f)),
    Point(vec2(300.0f, 500.0f))
  };
  const std::vector<Link> upside_down_v_links = {
    // Right
    Link(0, 1, upside_down_v_points),
    Link(1, 2, upside_down_v_points),
    Link(2, 3, upside_down_v_points),
    Link(3, 4, upside_down_v_points),

    // Left
    Link(0, 5, upside_down_v_points),
    Link(5, 6, upside_down_v_points),
    Link(6, 7, upside_down_v_points),
    Link(7, 8, upside_down_v_points)
  };
  // clang-format on
  void (*upside_down_v_update)(Rope &, float, Vector2) = [](Rope &self,
                                                            float delta_time,
                                                            Vector2 mouse_pos) {
    for (Point &point : self.points) {
      if (point.state != Point::State::Free)
        continue;
      Vector2 orig_pos = point.pos;
      Vector2 diff = Vector2Subtract(point.pos, point.last_pos);
      point.pos = Vector2Add(point.pos, diff);
      Vector2 gravity = vec2(0.0f, -1.0f * delta_time * 0.06f);
      point.pos = Vector2Add(point.pos, gravity);
      point.last_pos = orig_pos;
    }

    if (self.grabbed) {
      self.grabbed_point->pos = Vector2Add(mouse_pos, self.grabbed_offset);
    }

    for (int i = 0; i < 5; i++) {
      for (Link &link : self.links) {
        float actual_length =
            Vector2Distance(self.points[link.ia].pos, self.points[link.ib].pos);
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
          Vector2 sum =
              Vector2Add(self.points[link.ia].pos, self.points[link.ib].pos);
          Vector2 center = Vector2Multiply(sum, vec2(0.5f, 0.5f));
          Vector2 offset_a = Vector2Subtract(self.points[link.ia].pos, center);
          Vector2 offset_b = Vector2Subtract(self.points[link.ib].pos, center);
          Vector2 new_offset_a = Vector2Scale(offset_a, scale);
          Vector2 new_offset_b = Vector2Scale(offset_b, scale);
          self.points[link.ia].pos = Vector2Add(center, new_offset_a);
          self.points[link.ib].pos = Vector2Add(center, new_offset_b);
        }
      }
    }
  };
  // clang-format off

  return {
    Rope("Sebastian Lague", lague_points, lague_links),
    Rope("Up", up_points, up_links),
    Rope("V", v_points, v_links),
    Rope("X", x_points, x_links),
    Rope("X slack", x_slack_points, x_slack_links),
    Rope("Upside down V", upside_down_v_points, upside_down_v_links, upside_down_v_update)
  };
  // clang-format on
}

#endif // ROPE_ROPES_HPP
