#pragma once

#include <Eigen/Dense>
#include "frestu/data_type/data_type.h"

namespace frestu::feature_extraction::time_series {

using Int = frestu::data_type::Int;

using Eigen::Dynamic;
using Eigen::Matrix;

class ExtractorSma {
  const Int window_;

public:
  ExtractorSma(const Int window) : window_(window) {};

  /**
   * @param[in] df const を付けるとエラー。
   */
  template <typename FnRet, typename Df>
  Matrix<FnRet, Dynamic, Dynamic> Extract(Df& df) {
    const auto ma = df.template Roll<FnRet>(
      [](const auto& b) {
        return b.mean();
      },
      window_
    );

    return ma;
  };
};

}