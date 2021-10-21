#pragma once

#include <string>
#include <unordered_map>
#include "frestu/data_type/data_type.h"

namespace frestu::optimization::ga::gene {

using std::string;
using std::unordered_map;
using frestu::data_type::Int;
using frestu::data_type::Real;

template <typename GeneValues>
class GeneAbstract {
protected:
  const Int dimension_;
  const Real mutate_probability_;

public:
  // const 修飾をされる tuple の要素として扱うので mutable 修飾
  mutable GeneValues values_;

  // デフォルトコンストラクタを定義しない。
  GeneAbstract(Int dimension, Real mutate_probability)
      : dimension_(dimension), mutate_probability_(mutate_probability) {};

  // const 修飾をされる tuple の要素として扱うので const 修飾
  virtual void Mutate(void) const = 0;
  virtual void Realize(void) const = 0;
  virtual void Crossover(
      const GeneAbstract* gene_parent_1,
      const GeneAbstract* gene_parent_2,
      unordered_map<string, Real> kwargs) const = 0;

  Int dimension(void) {
    return dimension_;
  };
};

}