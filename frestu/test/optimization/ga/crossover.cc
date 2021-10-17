#include <iostream>
#include <Eigen/Dense>
#include "gtest/gtest.h"
#include "frestu/optimization/ga/crossover.h"

namespace {

using std::vector;
using Eigen::VectorXd;

TEST(Crossover, CrossoverNPoint) {
  using namespace frestu::optimization::ga::crossover;
  
  // Std Vector
  vector<double> vec_1 {1, 2, 3, 4, 5};
  vector<double> vec_2 {6, 7, 8, 9, 0};
  auto vec = CrossoverNPoint(vec_1, vec_2, 2);
  for (int i = 0; i < 5; i++) {
    // cout << vec[i] << endl;
    // vec_1 と vec_2 のどちらかに含まれているかだけ
    EXPECT_TRUE((vec[i] == vec_1[i]) || (vec[i] == vec_2[i]));
  }

  // Eigen Vector
  VectorXd vec_e_1 = VectorXd(5);
  VectorXd vec_e_2 = VectorXd(5);
  vec_e_1 << 1,2,3,4,5;
  vec_e_2 << 6,7,8,9,0;
  auto vec_e_ = CrossoverNPoint(vec_e_1, vec_e_2, 2);
  for (int i = 0; i < 5; i++) {
    // cout << vec_e_[i] << endl;
    // vec_e_1 と vec_e_2 のどちらかに含まれているかだけ
    EXPECT_TRUE((vec_e_[i] == vec_e_1[i]) || (vec_e_[i] == vec_e_2[i]));
  }
}

TEST(Crossover, CrossoverUniform) {
  using namespace frestu::optimization::ga::crossover;
  
  // Std Vector
  vector<double> vec_1 {1, 2, 3, 4, 5};
  vector<double> vec_2 {6, 7, 8, 9, 0};
  auto vec = CrossoverUniform(vec_1, vec_2);
  for (int i = 0; i < 5; i++) {
    // cout << vec[i] << endl;
    // vec_e_1 と vec_e_2 のどちらかに含まれているかだけ
    EXPECT_TRUE((vec[i] == vec_1[i]) || (vec[i] == vec_2[i]));
  }

  // Eigen Vector
  VectorXd vec_e_1 = VectorXd(5);
  VectorXd vec_e_2 = VectorXd(5);
  vec_e_1 << 1,2,3,4,5;
  vec_e_2 << 6,7,8,9,0;
  auto vec_e_ = CrossoverUniform(vec_e_1, vec_e_2);
  for (int i = 0; i < 5; i++) {
    // cout << vec_e_[i] << endl;
    // vec_e_1 と vec_e_2 のどちらかに含まれているかだけ
    EXPECT_TRUE((vec_e_[i] == vec_e_1[i]) || (vec_e_[i] == vec_e_2[i]));
  }
}

}