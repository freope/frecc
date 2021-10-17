#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/util/generation/number/random/linear.h"

namespace {

using std::cout;
using std::endl;
using std::vector;

TEST(Linear, GenerateUniform_int_int){
  using namespace frestu::util::generation::number::random::linear;
  
  int r = GenerateUniform(0, 1);
  // cout << r << endl;
  EXPECT_TRUE((r == 0) || (r == 1));
}

TEST(Linear, GenerateUniform_double_double) {
  using namespace frestu::util::generation::number::random::linear;
  
  double r = GenerateUniform(0.0, 1.0);
  // cout << r << endl;
  EXPECT_GT(r, 0.0);
  EXPECT_LT(r, 1.0);
}

TEST(Linear, GenerateUniform_int_int_int) {
  using namespace frestu::util::generation::number::random::linear;

  vector<int> rvec = GenerateUniform<vector<int>>(0, 2, 10);

  EXPECT_EQ(rvec.size(), 10);
  for (const auto& c : rvec) {
    // cout << c << endl;
    EXPECT_TRUE((c == 0) || (c == 1) || (c == 2));
  }
}

TEST(Linear, GenerateUniform_double_double_double) {
  using namespace frestu::util::generation::number::random::linear;

  vector<double> rvec = GenerateUniform<vector<double>>(3.5, 6.0, 10);

  EXPECT_EQ(rvec.size(), 10);
  for (const auto& c : rvec) {
    // cout << c << endl;
    EXPECT_GT(c, 3.5);
    EXPECT_LT(c, 6.0);
  }
}

}