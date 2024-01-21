#include <gtest/gtest.h>
#include <algorithm>
#include <sstream>
#include <ranges>

#include "Canvas.hpp"

using namespace raytracer;
using namespace utility;

/* =========== Creation Test =========== */
TEST(canvas_tests, canvas_creation_correct) {
  const auto canvas1 = Canvas(20, 10);
  EXPECT_EQ(canvas1.width, 20);
  EXPECT_EQ(canvas1.height, 10);
}

TEST(canvas_tests, canvas_create_pixels_length_correct) {
  const auto canvas1 = Canvas(20, 10);
  EXPECT_EQ(canvas1.pixels.size(), 200);
}

TEST(canvas_tests, canvas_create_initialized_allZero_correct){
  const auto canvas1 = Canvas(20, 10);
  bool allZero = std::all_of(canvas1.pixels.begin(), canvas1.pixels.end(), 
                             [](Color i){ return i == Color(); });
  EXPECT_TRUE(allZero);
}

/* =========== Pixel Access Test =========== */
TEST(canvas_tests, canvas_pixel_index_2D_to_1D){
  auto canvas = Canvas(20, 10);
  EXPECT_EQ(canvas.pixelIndex(5, 5), 5+5*20);
}

TEST(canvas_tests, canvas_pixel_Write){
  auto canvas = Canvas(20, 10);
  auto red    = Color(1.0f, 0.0f, 0.0f);
  canvas.pixelWrite(red, 5, 5);
  EXPECT_EQ(canvas.pixelAt(5, 5), red);
}

TEST(canvas_tests, canvas_PPM_header){
  auto canvas = Canvas(5, 3);
  std::stringstream stream;
  
  canvas.canvasToPPM(stream);

  std::string line;
  std::getline(stream, line);
  EXPECT_EQ(line, "P3");

  std::getline(stream, line);
  EXPECT_EQ(line, "5 3");

  std::getline(stream, line);
  EXPECT_EQ(line, "255");
}

TEST(canvas_tests, canvas_PPM_short_data){
  auto canvas = Canvas(5, 3);
  canvas.pixelWrite(Color(1.5f, 0.0f, 0.0f), 0, 0);
  canvas.pixelWrite(Color(0.0f, 0.5f, 0.0f), 2, 1);
  canvas.pixelWrite(Color(-0.5f, 0.0f, 1.0f), 4, 2);
  
  std::stringstream stream;
  canvas.canvasToPPM(stream);

  constexpr char const* expected =
        "P3\n"
        "5 3\n"
        "255\n"
        "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n";

  std::string PPM{stream.str()};
  EXPECT_EQ(PPM, expected);
}

TEST(canvas_tests, canvas_PPM_long_data){
  auto canvas = Canvas(10, 2);
  for(size_t&& col : std::views::iota(0u, canvas.width)){
    for(size_t&& row : std::views::iota(0u, canvas.height)){
      canvas.pixelWrite(Color(1, 0.8, 0.6), col, row);
    }
  }
  std::stringstream stream;
  canvas.canvasToPPM(stream);

  constexpr char const* expected = "P3\n10 2\n255\n"
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
        "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
        "153 255 204 153 255 204 153 255 204 153 255 204 153\n";

  std::string PPM{stream.str()};
  EXPECT_EQ(PPM, expected);
}

TEST(canvas_tests, canvas_PPM_ends_with_new_line){
  auto canvas = Canvas(5, 3);
  std::stringstream stream;
  canvas.canvasToPPM(stream);
  
  auto output = stream.str();
  EXPECT_EQ(output.back(), '\n');
}