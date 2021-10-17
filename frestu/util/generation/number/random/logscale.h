#pragma once

#include <random>

namespace frestu::util::generation::number::random::logscale {

using Int = int;
using Real = double;

Real GenerateUniform(const Real minimum, const Real miximum);

template <typename Vec>
Vec GenerateUniform(const Real minimum, const Real maximum, const Int size) {
  Vec vec(size);

  for (Int i = 0; i < size; i++) {
    vec[i] = GenerateUniform(minimum, maximum);
  }

  return vec;
}

}