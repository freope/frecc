#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/data_type/data_type.h"
#include "frestu/util/generation/number/random/logscale.h"

namespace {

using Real = frestu::data_type::Real;

using std::cout;
using std::endl;
using std::vector;

TEST(Logscale, GenerateUniform_Real_Real) {
  using namespace frestu::util::generation::number::random::logscale;

  Real r = GenerateUniform(0.1, 100.0);

  EXPECT_GT(r, 0.1);
  EXPECT_LT(r, 100.0);
}

TEST(Linear, GenerateUniform_Real_Real_Real) {
  using namespace frestu::util::generation::number::random::logscale;

  vector<Real> rvec = GenerateUniform<vector<Real>>(0.1, 100.0, 10);

  EXPECT_EQ(rvec.size(), 10);
  for (const auto& c : rvec) {
    // cout << c << endl;
    EXPECT_GT(c, 0.1);
    EXPECT_LT(c, 100.0);
  }
}

}