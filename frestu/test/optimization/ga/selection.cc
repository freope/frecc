#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/data_type/data_type.h"
#include "frestu/optimization/ga/selection.h"

namespace {

using std::cout;
using std::endl;
using std::vector;
using frestu::data_type::Int;
using frestu::data_type::Real;

TEST(Selection, SelectRoulette) {
  using namespace frestu::optimization::ga::selection;

  vector<Real> vec {1, 2, 3, 4, 5};
  Int ix_selected = SelectRoulette<vector<Real>>(vec);

  EXPECT_TRUE(
    (ix_selected == 0)
      || (ix_selected == 1)
      || (ix_selected == 2)
      || (ix_selected == 3)
  );
  EXPECT_FALSE(ix_selected == 4);
}

TEST(Selection, SelectRanking) {
  using namespace frestu::optimization::ga::selection;

  vector<Real> vec {1, 2, 3, 4, 5};
  Int ix_selected = SelectRanking<vector<Real>>(vec);

  // cout << ix_selected << endl;

  EXPECT_TRUE(
    (ix_selected == 0)
      || (ix_selected == 1)
      || (ix_selected == 2)
      || (ix_selected == 3)
      || (ix_selected == 4)
  );
  EXPECT_FALSE(ix_selected == 5);
}

}