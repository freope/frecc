#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>
#include "frestu/util/generation/number/random/linear.h"

namespace frestu::optimization::ga::selection {

using Int = int;
using Real = double;

using std::runtime_error;
using frestu::util::generation::number::random::linear::GenerateUniform;

/**
 * @fn Int SelectRoulette(Vec items
 * @brief ルーレット選択を行う。
 * @param[in] items begin(), end(), cbegin(), cend() が定義されている必要がある。値渡し。成分が書き換えられる。
 * @return Int items で与えられたリストをソートした後の、選択された要素のインデックス。
 * @details 全て同じ値だったら計算できないという問題がある。また、最悪の個体は選択されない。
 */
template <typename Vec>
Int SelectRoulette(Vec items) {
  auto& items_scaled = items;

  const auto minimum = \
    *(min_element(items_scaled.cbegin(), items_scaled.cend()));
  for (auto& item : items_scaled) {
    item -= minimum;        
  }

  const auto sum_scaled = \
     accumulate(items_scaled.begin(), items_scaled.end(), 0);
  if (sum_scaled == 0) {
    throw runtime_error("sum_scaled should not be zero.");
  }

  for (auto& item : items_scaled) {
    item /= sum_scaled;
  }

  // 降順に並び替え
  sort(items_scaled.begin(), items_scaled.end(),
       [](auto lhs, auto rhs) ->bool { return rhs < lhs; });

  Int ix_selected = 0;
  Real r = GenerateUniform(0.0, 1.0);
  for (const auto& item : items_scaled) {
    r -= item;
    if (r < 0) {
      return ix_selected;
    }
    ix_selected += 1;
  }
  // ここに到達することはないが、warning を避けるため。
  return ix_selected;
}

/**
 * @fn Int SelectRanking(const Vec& items)
 * @brief ランキング選択を行う。
 * @param[in] items size() が定義されている必要がある。
 * @return Int items で与えられたリストをソートした後の、選択された要素のインデックス。
 */
template <typename Vec>
Int SelectRanking(const Vec& items) {
  const size_t size = items.size();

  // 最悪の個体も選ばれるように
  Real items_scaled[size];
  for (size_t i = 0; i < size; i++) {
    items_scaled[i] = size - i;
  }
  const Real sum_scaled = (Real)(size * (size + 1)) / 2;

  for (auto& item : items_scaled) {
    item /= sum_scaled;
  }

  Int ix_selected = 0;
  Real r = GenerateUniform(0.0, 1.0);
  for (const auto& item : items_scaled) {
    r -= item;
    if (r < 0) {
      return ix_selected;
    }
    ix_selected += 1;
  }
  // ここに到達することはないが、warning を避けるため。
  return ix_selected;
}

}