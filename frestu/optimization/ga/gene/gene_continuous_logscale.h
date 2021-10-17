#pragma once

#include <vector>
#include "frestu/optimization/ga/gene/gene_continuous.h"
#include "frestu/util/generation/number/random/logscale.h"
#include "frestu/util/generation/number/sequential/logscale.h"

namespace frestu::optimization::ga::gene {

using Int = int;
using Real = double;

using std::string;
using std::unordered_map;
using std::vector;
using frestu::optimization::ga::gene::GeneContinuous;
namespace rlog = frestu::util::generation::number::random::logscale;
namespace slog = frestu::util::generation::number::sequential::logscale;

template <typename GeneValues, typename GeneValue>
class GeneContinuousLogscale : public GeneContinuous<GeneValues, GeneValue> {
  GeneValues RealizeRandom(void) const override {
    return rlog::GenerateUniform<GeneValues>(
      this->minimum_, this->maximum_, this->dimension_);
  };

  GeneValues RealizeSequential(void) const override {
    return slog::GenerateQuantity<GeneValues>(
      this->minimum_, this->maximum_, this->dimension_);
  };

public:
  // デフォルトコンストラクタを定義しない。
  GeneContinuousLogscale(
      GeneValue minimum, GeneValue maximum, bool random_realization,
      Int dimension, Real mutate_probability)
      : GeneContinuous<GeneValues, GeneValue>(
          minimum, maximum, random_realization,
          dimension, mutate_probability) {};

  /**
   * @param[in] kwargs const 修飾をできないので、参照渡しでなくコピー渡し。
   */
  void Crossover(
      const GeneAbstract<GeneValues>* gene_parent_1,
      const GeneAbstract<GeneValues>* gene_parent_2,
      unordered_map<string, Real> kwargs) const override {
    const Real fitness_parent_1 = kwargs["fitness_parent_1"];
    const Real fitness_parent_2 = kwargs["fitness_parent_2"];
    const Real learning_rate = kwargs["learning_rate"];

    Real log_parent_1;
    Real log_parent_2;
    Real amount_of_change;
    const Real diff_fitness = fitness_parent_2 - fitness_parent_1;
    const Real sign_diff_fitness = \
      (diff_fitness == 0.0) ? 0.0 : diff_fitness / abs(diff_fitness);

    for (Int ix = 0; ix < this->dimension_; ix++) {
      log_parent_1 = log10(gene_parent_1->values_[ix]);
      log_parent_2 = log10(gene_parent_2->values_[ix]);

      amount_of_change = learning_rate \
        * (log_parent_2 - log_parent_1) \
        * sign_diff_fitness;

      this->values_[ix] = pow(10, log_parent_1 + amount_of_change);

      // 最小値以上最大値以下に制限する
      if (this->values_[ix] < this->minimum_) {
        this->values_[ix] = this->minimum_;
      }
      if (this->maximum_ < this->values_[ix]) {
        this->values_[ix] = this->maximum_;
      }
    }
  };
};

}