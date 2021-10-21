#pragma once

#include <Eigen/Dense>

namespace frestu::feature_extraction::time_series {

using Int = int;
using Real = double;

using Eigen::Dynamic;
using Eigen::Matrix;

class ExtractorSstd {
  const Int window_;

public:
  ExtractorSstd(const Int window) : window_(window) {};

  template <typename FnRet, typename Df>
  Matrix<FnRet, Dynamic, Dynamic> Extract(Df& df) {
    // 不偏分散
    const Real denominator = static_cast<Real>(window_ - 1);

    const auto std = df.template Roll<FnRet>(
      [denominator](const auto& b){
        const auto x = (b.array() - b.mean()).matrix();
        const auto y = (x.transpose() * x).array();
        const auto z = (y / denominator).sqrt();
        return z(0, 0);
      },
      window_
    );

    return std;
  };

};

}