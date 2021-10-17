#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/util/generation/number/random/logscale.h"

namespace {

using std::cout;
using std::endl;
using std::vector;

TEST(Logscale, GenerateUniform_double_double) {
  using namespace frestu::util::generation::number::random::logscale;

  double r = GenerateUniform(0.1, 100.0);

  EXPECT_GT(r, 0.1);
  EXPECT_LT(r, 100.0);
}

TEST(Linear, GenerateUniform_double_double_double) {
  using namespace frestu::util::generation::number::random::logscale;

  vector<double> rvec = GenerateUniform<vector<double>>(0.1, 100.0, 10);

  EXPECT_EQ(rvec.size(), 10);
  for (const auto& c : rvec) {
    // cout << c << endl;
    EXPECT_GT(c, 0.1);
    EXPECT_LT(c, 100.0);
  }
}

}