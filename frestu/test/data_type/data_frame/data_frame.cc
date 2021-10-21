#include <iostream>
#include <string>
#include <tuple>
#include <Eigen/Dense>
#include "gtest/gtest.h"
#include "frestu/data_type/data_frame/data_frame.h"
#include "frestu/util/io/table_reader.h"

namespace {

using std::cout;
using std::endl;
using std::get;
using std::make_tuple;
using std::string;
using std::tuple_size;
using std::vector;
using Eigen::Dynamic;
using Eigen::Map;
using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXi;
using Eigen::VectorXd;
using frestu::data_type::Int;
using frestu::data_type::Real;
using frestu::data_type::data_frame::DataFrame;
using frestu::util::io::TableReader;

auto tr = TableReader("./data/ts.dat");
// time
auto indices = tr.ReadColumn<string>(0, [](auto v) { return v; });
auto value_i = tr.ReadColumn<Int>(1, [](auto v) { return stoi(v); });

auto ixs_d = vector<int>{1, 2};
auto value_d = tr.ReadColumns<Real>(ixs_d, [](auto v) { return stod(v); });

VectorXi vec_i = Map<VectorXi>(&value_i[0], value_i.size());
MatrixXd mat_d = Map<MatrixXd>(&value_d[0], tr.n_rows(), ixs_d.size());

auto df_columns = make_tuple(vec_i, mat_d);
auto df = DataFrame(df_columns);

TEST(DataFrame, Apply_Fn) {
  auto sum_df = df.Apply<Real>([](auto& b) { return b.sum(); });

  // vec_i
  EXPECT_NEAR(sum_df[0], 275, 0.01);
  // mat_d
  EXPECT_NEAR(sum_df[1], 551.1, 0.01);
}

TEST(DataFrame, Apply_Fn_Int_Int) {
  auto sum_1 = df.Apply<Real>([](auto& b) { return b.sum(); }, 0, 0);
  auto sum_2 = df.Apply<Real>([](auto& b) { return b.sum(); }, 0, 1);
  auto sum_3 = df.Apply<Real>([](auto& b) { return b.sum(); }, 0, 5);
  auto sum_4 = df.Apply<Real>([](auto& b) { return b.sum(); }, 3, 6);

  // vec_i
  EXPECT_NEAR(sum_1[0], 0.0, 0.01);
  EXPECT_NEAR(sum_2[0], 0.0, 0.01);
  EXPECT_NEAR(sum_3[0], 85, 0.01);
  EXPECT_NEAR(sum_4[0], 70, 0.01);

  // mat_d
  EXPECT_NEAR(sum_1[1], 0.0, 0.01);
  EXPECT_NEAR(sum_2[1], 0.1, 0.01);
  EXPECT_NEAR(sum_3[1], 170.5, 0.01);
  EXPECT_NEAR(sum_4[1], 140.3, 0.01);
}

TEST(DataFrame, Roll) {
  auto roll_1 = df.Roll<Real>([](auto& b) { return b.mean(); }, 2);
  auto roll_2 = df.Roll<Real>([](auto& b) { return b.mean(); }, 3);

  // vec_i
  EXPECT_NEAR(roll_1(0, 0), 9999, 0.01);
  EXPECT_NEAR(roll_1(1, 0), 5, 0.01);
  EXPECT_NEAR(roll_1(10, 0), 65, 0.01);

  // mat_d
  EXPECT_NEAR(roll_1(0, 1), 9999, 0.01);
  EXPECT_NEAR(roll_1(1, 1), 5.05, 0.01);
  EXPECT_NEAR(roll_1(10, 1), 65.05, 0.01);

  // vec_i
  EXPECT_NEAR(roll_2(0, 0), 9999, 0.01);
  EXPECT_NEAR(roll_2(1, 0), 9999, 0.01);
  EXPECT_NEAR(roll_2(2, 0), 10, 0.01);
  EXPECT_NEAR(roll_2(10, 0), 53, 0.01);

  // mat_d
  EXPECT_NEAR(roll_2(0, 1), 9999, 0.01);
  EXPECT_NEAR(roll_2(1, 1), 9999, 0.01);
  EXPECT_NEAR(roll_2(2, 1), 10.05, 0.01);
  EXPECT_NEAR(roll_2(10, 1), 53.38, 0.01);
}

TEST(DataFrame, Recurse) {
  // 累積和
  const Int window = 1;
  const Int window_recursive = 1;
  const size_t size_df_columns = 2; //tuple_size<columns>::value;

  Matrix<Real, window_recursive, size_df_columns> initial_values;
  initial_values(0, 0) = 0.0;
  initial_values(0, 1) = 0.0;

  auto recursion = df.RollRecursion<Real>([](auto& b, auto& r) {
    return b.sum() + r.sum();
  }, window, window_recursive, initial_values);

  // vec_i
  EXPECT_EQ(recursion(0, 0), 0);
  EXPECT_EQ(recursion(1, 0), 10);
  EXPECT_EQ(recursion(10, 0), 275);

  // mat_d
  EXPECT_NEAR(recursion(0, 1), 0.1, 0.1);
  EXPECT_NEAR(recursion(1, 1), 20.2, 0.1);
  EXPECT_NEAR(recursion(10, 1), 551.1, 0.1);
}

}