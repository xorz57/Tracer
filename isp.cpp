#include "tracing.hpp"

#include <chrono>
#include <thread>
#include <vector>

void read_metadata(int i) {
  const tracing::DurationEvent event("read_metadata");
  std::this_thread::sleep_for(std::chrono::milliseconds(3));
}

void decode_pixels(int i) {
  const tracing::DurationEvent event("decode_pixels");
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void check_image_format(int i) {
  const tracing::DurationEvent event("check_image_format");
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void load_image_buffer(int i) {
  const tracing::DurationEvent event("load_image_buffer");
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void apply_blur(int i) {
  const tracing::DurationEvent event("apply_blur");
  std::this_thread::sleep_for(std::chrono::milliseconds(8));
}

void apply_sharpen(int i) {
  const tracing::DurationEvent event("apply_sharpen");
  std::this_thread::sleep_for(std::chrono::milliseconds(7));
}

void adjust_contrast(int i) {
  const tracing::DurationEvent event("adjust_contrast");
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void normalize_colors(int i) {
  const tracing::DurationEvent event("normalize_colors");
  std::this_thread::sleep_for(std::chrono::milliseconds(6));
}

void calculate_gradient(int i) {
  const tracing::DurationEvent event("calculate_gradient");
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void threshold_edges(int i) {
  const tracing::DurationEvent event("threshold_edges");
  std::this_thread::sleep_for(std::chrono::milliseconds(8));
}

void refine_edges(int i) {
  const tracing::DurationEvent event("refine_edges");
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void scale_image(int i) {
  const tracing::DurationEvent event("scale_image");
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
}

void crop_image(int i) {
  const tracing::DurationEvent event("crop_image");
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
}

void pad_image(int i) {
  const tracing::DurationEvent event("pad_image");
  std::this_thread::sleep_for(std::chrono::milliseconds(3));
}

void compress_image(int i) {
  const tracing::DurationEvent event("compress_image");
  std::this_thread::sleep_for(std::chrono::milliseconds(8));
}

void write_headers(int i) {
  const tracing::DurationEvent event("write_headers");
  std::this_thread::sleep_for(std::chrono::milliseconds(3));
}

void write_image_data(int i) {
  const tracing::DurationEvent event("write_image_data");
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void load_image(int i) {
  const tracing::DurationEvent event("load_image");
  check_image_format(i);
  read_metadata(i);
  load_image_buffer(i);
  decode_pixels(i);
}

void apply_filter(int i) {
  const tracing::DurationEvent event("apply_filter");
  apply_blur(i);
  apply_sharpen(i);
  adjust_contrast(i);
  normalize_colors(i);
}

void detect_edges(int i) {
  const tracing::DurationEvent event("detect_edges");
  calculate_gradient(i);
  threshold_edges(i);
  refine_edges(i);
}

void resize_image(int i) {
  const tracing::DurationEvent event("resize_image");
  scale_image(i);
  crop_image(i);
  pad_image(i);
}

void encode_image(int i) {
  const tracing::DurationEvent event("encode_image");
  compress_image(i);
  write_headers(i);
  write_image_data(i);
}

void task(int i) {
  load_image(i + 1);
  apply_filter(i + 1);
  detect_edges(i + 1);
  resize_image(i + 1);
  encode_image(i + 1);
}

int main() {
  std::vector<std::thread> threads;
  for (int i = 0; i < 16; ++i) {
    threads.emplace_back(task, i);
  }
  for (auto &t : threads) {
    t.join();
  }
  tracing::Tracer::get_instance().dump("trace.json", 4);
  return 0;
}
