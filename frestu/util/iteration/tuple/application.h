#pragma once

#include <iostream>
#include <tuple>

namespace frestu::util::iteration::tuple {

using std::size_t;
using std::index_sequence;
using std::get;
using std::make_tuple;

/**
 * @fn void Apply(const Fn& func, const Tup& tup, std::index_sequence<ixs...>)
 * @brief タプル tup の全要素に関数 func を適用する。
 * @param[in] func タプルの要素を引数にとる関数。
 * @param[in] tup タプル。
 * @return void
 */
template <typename Fn, typename Tup, size_t... ixs>
void Apply(const Fn& func, const Tup& tup, index_sequence<ixs...>) {
  using swallow = int[];
  (void)swallow {
    (func(get<ixs>(tup)), 0)... 
  };
};

/**
 * @fn Ret Apply(const Fn& func, const Tup& tup, index_sequence<ixs...>)
 * @brief タプル tup の全要素に関数 func を適用し、その結果を得る。
 * @param[in] func タプルの要素を引数にとる関数。
 * @param[in] tup タプル。
 * @return Ret タプル tup の各要素に関数 func を適用させた値を要素に持つタプル。
 */
template <typename Ret, typename Fn, typename Tup, size_t... ixs>
Ret Apply(const Fn& func, const Tup& tup, index_sequence<ixs...>) {
  Ret ret;
  using swallow = int[];
  (void)swallow {
    (ret = make_tuple(func(get<ixs>(tup))...), 0)
  };
  return ret;
};

}
