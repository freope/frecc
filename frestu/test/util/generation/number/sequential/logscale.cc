#include <cmath>
#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/util/generation/number/sequential/logscale.h"

namespace {

using std::cout;
using std::endl;
using std::vector;

TEST(Generation, GenerationStep_int_int_int) {
  using namespace frestu::util::generation::number::sequential::logscale;

  vector<double> vec = GenerateStep<vector<double>>(0.1, 100.0, 0.5);

  // for (const auto& c : vec) {
  //     cout << c << endl;
  // }
  EXPECT_NEAR(vec[0], 0.1, 0.1);
  EXPECT_NEAR(vec[2], 1.0, 0.1);
  EXPECT_NEAR(vec[4], 10.0, 0.1);
  EXPECT_NEAR(vec[6], 100.0, 0.1);
}

TEST(Generation, GenerationQuantity_double_double_int) {
  using namespace frestu::util::generation::number::sequential::logscale;

  vector<double> vec = GenerateQuantity<vector<double>>(
      0.1, 100.0, 7);

  // for (const auto& c : vec) {
  //     cout << c << endl;
  // }
  EXPECT_NEAR(vec[0], 0.1, 0.1);
  EXPECT_NEAR(vec[2], 1.0, 0.1);
  EXPECT_NEAR(vec[4], 10.0, 0.1);
  EXPECT_NEAR(vec[6], 100.0, 0.1);
}

}