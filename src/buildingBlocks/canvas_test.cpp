#include <gtest/gtest.h>
#include <algorithm>

#include "include/canvas.h"

/* =========== Creation Test =========== */
TEST(canvas_test, canvas_creation_correct) {
  const auto canvas1 = Canvas(20, 10);
  EXPECT_EQ(canvas1.width, 20);
  EXPECT_EQ(canvas1.height, 10);
}

TEST(canvas_test, canvas_create_pixels_length_correct) {
  const auto canvas1 = Canvas(20, 10);
  EXPECT_EQ(canvas1.pixels.size(), 200);
}

TEST(canvas_test, canvas_create_initialized_allZero_correct){
  const auto canvas1 = Canvas(20, 10);
  bool allZero = std::all_of(canvas1.pixels.begin(), canvas1.pixels.end(), 
                             [](Color i){ return i == Color(); });
  EXPECT_TRUE(allZero);
}

/* =========== Pixel Access Test =========== */
TEST(canvas_test, canvas_pixel_index_2D_to_1D){
  auto canvas = Canvas(20, 10);
  EXPECT_EQ(canvas.pixelIndex(5, 5), 5+5*20);
}

TEST(canvas_test, canvas_pixel_Write){
  auto canvas = Canvas(20, 10);
  auto red    = Color(1.0f, 0.0f, 0.0f);
  canvas.pixelWrite(red, 5, 5);
  EXPECT_EQ(canvas.pixelAt(5, 5), red);
}