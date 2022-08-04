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
    EXPECT_TRUE(tuple.isPoint());
}

TEST(tuple_test, tuple_w_1_isVector_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 1.0);
    EXPECT_FALSE(tuple.isVector());
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

TEST(tuple_test, tuple_w_0_isPoint_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 0);
    EXPECT_FALSE(tuple.isPoint());
}

TEST(tuple_test, tuple_w_0_isVector_correct) {
	const auto tuple = Tuple(4.3, -4.2, 3.1, 0);
    EXPECT_TRUE(tuple.isVector());
}


/* =========== Creation Test Vector =========== */
TEST(tuple_test, vector_creation_correct) {
    const auto Vec1        = Vector(4, -4, 3);
    const auto equalTuple  = Tuple(4, -4, 3, 0);
    EXPECT_TRUE(Vec1==equalTuple);
}

/* =========== Creation Test Point =========== */
TEST(tuple_test, point_creation_correct) {
    const auto point1      = Point(4, -4, 3);
    const auto equalTuple  = Tuple(4, -4, 3, 1);
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

/* =========== Operations Test addition =========== */
TEST(tuple_test, tuple_addition_correct) {
    const auto tuple1 = Tuple(3, -2, 5, 1);
    const auto tuple2 = Tuple(-2, 3, 1, 0);
    const auto tuple3 = Tuple(1, 1, 6, 1);
    EXPECT_EQ(tuple1+tuple2, tuple3);
}

/* =========== Operations Test subtraction =========== */
TEST(tuple_test, tuple_subtraction_vector_vector_isVector_correct) {
    const auto vec1 = Vector(3, 2, 1);
    const auto vec2 = Vector(5, 6, 7);
    const auto vec3 = Vector(-2, -4, -6);
    EXPECT_EQ(vec1-vec2, vec3);
}

TEST(tuple_test, tuple_subtraction_point_vector_isPoint_correct) {
    const auto pnt1 = Point(3, 2, 1);
    const auto vec1 = Vector(5, 6, 7);
    const auto pnt2 = Point(-2, -4, -6);
    EXPECT_EQ(pnt1-vec1, pnt2);
}

TEST(tuple_test, tuple_subtraction_point_point_isVector_correct) {
    const auto pnt1 = Point(3, 2, 1);
    const auto pnt2 = Point(5, 6, 7);
    const auto vec1 = Vector(-2, -4, -6);
    EXPECT_EQ(pnt1-pnt2, vec1);
}

/* =========== Operations Test scalar multiplication =========== */
TEST(tuple_test, tuple_scalar_multiplication_by_scalar_correct) {
    const auto tuple1 = Tuple(1, -2, 3, -4);
    const auto tuple2 = Tuple(3.5, -7, 10.5, -14);
    EXPECT_EQ(tuple1*3.5, tuple2);
}

TEST(tuple_test, tuple_scalar_multiplication_by_fraction_correct) {
    const auto tuple1 = Tuple(1, -2, 3, -4);
    const auto tuple2 = Tuple(0.5, -1, 1.5, -2);
    EXPECT_EQ(tuple1*0.5, tuple2);
}

/* =========== Operations Test scalar division =========== */
TEST(tuple_test, tuple_scalar_division_by_scalar_correct) {
    const auto tuple1 = Tuple(1, -2, 3, -4);
    const auto tuple2 = Tuple(0.5, -1, 1.5, -2);
    EXPECT_EQ(tuple1/2, tuple2);
}