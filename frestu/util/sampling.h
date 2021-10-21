#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include "frestu/data_type/data_type.h"
#include "frestu/util/generation/number/random/linear.h"

namespace frestu::util::sampling {

using Int = frestu::data_type::Int;

using std::mt19937;
using std::random_device;
using std::runtime_error;
using frestu::util::generation::number::random::linear::GenerateUniform;

// 非決定的な乱数生成器
random_device randomDevise;
// メルセンヌ・ツイスタの32ビット版、引数は初期シード値
mt19937 randomEngineMt(randomDevise());

/**
 * @fn Vec SampleWithoutReplacement(Vec samples, const size_t k)
 * @brief 非復元抽出。
 * @param[in] samples 抽出対象となるアイテムの集合。
 *                    size() が定義されている必要がある。
 *                    引数が要素数だけのコンストラクタが定義されている必要がある。
 * @param[in] k 抽出するアイテムの数。
 * @return Vec 抽出されたアイテムの集合。
 */
template <typename Vec>
Vec SampleWithoutReplacement(Vec samples, const size_t k) {
  Vec samples_selected(k);
  const size_t size = samples.size();

  if (size < k) {
    throw runtime_error("k is too large.");
  }

  shuffle(samples.begin(), samples.end(), randomEngineMt);

  for (size_t i = 0; i < k; i++) {
    samples_selected[i] = samples[i];
  }

  return samples_selected;
}

/**
 * @fn Vec SampleWithReplacement(const Vec& samples, const Int k)
 * @brief 復元抽出。
 * @param[in] samples 抽出対象となるアイテムの集合。
 * @param[in] k 抽出するアイテムの数。
 * @return Vec 抽出されたアイテムの集合。
 */
template <typename Vec>
Vec SampleWithReplacement(const Vec& samples, const Int k) {
  Vec samples_selected(k);
  // NOTE: size_t でなく Int を使っている。
  const Int size = samples.size();

  for (Int i = 0; i < k; i++) {
    samples_selected[i] = samples[GenerateUniform(0, size-1)];
  }

  return samples_selected;
}

template <typename Vec>
Vec Sample(const Vec& samples, const Int k, const bool replacement) {
  if (replacement) {
    // 復元抽出
    return SampleWithReplacement<Vec>(samples, k);
  }
  // 非復元抽出
  return SampleWithoutReplacement<Vec>(samples, k);
}

}