#include <algorithm>
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "gtest/gtest.h"
#include "frestu/data_type/data_type.h"
#include "frestu/util/sampling.h"

namespace {

using Int = frestu::data_type::Int;

using std::find;
using std::vector;
using Eigen::VectorXi;

TEST(Sampling, SampleWithoutReplacement) {
  using namespace frestu::util::sampling;

  // Std Vector
  vector<Int> samples {1, 2, 3};
  vector<Int> selecteds = SampleWithoutReplacement<vector<Int>>(samples, 2);
  EXPECT_EQ(selecteds.size(), 2);
  // 選択された要素が 1, 2, 3 のどれか
  for (const auto& sample : selecteds) {
    // cout << sample << endl;
    EXPECT_TRUE(
      (*find(selecteds.begin(), selecteds.end(), sample) == 1)
        || (*find(selecteds.begin(), selecteds.end(), sample) == 2)
        || (*find(selecteds.begin(), selecteds.end(), sample) == 3)
    );
  }

  // Eigen Vector
  VectorXi samples_e = VectorXi(3);
  samples_e << 1,2,3;
  VectorXi selecteds_e = SampleWithoutReplacement<VectorXi>(samples_e, 2);    
  EXPECT_EQ(selecteds_e.size(), 2);
  // 選択された要素が 1, 2, 3 のどれか
  for (const auto& sample : selecteds_e) {
    // cout << sample << endl;
    EXPECT_TRUE(
      (*find(selecteds_e.begin(), selecteds_e.end(), sample) == 1)
        || (*find(selecteds_e.begin(), selecteds_e.end(), sample) == 2)
        || (*find(selecteds_e.begin(), selecteds_e.end(), sample) == 3)
    );
  }
}

TEST(Sampling, SampleWithReplacement) {
  using namespace frestu::util::sampling;

  // Std Vector
  vector<Int> samples {1, 2, 3};
  vector<Int> selecteds = SampleWithReplacement<vector<Int>>(samples, 5);
  EXPECT_EQ(selecteds.size(), 5);
  // 選択された要素が 1, 2, 3 のどれか
  for (const auto& sample : selecteds) {
    // cout << sample << endl;
    EXPECT_TRUE(
      (*find(selecteds.begin(), selecteds.end(), sample) == 1)
        || (*find(selecteds.begin(), selecteds.end(), sample) == 2)
        || (*find(selecteds.begin(), selecteds.end(), sample) == 3)
    );
  }

  // Eigen Vector
  VectorXi samples_e = VectorXi(3);
  samples_e << 1,2,3;
  VectorXi selecteds_e = SampleWithReplacement<VectorXi>(samples_e, 5);
  EXPECT_EQ(selecteds_e.size(), 5);
  // 選択された要素が 1, 2, 3 のどれか
  for (const auto& sample : selecteds_e) {
    // cout << sample << endl;
    EXPECT_TRUE(
      (*find(selecteds_e.begin(), selecteds_e.end(), sample) == 1)
        || (*find(selecteds_e.begin(), selecteds_e.end(), sample) == 2)
        || (*find(selecteds_e.begin(), selecteds_e.end(), sample) == 3)
    );
  }
}

}