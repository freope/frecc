#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/data_type/data_type.h"
#include "frestu/util/io/table_reader.h"

namespace {

using Int = frestu::data_type::Int;
using Real = frestu::data_type::Real;

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::tuple;
using std::vector;
using frestu::util::io::TableReader;

TEST(TableReader, n_rows) {
  const string path_1 = "./data/ts.dat";
  auto tr_1 = TableReader(path_1);
  EXPECT_EQ(tr_1.n_rows(), 11);

  const string path_2 = "./data/ts_with_header.dat";
  auto tr_2 = TableReader(path_2, true);
  EXPECT_EQ(tr_2.n_rows(), 11);
}

TEST(TableReader, ReadColumn) {
  const string path = "./data/ts.dat";
  auto tr = TableReader(path);

  auto times = tr.ReadColumn(0);
  auto values_i = tr.ReadColumn<Int>(1, [](auto v){return stoi(v);});
  auto values_d = tr.ReadColumn<Real>(2, [](auto v){return stod(v);});

  // for (const auto& c : times) {
  //   std::cout << c << std::endl;
  // }
  // for (const auto& c : values_i) {
  //   std::cout << c << std::endl;
  // }
  // for (const auto& c : values_d) {
  //   std::cout << c << std::endl;
  // }

  EXPECT_EQ(times[0], "2020-01-01 17:00:00");
  EXPECT_EQ(times[10], "2020-01-01 17:10:00");
  EXPECT_EQ(values_i[0], 0);
  EXPECT_EQ(values_i[10], 70);
  EXPECT_NEAR(values_d[0], 0.1, 0.1);
  EXPECT_NEAR(values_d[10], 70.1, 0.1);
}

TEST(TableReader, ReadColumns) {
  const string path = "./data/ts.dat";
  auto tr = TableReader(path);

  vector<Int> ixs {1, 2};
  auto values = tr.ReadColumns<Real>(ixs, [](auto v){return stod(v);});

  // for (const auto& c : values_d) {
  //   cout << c << endl;
  // }

  EXPECT_NEAR(values[0], 0.0, 0.1);
  EXPECT_NEAR(values[10], 70, 0.1);
  EXPECT_NEAR(values[11], 0.1, 0.1);
  EXPECT_NEAR(values[21], 70.1, 0.1);
}

}