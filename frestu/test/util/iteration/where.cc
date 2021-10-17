#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "gtest/gtest.h"
#include "frestu/util/iteration/where.h"

namespace {

using std::vector;
using Eigen::VectorXi;

TEST(Where, Where_T_U_T_T) {
  using namespace frestu::util::iteration;

  vector<bool> booleans {true, false, false, true, true};
  int value_true = 10;

  // Std Vector
  vector<int> vec_false {1, 2, 3, 4, 5};
  auto vec_selected = Where<vector<int>, int>(booleans, value_true, vec_false);
  EXPECT_EQ(vec_selected[0], 10);
  EXPECT_EQ(vec_selected[1], 2);
  EXPECT_EQ(vec_selected[2], 3);
  EXPECT_EQ(vec_selected[3], 10);
  EXPECT_EQ(vec_selected[4], 10);

  // Eigen Vector
  auto vece_false = VectorXi(5);
  vece_false << 1,2,3,4,5;
  auto vece_selected = Where<VectorXi, int>(booleans, value_true, vece_false);
  EXPECT_EQ(vece_selected[0], 10);
  EXPECT_EQ(vece_selected[1], 2);
  EXPECT_EQ(vece_selected[2], 3);
  EXPECT_EQ(vece_selected[3], 10);
  EXPECT_EQ(vece_selected[4], 10);
}

TEST(Where, Where_T_T_U_T) {
  using namespace frestu::util::iteration;

  vector<bool> booleans {true, false, false, true, true};
  int value_false = 10;

  // Std Vector
  vector<int> vec_true {1, 2, 3, 4, 5};
  auto vec_selected = Where<vector<int>, int>(booleans, vec_true, value_false);
  EXPECT_EQ(vec_selected[0], 1);
  EXPECT_EQ(vec_selected[1], 10);
  EXPECT_EQ(vec_selected[2], 10);
  EXPECT_EQ(vec_selected[3], 4);
  EXPECT_EQ(vec_selected[4], 5);

  // Eigen Vector
  auto vece_true = VectorXi(5);
  vece_true << 1,2,3,4,5;
  auto vece_selected = Where<VectorXi, int>(booleans, vece_true, value_false);
  EXPECT_EQ(vece_selected[0], 1);
  EXPECT_EQ(vece_selected[1], 10);
  EXPECT_EQ(vece_selected[2], 10);
  EXPECT_EQ(vece_selected[3], 4);
  EXPECT_EQ(vece_selected[4], 5);
}

TEST(Where, Where_T_T_T_T) {
  using namespace frestu::util::iteration;

  vector<bool> booleans {true, false, false, true, true};

  // Std Vector
  vector<int> vec_true {1, 2, 3, 4, 5};
  vector<int> vec_false {10, 20, 30, 40, 50};
  auto vec_selected = Where<vector<int>>(booleans, vec_true, vec_false);
  EXPECT_EQ(vec_selected[0], 1);
  EXPECT_EQ(vec_selected[1], 20);
  EXPECT_EQ(vec_selected[2], 30);
  EXPECT_EQ(vec_selected[3], 4);
  EXPECT_EQ(vec_selected[4], 5);

  // Eigen Vector
  auto vece_true = VectorXi(5);
  auto vece_false = VectorXi(5);
  vece_true << 1,2,3,4,5;
  vece_false << 10,20,30,40,50;
  auto vece_selected = Where<VectorXi>(booleans, vece_true, vece_false);
  EXPECT_EQ(vece_selected[0], 1);
  EXPECT_EQ(vece_selected[1], 20);
  EXPECT_EQ(vece_selected[2], 30);
  EXPECT_EQ(vece_selected[3], 4);
  EXPECT_EQ(vece_selected[4], 5);
}

}