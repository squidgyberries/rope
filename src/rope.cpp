#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

#include "ropes.hpp"

#include <fmt/core.h>

constexpr int window_width = 800;
constexpr int window_height = 600;

constexpr float link_width = 12.0f;
constexpr float point_radius = 18.0f;

constexpr int font_size = 20;

size_t rope_index = 0;

const std::vector<Rope> ropes = getRopes();
Rope rope = ropes[rope_index];

std::mutex rope_mutex;

std::atomic<bool> should_close = false;
std::atomic<float> physics_delta_time = 0.0f;
std::atomic<bool> paused = true;
std::atomic<Vector2> mouse_pos = vec2(0.0f, 0.0f);

void simulate() {
  float delta_time = 0.0f;
  auto last_frame = std::chrono::steady_clock::now();

  while (!should_close) {
    if (!paused) {
      rope_mutex.lock();
      rope.update(rope, delta_time, mouse_pos);
      rope_mutex.unlock();
    }

    auto dur = std::chrono::steady_clock::now() - last_frame;
    std::this_thread::sleep_for(
        std::chrono::duration<float, std::milli>(1000.0f / 60.0f) - dur);

    auto current_frame = std::chrono::steady_clock::now();
    delta_time =
        std::chrono::duration<float, std::milli>(current_frame - last_frame)
            .count();
    last_frame = current_frame;
    physics_delta_time = delta_time;
  }
}

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(window_width, window_height, "rope");

  // SetTargetFPS(10);

  std::thread simulate_thread(simulate);

  while (!should_close) {
    mouse_pos = GetMousePosition();

    if (IsKeyPressed(KEY_SPACE)) {
      paused = !paused;
    }
    if (IsKeyPressed(KEY_R)) {
      rope_mutex.lock();
      rope = ropes[rope_index];
      rope_mutex.unlock();
    }
    if (IsKeyPressed(KEY_RIGHT)) {
      if (rope_index + 1 < ropes.size()) {
        ++rope_index;
        paused = true;
        rope_mutex.lock();
        rope = ropes[rope_index];
        rope_mutex.unlock();
      }
    }
    if (IsKeyPressed(KEY_LEFT)) {
      if (rope_index > 0) {
        --rope_index;
        paused = true;
        rope_mutex.lock();
        rope = ropes[rope_index];
        rope_mutex.unlock();
      }
    }

    rope_mutex.lock();

    for (Point &point : rope.points) {
      if (point.state == Point::State::Locked &&
          IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
          Vector2DistanceSqr(point.pos, mouse_pos) <
              point_radius * point_radius) {
        rope.grabbed_point = &point;
        rope.grabbed_offset = Vector2Subtract(point.pos, mouse_pos);
      }
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      rope.grabbed_point = nullptr;
    }

    rope.grabbed = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && rope.grabbed_point;

    rope_mutex.unlock();

    BeginDrawing();

    ClearBackground(RAYWHITE);

    rope_mutex.unlock();

    for (const Link &link : rope.links) {
      DrawLineEx(rope.points[link.ia].pos, rope.points[link.ib].pos, link_width,
                 GRAY);
    }

    for (const Point &point : rope.points) {
      Color color = BLACK;
      switch (point.state) {
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
      DrawCircleV(point.pos, point_radius, color);
    }

    rope_mutex.unlock();

    DrawText(
        fmt::format("Frame time: {:.2f} ms", GetFrameTime() * 1000.0f).c_str(),
        10, 10, font_size, BLACK);

    DrawText(fmt::format("Frames per second: {}", GetFPS()).c_str(), 10, 30,
             font_size, BLACK);

    DrawText(fmt::format("Physics time: {:.2f} ms", physics_delta_time).c_str(),
             10, 60, font_size, BLACK);

    DrawText(fmt::format("Physics per second: {}",
                         (int)(1000.0f / physics_delta_time))
                 .c_str(),
             10, 80, font_size, BLACK);

    if (paused) {
      DrawText("paused", 10, window_height - 30, font_size, BLACK);
    } else {
      DrawText("playing", 10, window_height - 30, font_size, BLACK);
    }

    std::string name_str = fmt::format("{} ({}/{})", ropes[rope_index].name,
                                       rope_index + 1, ropes.size());
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
