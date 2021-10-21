#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "frestu/data_type/data_type.h"
#include "frestu/optimization/ga/gene/gene_abstract.h"
#include "frestu/util/iteration/tuple/application.h"

namespace frestu::optimization::ga {

using std::function;
using std::get;
using std::index_sequence;
using std::make_index_sequence;
using std::string;
using std::tuple_size;
using std::unordered_map;
using frestu::data_type::Int;
using frestu::data_type::Real;
using frestu::optimization::ga::gene::GeneAbstract;
using frestu::util::iteration::tuple::Apply;

template <typename Chrom>
class Individual {
  static constexpr size_t size_chrom_ = tuple_size<Chrom>::value;
  using ixs_chrom = make_index_sequence<size_chrom_>;
  using Evaluating = function<Real(Chrom)>;

  const Evaluating evaluate_;
  const Real learning_rate_;
  Real fitness_;

  template <typename Kwargs, size_t... ixs>
  void ApplyCrossover(
      const Chrom& chrom_parent_1, const Chrom& chrom_parent_2,
      const Kwargs& kwargs, index_sequence<ixs...>) {
    using swallow = int[];
    (void)swallow {
      (
        get<ixs>(chromosome_).Crossover(
          &get<ixs>(chrom_parent_1),
          &get<ixs>(chrom_parent_2),
          kwargs),
        0
      )...
    };
  };

  template <size_t... ixs>
  void CopyChromosome(
      const Chrom& chrom_from, const Chrom& chrom_to, index_sequence<ixs...>) {
    using swallow = int[];
    (void)swallow {
      (get<ixs>(chrom_to).values_ = get<ixs>(chrom_from).values_, 0)...
    };
  };

public:
  Chrom chromosome_;

  Individual(Chrom& chromosome, Evaluating& evaluate, Real learning_rate=1.5)
      : evaluate_(evaluate), learning_rate_(learning_rate),
        chromosome_(chromosome) {};

  Real fitness(void) const {
    return fitness_;
  };

  // HACK: ムーブ代入演算子の定義に失敗するので、コピー代入演算子を使っている。
  Individual& operator=(const Individual& another) {
    if (this != &another) {
      // evaluate_, learning_rate_ は、個体間で共通という前提なので、
      // chromosome_ と fitness_ だけコピーしている。
      CopyChromosome(another.chromosome_, chromosome_, ixs_chrom());
      fitness_ = another.fitness_;
    }
    return *this;
  };

  // HACK: タプルのメンバーを持つためか、定義に失敗する。
  // Individual& operator=(Individual&& x) noexcept {
  //   if (this != &x) {
  //     chromosome_ = MoveChromosome(x.chromosome_);
  //     fitness_ = x.fitness_;
  //   }
  //   return *this;
  // };

  void Mutate(void) {
    auto mutate = [](auto& gene) {
      gene.Mutate();
    };
    Apply(mutate, chromosome_, ixs_chrom());
  };

  /**
   * @fn void Crossover(Individual& parent_1, Individual& parent_2)
   * @brief 交叉
   * @param[in] parent_1 fitness() を使うので const 修飾はできないが参照なので注意。
   * @param[in] parent_2 fitness() を使うので const 修飾はできないが参照なので注意。
   * @return void
   */
  void Crossover(Individual& parent_1, Individual& parent_2) {
    auto kwargs = unordered_map<string, Real> {
      {"fitness_parent_1", parent_1.fitness()},
      {"fitness_parent_2", parent_2.fitness()},
      {"learning_rate", learning_rate_},
    };

    const auto& chrom_parent_1 = parent_1.chromosome_;
    const auto& chrom_parent_2 = parent_2.chromosome_;
    ApplyCrossover(
      chrom_parent_1, chrom_parent_2, kwargs, ixs_chrom());
  };

  void Realize(void) {
    auto realize = [](auto& gene) {
      gene.Realize();
    };
    Apply(realize, chromosome_, ixs_chrom());
  };

  void Evaluate(void) {
    fitness_ = evaluate_(chromosome_);
  };
};

}