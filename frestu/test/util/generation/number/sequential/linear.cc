#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/util/generation/number/sequential/linear.h"

namespace {

using std::cout;
using std::endl;
using std::vector;

TEST(Generation, GenerateStep_int_int_int) {
  using namespace frestu::util::generation::number::sequential::linear;

  vector<int> vec = GenerateStep<vector<int>>(2, 10, 3);

  // for (const auto& c : vec) {
  //     cout << c << endl;
  // }
  EXPECT_EQ(vec[0], 2);
  EXPECT_EQ(vec[1], 5);
  EXPECT_EQ(vec[2], 8);
}

TEST(Generation, GenerateStep_double_double_double) {
  using namespace frestu::util::generation::number::sequential::linear;

  vector<double> vec = GenerateStep<vector<double>>(2.0, 10.0, 3.2);

  // for (const auto& c : vec) {
  //     cout << c << endl;
  // }
  EXPECT_NEAR(vec[0], 2.0, 0.1);
  EXPECT_NEAR(vec[1], 5.2, 0.1);
  EXPECT_NEAR(vec[2], 8.4, 0.1);
}

TEST(Generation, GenerateQuantity_double_double_int) {
  using namespace frestu::util::generation::number::sequential::linear;

  vector<double> vec = GenerateQuantity<vector<double>>(2.0, 10.0, 10);

  EXPECT_NEAR(vec[0], 2.0, 0.1);
  EXPECT_NEAR(vec[4], 5.5, 0.1);
  EXPECT_NEAR(vec[9], 10.0, 0.1);
}

}