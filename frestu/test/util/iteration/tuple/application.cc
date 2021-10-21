#include <iostream>
#include "gtest/gtest.h"
#include "frestu/data_type/data_type.h"
#include "frestu/util/iteration/tuple/application.h"

namespace {

using Int = frestu::data_type::Int;
using Real = frestu::data_type::Real;

using std::cout;
using std::endl;
using std::get;
using std::make_index_sequence;
using std::make_tuple;
using std::tuple;
using std::tuple_size;
using frestu::util::iteration::tuple::Apply;

TEST(Application, Apply_F_T) {
  auto tpl = make_tuple("abc", 1, 1.2);
  constexpr size_t size = tuple_size<decltype(tpl)>::value;
  auto f = [](auto t){};
  // 実行できることだけ確認
  Apply(f, tpl, make_index_sequence<size>());
}

TEST(Application, R_Apply_F_T) {
  auto tpl = make_tuple(1, 1.2);
  constexpr size_t size = tuple_size<decltype(tpl)>::value;
  auto f = [](auto t){
    return t + 1;
  };
  auto rv = Apply<tuple<Int, Real>>(f, tpl, make_index_sequence<size>());
  EXPECT_EQ(get<0>(rv), 2);
  EXPECT_NEAR(get<1>(rv), 2.2, 0.1);
}

}