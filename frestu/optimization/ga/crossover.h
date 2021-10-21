#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include "frestu/data_type/data_type.h"
#include "frestu/util/sampling.h"

namespace frestu::optimization::ga::crossover {

using std::min;
using std::runtime_error;
using std::to_string;
using std::vector;
using frestu::data_type::Int;
using frestu::util::sampling::Sample;
using frestu::util::sampling::SampleWithoutReplacement;

/**
 * @fn Vec CrossoverNPoint(const Vec& vec_1, const Vec& vec_2, const Int n_point)
 * @brief n 点交叉。
 * @param[in] vec_1 size() が定義されている必要がある。コピーコンストラクタが定義されている必要がある。
 * @param[in] vec_2 size() が定義されている必要がある。
 * @return Vec 交叉してできるベクトル。
 */
template <typename Vec>
Vec CrossoverNPoint(const Vec& vec_1, const Vec& vec_2, const Int n_point) {
  // NOTE: size_t でなく Int を使っている。
  const Int size = min(vec_1.size(), vec_2.size());
  const Int max_n = size - 1;

  if (max_n < n_point) {
    throw runtime_error("n_point should be " + to_string(max_n) + " or less.");
  }

  vector<Int> ixs;
  for (Int i = 0; i < max_n; i++) {
    ixs.push_back(i);
  }
  // 非復元抽出
  auto separating_positions = SampleWithoutReplacement<>(ixs, n_point);
  sort(separating_positions.begin(), separating_positions.end());

  // どの分割区間の要素を扱っているか
  Int separation_number = 0;
  // vec_1 と vec_2 の順序の区別を無くす
  vector<bool> b {true, false};
  // vec_1 を選択しているか vec_2 を選択しているか
  bool is_switching = SampleWithoutReplacement<>(b, 1)[0];
  // 戻り値。コピーコンストラクタが定義されている型である必要
  Vec vec(vec_1);
  for (Int i = 0; i < size; i++) {
    if (is_switching) {
      vec[i] = vec_1[i];
    } else {
      vec[i] = vec_2[i];
    }
    if (separating_positions[separation_number] == i) {
      is_switching = !is_switching;
      separation_number++;
    }
  }

  return vec;
}

/**
 * @fn Vec CrossoverUniform(const Vec& vec_1, const Vec& vec_2)
 * @brief 一様交叉。
 * @param[in] vec_1 size() が定義されている必要がある。
 *                  コピーコンストラクタが定義されている必要がある。
 * @param[in] vec_2 size() が定義されている必要がある。
 * @return Vec 交叉してできるベクトル。
 */
template <typename Vec>
Vec CrossoverUniform(const Vec& vec_1, const Vec& vec_2) {
  // NOTE: size_t でなく Int を使っている。
  const Int size = min(vec_1.size(), vec_2.size());

  vector<bool> b {true, false};
  // 復元抽出。vec_1 を選択しているか vec_2 を選択しているか
  auto is_switching = Sample<>(b, size, true);

  // 戻り値。コピーコンストラクタが定義されている型である必要
  Vec vec(vec_1);
  for (Int i = 0; i < size; i++) {
    if (is_switching[i]) {
      vec[i] = vec_2[i];
    }
  }

  return vec;
}

}