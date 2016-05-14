#include "Vect.h"
#include "gtest/gtest.h"
#include <math.h>

/*
 * VectTest
 */

TEST(VectTest, Magnitude) {
  Vect a(1, 0, 0);
  Vect b(0, -1, 0);
  Vect c(1, 1, 1);
  Vect d(-100, 5000, -200);
  EXPECT_EQ(1, a.magnitude());
  EXPECT_EQ(1, b.magnitude());
  EXPECT_EQ(sqrt(3), c.magnitude());
  EXPECT_EQ(sqrt(25050000), d.magnitude());
}

TEST(VectTest, Normalise) {
  Vect a(5, 0, 0);
  Vect a_c(1, 0, 0);
  Vect b(1, 1, 1);
  Vect b_c(sqrt(1/3.0), sqrt(1/3.0), sqrt(1/3.0));
  Vect c(-1, 0, -1);
  Vect c_c(-sqrt(1/2.0), 0, -sqrt(1/2.0));
  Vect d(-1000, 2500, -1500);
  Vect d_c(-1000/sqrt(95E5), 2500/sqrt(95E5), -1500/sqrt(95E5));
  EXPECT_EQ(a_c, a.normalise());
  EXPECT_EQ(b_c, b.normalise());
  EXPECT_EQ(c_c, c.normalise());
  EXPECT_EQ(d_c, d.normalise());
}

TEST(VectTest, Negative) {
  Vect a(5, 0, 0);
  Vect a_c(-5, 0, 0);
  Vect b(1, 1, 1);
  Vect b_c(-1, -1, -1);
  Vect c(-1, 0, -1);
  Vect c_c(1, 0, 1);
  Vect d(-1000, 2500, -1500);
  Vect d_c(1000, -2500, 1500);
  EXPECT_EQ(a_c, a.negative());
  EXPECT_EQ(b_c, b.negative());
  EXPECT_EQ(c_c, c.negative());
  EXPECT_EQ(d_c, d.negative());
}

TEST(VectTest, Dot) {
  Vect a(1, 0, 0);
  Vect b(0, -1, 0);
  Vect c(1, 1, 1);
  Vect d(-100, 5000, -200);
  EXPECT_EQ(0, a.dot(b));
  EXPECT_EQ(1, a.dot(a));
  EXPECT_EQ(1, a.dot(c));
  EXPECT_EQ(-1, b.dot(c));
  EXPECT_EQ(-5000, b.dot(d));
  EXPECT_EQ(4700, c.dot(d));
}

TEST(VectTest, Cross) {
  Vect a(5, 0, 0);
  Vect b(1, 1, 1);
  Vect ab_c(0, -5, 5);
  Vect ba_c(0, 5, -5);
  Vect c(-1, 0, -1);
  Vect ac_c(0, 5, 0);
  Vect d(-1000, 2500, -1500);
  Vect cd_c(2500, -500, -2500);
  EXPECT_EQ(ab_c, a.cross(b));
  EXPECT_EQ(ba_c, b.cross(a));
  EXPECT_EQ(ac_c, a.cross(c));
  EXPECT_EQ(cd_c, c.cross(d));
}

TEST(VectTest, Add) {
  Vect a(5, 0, 0);
  Vect b(1, 1, 1);
  Vect ab_c(6, 1, 1);
  Vect ba_c(6, 1, 1);
  Vect c(-1, 0, -1);
  Vect ac_c(4, 0, -1);
  Vect d(-1000, 2500, -1500);
  Vect cd_c(-1001, 2500, -1501);
  EXPECT_EQ(ab_c, a.add(b));
  EXPECT_EQ(ba_c, b.add(a)); 
  EXPECT_EQ(ac_c, a.add(c));
  EXPECT_EQ(cd_c, c.add(d));
}

TEST(VectTest, ScalarMultiply) {
  Vect a(5, 0, 0);
  Vect a_c(1, 0, 0);
  Vect b(1, 1, 1);
  Vect b_c(-1, -1, -1);
  Vect c(-1, 0, -1);
  Vect c_c(-15, 0, -15);
  Vect d(-10, 25, -15);
  Vect d_c(1000, -2500, 1500);
  EXPECT_EQ(a_c, a.mult(0.2));
  EXPECT_EQ(b_c, b.mult(-1));
  EXPECT_EQ(c_c, c.mult(15));
  EXPECT_EQ(d_c, d.mult(-100));
}

TEST(VectTest, getVectBetweenTwoPoints) {
  Vect a(5, 0, 0);
  Vect b(1, 1, 1);
  Vect ab_c(-4, 1, 1);
  Vect ba_c(4, -1, -1);
  Vect c(-1, 0, -1);
  Vect ac_c(-6, 0, -1);
  Vect d(-1000, 2500, -1500);
  Vect cd_c(-999, 2500, -1499);
  EXPECT_EQ(ab_c, a.getVectBetweenTwoPoints(b));
  EXPECT_EQ(ba_c, b.getVectBetweenTwoPoints(a));
  EXPECT_EQ(ac_c, a.getVectBetweenTwoPoints(c));
  EXPECT_EQ(cd_c, c.getVectBetweenTwoPoints(d));
}

/*
 * ObjectTest
 */
