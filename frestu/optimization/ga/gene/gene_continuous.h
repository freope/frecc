#pragma once

#include <vector>
#include "frestu/data_type/data_type.h"
#include "frestu/optimization/ga/gene/gene_abstract.h"
#include "frestu/util/generation/number/random/linear.h"
#include "frestu/util/generation/number/sequential/linear.h"

namespace frestu::optimization::ga::gene {

using Int = frestu::data_type::Int;
using Real = frestu::data_type::Real;

using std::string;
using std::unordered_map;
using std::vector;
using frestu::optimization::ga::gene::GeneAbstract;
namespace rlin = frestu::util::generation::number::random::linear;
namespace slin = frestu::util::generation::number::sequential::linear;

template <typename GeneValues, typename GeneValue>
class GeneContinuous : public GeneAbstract<GeneValues> {
  virtual GeneValues RealizeRandom(void) const {
    return rlin::GenerateUniform<GeneValues>(
      minimum_, maximum_, this->dimension_);
  };

  virtual GeneValues RealizeSequential(void) const {
    return slin::GenerateQuantity<GeneValues>(
      minimum_, maximum_, this->dimension_);
  };

protected:
  const GeneValue minimum_;
  const GeneValue maximum_;
  const bool random_realization_;

public:
  // デフォルトコンストラクタを定義しない。
  GeneContinuous(
      GeneValue minimum, GeneValue maximum, bool random_realization,
      Int dimension, Real mutate_probability)
      : GeneAbstract<GeneValues>(dimension, mutate_probability),
        minimum_(minimum), maximum_(maximum),
        random_realization_(random_realization) {};

  void Mutate(void) const override {
    const auto probs = rlin::GenerateUniform<vector<Real>>(
      0.0, 1.0, this->dimension_);
    // private でも継承先でオーバーライドしたものに置き換わり得る。
    const auto values_mutated = RealizeRandom();

    for (Int ix = 0; ix < this->dimension_; ix++) {
      // cout << probs[ix] << endl;
      if (probs[ix] < this->mutate_probability_) {
        this->values_[ix] = values_mutated[ix];
      }
    }
  };

  /**
   * @param[in] kwargs const 修飾をできないので、参照渡しでなくコピー渡し。
   */
  virtual void Crossover(
      const GeneAbstract<GeneValues>* gene_parent_1,
      const GeneAbstract<GeneValues>* gene_parent_2,
      unordered_map<string, Real> kwargs) const override {
    const Real fitness_parent_1 = kwargs["fitness_parent_1"];
    const Real fitness_parent_2 = kwargs["fitness_parent_2"];
    const Real learning_rate = kwargs["learning_rate"];

    Real amount_of_change;
    const Real diff_fitness = fitness_parent_2 - fitness_parent_1;
    const Real sign_diff_fitness = \
      (diff_fitness == 0.0) ? 0.0 : diff_fitness / abs(diff_fitness);

    for (Int ix = 0; ix < this->dimension_; ix++) {
      amount_of_change = learning_rate \
        * (gene_parent_2->values_[ix] - gene_parent_1->values_[ix]) \
        * sign_diff_fitness;

      this->values_[ix] = gene_parent_1->values_[ix] + amount_of_change;

      // 最小値以上最大値以下に制限する
      if (this->values_[ix] < minimum_) {
        this->values_[ix] = minimum_;
      }
      if (maximum_ < this->values_[ix]) {
        this->values_[ix] = maximum_;
      }
    }
  };

  void Realize(void) const override {
    if (random_realization_) {
      // private でも継承先でオーバーライドしたものに置き換わり得る。
      this->values_ = RealizeRandom();
    } else {
      // private でも継承先でオーバーライドしたものに置き換わり得る。
      this->values_ = RealizeSequential();
    }
  };
};

}