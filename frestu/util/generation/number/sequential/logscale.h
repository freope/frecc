#pragma once

#include <cmath>
#include "frestu/data_type/data_type.h"

namespace frestu::util::generation::number::sequential::logscale {

using frestu::data_type::Int;
using frestu::data_type::Real;

/**
 * @fn Vec GenerateStep(const Real minimum, const Real maximum, const Real log_step=1)
 * @brief ログスケール上で一様な数を生成する。ステップ幅で指定。
 * @param[in] minimum 最小値。ログスケール上ではない。
 * @param[in] maximum 最大値。ログスケール上ではない。
 * @param[in] log_step ログスケールでのステップ幅。
 * @return Vec 生成された数の集合。
 */
template <typename Vec>
Vec GenerateStep(
    const Real minimum, const Real maximum, const Real log_step=1) {
  const Real log_minimum = log10(minimum);
  const Real log_maximum = log10(maximum);
  const Int size = (log_maximum - log_minimum) / log_step + 1;
  Vec vec(size);

  Int ix = 0;
  Real log_value = log_minimum;
  while (log_value <= log_maximum) {
    vec[ix] = pow(10, log_value);
    log_value += log_step;
    ix++;
  }

  return vec;
}

/**
 * @fn Vec GenerateQuantity(const Real minimum, const Real maximum, const Int n)
 * @brief ログスケール上で一様な数を生成する。個数で指定。
 * @param[in] minimum 最小値。ログスケール上ではない。
 * @param[in] maximum 最大値。ログスケール上ではない。
 * @param[in] n 生成する数の個数。
 * @return Vec 生成された数の集合。
 */
template <typename Vec>
Vec GenerateQuantity(const Real minimum, const Real maximum, const Int n) {
  Vec vec(n);

  const Real log_minimum = log10(minimum);
  const Real log_maximum = log10(maximum);

  const Real n_split = n - 1;
  const Real log_step = (log_maximum - log_minimum) / n_split;
  for (Int i = 0; i < n; i++) {
    vec[i] = pow(10, log_minimum + (Real)i * log_step);
  }

  return vec;
}

}