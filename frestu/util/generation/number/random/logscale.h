#pragma once

#include <random>
#include "frestu/data_type/data_type.h"

namespace frestu::util::generation::number::random::logscale {

using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using frestu::data_type::Int;
using frestu::data_type::Real;

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