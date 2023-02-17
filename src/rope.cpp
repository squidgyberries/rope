#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

#include "ropes.hpp"

#include <fmt/core.h>

constexpr int window_width = 800;
constexpr int window_height = 600;

constexpr int font_size = 20;

size_t orig_index = 0;

std::vector<Point> points;
std::vector<Link> links;

std::mutex points_links_mutex;

std::atomic<bool> should_close = false;
std::atomic<float> physics_delta_time;
std::atomic<bool> paused = true;

constexpr int num_iterations = 5;

void simulate() {
  float delta_time = 0.0f;
  auto last_frame = std::chrono::high_resolution_clock::now();

  while (!should_close) {
    if (!paused) {
      points_links_mutex.lock();

      for (Point &point : points) {
        if (point.state != Point::State::Free)
          continue;
        Vector2 orig_pos = point.pos;
        Vector2 diff = Vector2Subtract(point.pos, point.last_pos);
        point.pos = Vector2Add(point.pos, diff);
        Vector2 gravity = vec2(0.0f, 1.0f * delta_time * 0.06f);
        point.pos = Vector2Add(point.pos, gravity);
        point.last_pos = orig_pos;
      }

      for (int i = 0; i < num_iterations; i++) {
        for (Link &link : links) {
          float actual_length =
              Vector2Distance(points[link.ia].pos, points[link.ib].pos);
          if (link.length == actual_length)
            continue;
          float scale = link.length / actual_length;
          if (points[link.ia].state != Point::State::Free) {
            Vector2 offset =
                Vector2Subtract(points[link.ib].pos, points[link.ia].pos);
            Vector2 new_offset = Vector2Scale(offset, scale);
            points[link.ib].pos =
                Vector2Add(points[link.ia].pos, new_offset);
          } else if (points[link.ib].state != Point::State::Free) {
            Vector2 offset =
                Vector2Subtract(points[link.ia].pos, points[link.ib].pos);
            Vector2 new_offset = Vector2Scale(offset, scale);
            points[link.ia].pos =
                Vector2Add(points[link.ib].pos, new_offset);
          } else if (points[link.ia].state == Point::State::Free && points[link.ib].state == Point::State::Free) {
            Vector2 sum =
                Vector2Add(points[link.ia].pos, points[link.ib].pos);
            Vector2 center = Vector2Multiply(sum, vec2(0.5f, 0.5f));
            Vector2 offset_a =
                Vector2Subtract(points[link.ia].pos, center);
            Vector2 offset_b =
                Vector2Subtract(points[link.ib].pos, center);
            Vector2 new_offset_a = Vector2Scale(offset_a, scale);
            Vector2 new_offset_b = Vector2Scale(offset_b, scale);
            points[link.ia].pos = Vector2Add(center, new_offset_a);
            points[link.ib].pos = Vector2Add(center, new_offset_b);
          }
        }
      }

      points_links_mutex.unlock();
    }

    auto dur = std::chrono::high_resolution_clock::now() - last_frame;
    std::this_thread::sleep_for(
        std::chrono::duration<float, std::milli>(1000.0f / 60.0f) - dur);

    auto current_frame = std::chrono::high_resolution_clock::now();
    delta_time =
        std::chrono::duration<float, std::milli>(current_frame - last_frame)
            .count();
    last_frame = current_frame;
    physics_delta_time = delta_time;
  }
}

int main() {
  points = ropes[orig_index].points;
  links = ropes[orig_index].links;

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(window_width, window_height, "rope");

  // SetTargetFPS(10);

  std::thread simulate_thread(simulate);

  while (!should_close) {
    if (IsKeyPressed(KEY_SPACE))
      paused = !paused;
    if (IsKeyPressed(KEY_R)) {
      points_links_mutex.lock();
      points = ropes[orig_index].points;
      links = ropes[orig_index].links;
      points_links_mutex.unlock();
    }
    if (IsKeyPressed(KEY_RIGHT)) {
      if (orig_index + 1 < ropes.size()) {
        ++orig_index;
        paused = true;
        points_links_mutex.lock();
        points = ropes[orig_index].points;
        links = ropes[orig_index].links;
        points_links_mutex.unlock();
      }
    }
    if (IsKeyPressed(KEY_LEFT)) {
      if (orig_index > 0) {
        --orig_index;
        paused = true;
        points_links_mutex.lock();
        points = ropes[orig_index].points;
        links = ropes[orig_index].links;
        points_links_mutex.unlock();
      }
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    points_links_mutex.lock();

    for (const Link &link : links)
      DrawLineEx(points[link.ia].pos, points[link.ib].pos, 12.0f, GRAY);

    for (const Point &point : points) {
      Color color = BLACK;
      switch(point.state) {
        case Point::State::Free:
          color = DARKGRAY;
          break;
        case Point::State::Locked:
          color = RED;
          break;
        case Point::State::Animated:
          color = BLUE;
          break;
      }
      DrawCircleV(point.pos, 18.0f, color);
    }

    points_links_mutex.unlock();

    DrawText(
        fmt::format("Frame time: {:.2f} ms", GetFrameTime() * 1000.0f)
            .c_str(),
        10, 10, font_size, BLACK);

    DrawText(fmt::format("Frames per second: {}", GetFPS()).c_str(), 10,
                 30, font_size, BLACK);

    DrawText(
        fmt::format("Physics time: {:.2f} ms", physics_delta_time).c_str(), 10,
        60, font_size, BLACK);

    DrawText(fmt::format("Physics per second: {}",
                             (int)(1000.0f / physics_delta_time))
                     .c_str(),
                 10, 80, font_size, BLACK);

    if (paused)
      DrawText("paused", 10, window_height - 30, font_size, BLACK);
    else
      DrawText("playing", 10, window_height - 30, font_size, BLACK);

    std::string name_str = fmt::format("{} ({}/{})", ropes[orig_index].name,
                                       orig_index + 1, ropes.size());
    int name_length = MeasureText(name_str.c_str(), font_size);
    DrawText(name_str.c_str(), window_width - name_length - 10,
                 window_height - 30, font_size, BLACK);

    EndDrawing();

    should_close = WindowShouldClose();
  }

  simulate_thread.join();

  CloseWindow();

  return 0;
}
