#include "frestu/util/generation/number/random/linear.h"

namespace frestu::util::generation::number::random::linear {

// 非決定的な乱数生成器
random_device randomDevise;
// メルセンヌ・ツイスタの32ビット版、引数は初期シード値
mt19937 randomEngineMt(randomDevise());

Int GenerateUniform(const Int minimum, const Int maximum) {
  // 範囲指定の一様乱数
  uniform_int_distribution<> rand(minimum, maximum);
  return rand(randomEngineMt);
}

Real GenerateUniform(const Real minimum, const Real maximum) {
  Real r = (Real)randomEngineMt() / randomEngineMt.max();

  r = r * (maximum - minimum) + minimum;
  
  return r;
}

}