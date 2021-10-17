#pragma once

#include <random>

namespace frestu::util::generation::number::random::linear {

using Int = int;
using Real = double;

Int GenerateUniform(const Int minimum, const Int maximum);

Real GenerateUniform(const Real minimum, const Real miximum);

template <typename Vec>
Vec GenerateUniform(const Int minimum, const Int maximum, const Int size) {
  Vec vec(size);

  for (Int i = 0; i < size; i++) {
    vec[i] = GenerateUniform(minimum, maximum);
  }

  return vec;
}

template <typename Vec>
Vec GenerateUniform(const Real minimum, const Real maximum, const Int size) {
  Vec vec(size);

  for (Int i = 0; i < size; i++) {
    vec[i] = GenerateUniform(minimum, maximum);
  }

  return vec;
}

}