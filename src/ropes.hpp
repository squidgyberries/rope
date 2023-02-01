#ifndef ROPE_ROPES_HPP
#define ROPE_ROPES_HPP

#include "rope.hpp"

#include <vector>

// clang-format off
// From Sebastian Lague's AI video game idea video on YouTube
const std::vector<Point> lague_points = {
  Point(vec2(400.0f, 200.0f), true),
  Point(vec2(450.0f, 200.0f), false),
  Point(vec2(500.0f, 200.0f), false),
  Point(vec2(550.0f, 200.0f), false),
  Point(vec2(600.0f, 200.0f), false),
  Point(vec2(635.3553390594f, 164.6446609406f), false),
  Point(vec2(635.3553390594f, 235.3553390594f), false),
  Point(vec2(670.7106781188f, 200.0f), false)
};
const std::vector<Link> lague_links = {
  Link(0, 1, lague_points[0], lague_points[1]),
  Link(1, 2, lague_points[1], lague_points[2]),
  Link(2, 3, lague_points[2], lague_points[3]),
  Link(3, 4, lague_points[3], lague_points[4]),
  Link(4, 5, lague_points[4], lague_points[5]),
  Link(4, 6, lague_points[4], lague_points[6]),
  Link(5, 7, lague_points[5], lague_points[7]),
  Link(6, 7, lague_points[6], lague_points[7]),
  Link(5, 6, lague_points[5], lague_points[6])
};

const std::vector<Point> up_points = {
  Point(vec2(400.0f, 300.0f), true),
  Point(vec2(400.0f, 250.0f), false),
  Point(vec2(400.0f, 200.0f), false),
  Point(vec2(400.0f, 150.0f), false),
  Point(vec2(400.0f, 100.0f), false)
};
const std::vector<Link> up_links = {
  Link(0, 1, up_points[0], up_points[1]),
  Link(1, 2, up_points[1], up_points[2]),
  Link(2, 3, up_points[2], up_points[3]),
  Link(3, 4, up_points[3], up_points[4])
};

const std::vector<Point> v_points = {
  Point(vec2(400.0f, 300.0f), true),

  Point(vec2(425.0f, 250.0f), false),
  Point(vec2(450.0f, 200.0f), false),
  Point(vec2(475.0f, 150.0f), false),
  Point(vec2(500.0f, 100.0f), false),

  Point(vec2(375.0f, 250.0f), false),
  Point(vec2(350.0f, 200.0f), false),
  Point(vec2(325.0f, 150.0f), false),
  Point(vec2(300.0f, 100.0f), false)
};
const std::vector<Link> v_links = {
  Link(0, 1, v_points[0], v_points[1]),
  Link(1, 2, v_points[1], v_points[2]),
  Link(2, 3, v_points[2], v_points[3]),
  Link(3, 4, v_points[3], v_points[4]),

  Link(0, 5, v_points[0], v_points[5]),
  Link(5, 6, v_points[5], v_points[6]),
  Link(6, 7, v_points[6], v_points[7]),
  Link(7, 8, v_points[7], v_points[8])
};
// clang-format on

#endif // ROPE_ROPES_HPP
