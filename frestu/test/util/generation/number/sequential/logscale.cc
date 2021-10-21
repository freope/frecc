#include <cmath>
#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/data_type/data_type.h"
#include "frestu/util/generation/number/sequential/logscale.h"

namespace {

using Int = frestu::data_type::Int;

using std::cout;
using std::endl;
using std::vector;

TEST(Generation, GenerationStep_Int_Int_Int) {
  using namespace frestu::util::generation::number::sequential::logscale;

  vector<Real> vec = GenerateStep<vector<Real>>(0.1, 100.0, 0.5);

  // for (const auto& c : vec) {
  //     cout << c << endl;
  // }
  EXPECT_NEAR(vec[0], 0.1, 0.1);
  EXPECT_NEAR(vec[2], 1.0, 0.1);
  EXPECT_NEAR(vec[4], 10.0, 0.1);
  EXPECT_NEAR(vec[6], 100.0, 0.1);
}

TEST(Generation, GenerationQuantity_Real_Real_Int) {
  using namespace frestu::util::generation::number::sequential::logscale;

  vector<Real> vec = GenerateQuantity<vector<Real>>(
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