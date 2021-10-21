#pragma once

#include "frestu/data_type/data_type.h"

namespace frestu::util::generation::number::sequential::linear {

using frestu::data_type::Int;
using frestu::data_type::Real;

/**
 * @fn Vec GenerateStep(const Int minimum, const Int maximum, const Int step=1)
 * @brief 一様な数を生成する。ステップ幅で指定。
 * @param[in] minimum 最小値。
 * @param[in] maximum 最大値。
 * @param[in] step ステップ幅。
 * @return Vec 生成された数の集合。
 */
template <typename Vec>
Vec GenerateStep(const Int minimum, const Int maximum, const Int step=1) {
  const Int size = (maximum - minimum) / step + 1;
  Vec vec(size);

  Int ix = 0;
  Int value = minimum;
  while (value <= maximum) {
    vec[ix] = value;
    value += step;
    ix++;
  }

  return vec;
}

template <typename Vec>
Vec GenerateStep(
    const Real minimum, const Real maximum, const Real step=1) {
  const Int size = (maximum - minimum) / step + 1;
  Vec vec(size);

  Int ix = 0;
  Real value = minimum;
  while (value <= maximum) {
    vec[ix] = value;
    value += step;
    ix++;
  }

  return vec;
}

template <typename Vec>
Vec GenerateQuantity(
    const Real minimum, const Real maximum, const Int n) {
  Vec vec(n);

  const Real n_split = n - 1;
  const Real step = (maximum - minimum) / n_split;
  for (Int i = 0; i < n; i++) {
    vec[i] = minimum + (Real)i * step;
  }

  return vec;
}

}