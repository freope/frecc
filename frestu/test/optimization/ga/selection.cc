#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/optimization/ga/selection.h"

namespace {

using std::cout;
using std::endl;
using std::vector;

TEST(Selection, SelectRoulette) {
  using namespace frestu::optimization::ga::selection;

  vector<double> vec {1, 2, 3, 4, 5};
  int ix_selected = SelectRoulette<vector<double>>(vec);

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

  vector<double> vec {1, 2, 3, 4, 5};
  int ix_selected = SelectRanking<vector<double>>(vec);

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