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
const std::vector<std::vector<Point>> orig_points = {lague_points, up_points,
                                                     v_points, x_points, x_slack_points};
const std::vector<std::vector<Link>> orig_links = {lague_links, up_links,
                                                   v_links, x_links, x_slack_links};
const std::vector<std::string> orig_names = {"Sebastian Lague", "Up", "V", "X", "X slack"};

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
        if (point.locked)
          continue;
        rl::Vector2 orig_pos = point.pos;
        rl::Vector2 diff = rl::Vector2Subtract(point.pos, point.last_pos);
        point.pos = rl::Vector2Add(point.pos, diff);
        rl::Vector2 gravity = vec2(0.0f, 1.0f * delta_time * 0.06f);
        point.pos = rl::Vector2Add(point.pos, gravity);
        point.last_pos = orig_pos;
      }

      for (int i = 0; i < num_iterations; i++) {
        for (Link &link : links) {
          float actual_length =
              rl::Vector2Distance(points[link.ia].pos, points[link.ib].pos);
          if (link.length == actual_length)
            continue;
          float scale = link.length / actual_length;
          if (points[link.ia].locked) {
            rl::Vector2 offset =
                rl::Vector2Subtract(points[link.ib].pos, points[link.ia].pos);
            rl::Vector2 new_offset = rl::Vector2Scale(offset, scale);
            points[link.ib].pos =
                rl::Vector2Add(points[link.ia].pos, new_offset);
          } else if (points[link.ib].locked) {
            rl::Vector2 offset =
                rl::Vector2Subtract(points[link.ia].pos, points[link.ib].pos);
            rl::Vector2 new_offset = rl::Vector2Scale(offset, scale);
            points[link.ia].pos =
                rl::Vector2Add(points[link.ib].pos, new_offset);
          } else if (!points[link.ia].locked && !points[link.ib].locked) {
            rl::Vector2 sum =
                rl::Vector2Add(points[link.ia].pos, points[link.ib].pos);
            rl::Vector2 center = rl::Vector2Multiply(sum, vec2(0.5f, 0.5f));
            rl::Vector2 offset_a =
                rl::Vector2Subtract(points[link.ia].pos, center);
            rl::Vector2 offset_b =
                rl::Vector2Subtract(points[link.ib].pos, center);
            rl::Vector2 new_offset_a = rl::Vector2Scale(offset_a, scale);
            rl::Vector2 new_offset_b = rl::Vector2Scale(offset_b, scale);
            points[link.ia].pos = rl::Vector2Add(center, new_offset_a);
            points[link.ib].pos = rl::Vector2Add(center, new_offset_b);
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
  points = orig_points[orig_index];
  links = orig_links[orig_index];

  rl::SetConfigFlags(rl::FLAG_MSAA_4X_HINT);
  rl::SetConfigFlags(rl::FLAG_VSYNC_HINT);
  rl::InitWindow(window_width, window_height, "rope");

  // rl::SetTargetFPS(10);

  std::thread simulate_thread(simulate);

  while (!should_close) {
    if (rl::IsKeyPressed(rl::KEY_SPACE))
      paused = !paused;
    if (rl::IsKeyPressed(rl::KEY_R)) {
      points_links_mutex.lock();
      points = orig_points[orig_index];
      links = orig_links[orig_index];
      points_links_mutex.unlock();
    }
    if (rl::IsKeyPressed(rl::KEY_RIGHT)) {
      if (orig_index + 1 < orig_points.size()) {
        ++orig_index;
        points_links_mutex.lock();
        points = orig_points[orig_index];
        links = orig_links[orig_index];
        points_links_mutex.unlock();
      }
    }
    if (rl::IsKeyPressed(rl::KEY_LEFT)) {
      if (orig_index > 0) {
        --orig_index;
        points_links_mutex.lock();
        points = orig_points[orig_index];
        links = orig_links[orig_index];
        points_links_mutex.unlock();
      }
    }

    rl::BeginDrawing();

    rl::ClearBackground(rl::RAYWHITE);

    points_links_mutex.lock();

    for (const Link &link : links)
      rl::DrawLineEx(points[link.ia].pos, points[link.ib].pos, 12.0f, rl::GRAY);

    for (const Point &point : points) {
      rl::Color color = rl::DARKGRAY;
      if (point.locked)
        color = rl::RED;
      rl::DrawCircleV(point.pos, 18.0f, color);
    }

    points_links_mutex.unlock();

    rl::DrawText(
        fmt::format("Frame time: {:.2f} ms", rl::GetFrameTime() * 1000.0f)
            .c_str(),
        10, 10, font_size, rl::BLACK);

    rl::DrawText(fmt::format("Frames per second: {}", rl::GetFPS()).c_str(), 10,
                 30, font_size, rl::BLACK);

    rl::DrawText(
        fmt::format("Physics time: {:.2f} ms", physics_delta_time).c_str(), 10,
        60, font_size, rl::BLACK);

    rl::DrawText(fmt::format("Physics per second: {}",
                             (int)(1000.0f / physics_delta_time))
                     .c_str(),
                 10, 80, font_size, rl::BLACK);

    if (paused)
      rl::DrawText("paused", 10, window_height - 30, font_size, rl::BLACK);
    else
      rl::DrawText("playing", 10, window_height - 30, font_size, rl::BLACK);

    std::string name_str = fmt::format("{} ({}/{})", orig_names[orig_index],
                                       orig_index + 1, orig_points.size());
    int name_length = rl::MeasureText(name_str.c_str(), font_size);
    rl::DrawText(name_str.c_str(), window_width - name_length - 10,
                 window_height - 30, font_size, rl::BLACK);

    rl::EndDrawing();

    should_close = rl::WindowShouldClose();
  }

  simulate_thread.join();

  rl::CloseWindow();

  return 0;
}
