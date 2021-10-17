#pragma once

#include <functional>
#include "frestu/optimization/ga/gene/gene_abstract.h"
#include "frestu/util/generation/number/random/linear.h"
#include "frestu/util/sampling.h"

namespace frestu::optimization::ga::gene {

using Int = int;
using Real = double;

using std::function;
using std::mt19937;
using std::random_device;
using std::vector;
using frestu::optimization::ga::gene::GeneAbstract;
using frestu::util::generation::number::random::linear::GenerateUniform;
using frestu::util::sampling::Sample;

// 非決定的な乱数生成器
random_device randomDevise;
// メルセンヌ・ツイスタの32ビット版、引数は初期シード値
mt19937 randomEngineMt(randomDevise());

template <typename GeneValues>
class GeneDiscrete : public GeneAbstract<GeneValues> {
  const GeneValues candidates_;
  const function<GeneValues(GeneValues, GeneValues)> crossover_;
  const bool replacement_;

  void MutateWithReplacement(void) const {
    const auto probs = GenerateUniform<vector<Real>>(
      0.0, 1.0, this->dimension_);
    const auto values_mutated = Sample<GeneValues>(
      candidates_, this->dimension_, replacement_);

    for (Int ix = 0; ix < this->dimension_; ix++) {
      // cout << probs[ix] << endl;
      if (probs[ix] < this->mutate_probability_) {
        this->values_[ix] = values_mutated[ix];
      }
    }
  };
  
  void MutateWithoutReplacement(void) const {
    const auto probs = GenerateUniform<vector<Real>>(
      0.0, 1.0, this->dimension_);
    vector<Int> ixs_mutated;

    for (Int ix = 0; ix < this->dimension_; ix++) {
      // cout << probs[ix] << endl;
      if (probs[ix] < this->mutate_probability_) {
        ixs_mutated.push_back(ix);
      }
    }

    const size_t size = ixs_mutated.size();
    GeneValues values_mutated(size);

    for (size_t ix = 0; ix < size; ix++) {
      values_mutated[ix] = this->values_[ixs_mutated[ix]];
    }

    shuffle(values_mutated.begin(), values_mutated.end(), randomEngineMt);

    for (size_t ix = 0; ix < size; ix++) {
      this->values_[ixs_mutated[ix]] = values_mutated[ix];
    }
  };
  
public:
  // デフォルトコンストラクタを定義しない。
  GeneDiscrete(
      GeneValues& candidates,
      const function<GeneValues(GeneValues, GeneValues)>& crossover,
      bool replacement,
      Int dimension,
      Real mutate_probability)
      : GeneAbstract<GeneValues>(dimension, mutate_probability),
        candidates_(candidates),
        crossover_(crossover),
        replacement_(replacement) {};

  void Mutate(void) const override {
    if (replacement_) {
      // 重複を許す場合
      MutateWithReplacement();
    } else {
      // 重複を許さない場合
      MutateWithoutReplacement();
    }
  };

  void Crossover(
      const GeneAbstract<GeneValues>* gene_parent_1,
      const GeneAbstract<GeneValues>* gene_parent_2,
      unordered_map<string, Real> kwargs) const override {
    this->values_ = crossover_(gene_parent_1->values_, gene_parent_2->values_);
  };

  void Realize(void) const override {
    this->values_ = Sample<GeneValues>(
      candidates_, this->dimension_, replacement_);
  };
};

}