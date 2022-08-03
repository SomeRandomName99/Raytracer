#include <gtest/gtest.h>

#include "include/tuple.h"

TEST(tuple_test, tuple_w_1_x_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 1.0);
    EXPECT_FLOAT_EQ(tuple.x, 4.3);
}

TEST(tuple_test, tuple_w_1_y_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 1.0);
    EXPECT_FLOAT_EQ(tuple.y, -4.2);
}

TEST(tuple_test, tuple_w_1_z_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 1.0);
    EXPECT_FLOAT_EQ(tuple.z, 3.1);
}

TEST(tuple_test, tuple_w_1_w_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 1.0);
    EXPECT_FLOAT_EQ(tuple.w, 1.0);
}

TEST(tuple_test, tuple_w_0_x_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 0);
    EXPECT_FLOAT_EQ(tuple.x, 4.3);
}

TEST(tuple_test, tuple_w_0_y_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 0);
    EXPECT_FLOAT_EQ(tuple.y, -4.2);
}

TEST(tuple_test, tuple_w_0_z_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 0);
    EXPECT_FLOAT_EQ(tuple.z, 3.1);
}

TEST(tuple_test, tuple_w_0_w_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 0);
    EXPECT_FLOAT_EQ(tuple.w, 0);
}
