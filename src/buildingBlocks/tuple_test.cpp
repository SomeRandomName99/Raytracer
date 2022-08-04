#include <gtest/gtest.h>

#include "include/tuple.h"


/* =========== Creation Test w = 1 =========== */
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

TEST(tuple_test, tuple_w_1_isPoint_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 1.0);
    EXPECT_FALSE(tuple.isPoint());
}

TEST(tuple_test, tuple_w_1_isVector_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 1.0);
    EXPECT_TRUE(tuple.isVector());
}

/* =========== Creation Test w = 0 =========== */
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

/* =========== Creation Test Vector =========== */
TEST(tuple_test, vector_creation_correct) {
    const auto Vec1        = Vector(4, -4, 3);
    const auto equalTuple  = Tuple(4, -4, 3, 1);
    EXPECT_TRUE(Vec1==equalTuple);
}

/* =========== Creation Test Point =========== */
TEST(tuple_test, point_creation_correct) {
    const auto point1      = Point(4, -4, 3);
    const auto equalTuple  = Tuple(4, -4, 3, 0);
    EXPECT_TRUE(point1==equalTuple);
}

/* =========== Operations Test equality =========== */
TEST(tuple_test, tuple_equality_equal_correct) {
	const auto tuple1 = Tuple(4.3, -4.2, 3.1, 0);
    const auto tuple2 = Tuple(4.3, -4.2, 3.1, 0);
    EXPECT_TRUE(tuple1==tuple2);
}

TEST(tuple_test, tuple_equality_notEqual_correct) {
	const auto tuple1 = Tuple(4.3, -4.2, 3.1, 0);
    const auto tuple2 = Tuple(4.3, -4.2, 3.1, 1);
    EXPECT_FALSE(tuple1==tuple2);
}
