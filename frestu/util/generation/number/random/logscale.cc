#include "frestu/util/generation/number/random/logscale.h"
#include <cmath>

namespace frestu::util::generation::number::random::logscale {

// 非決定的な乱数生成器
random_device randomDevise;
// メルセンヌ・ツイスタの32ビット版、引数は初期シード値
mt19937 randomEngineMt(randomDevise());

Real GenerateUniform(const Real minimum, const Real maximum) {
  Real r = (Real)randomEngineMt() / randomEngineMt.max();

  const Real log_minimum = log10(minimum);
  const Real log_maximum = log10(maximum);

  r = r * (log_maximum - log_minimum) + log_minimum;
  r = pow(10, r);
  
  return r;
}

}