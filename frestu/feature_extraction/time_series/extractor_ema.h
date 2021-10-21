#pragma once

#include <Eigen/Dense>
#include "frestu/data_type/data_type.h"

namespace frestu::feature_extraction::time_series {

using Int = frestu::data_type::Int;
using Real = frestu::data_type::Real;

using std::get;
using Eigen::Dynamic;
using Eigen::Matrix;

class ExtractorEma {
  static const Int rolling_window_ = 1;
  static const Int rolling_window_recursive_ = 1;
  static const Int size_columns_ = 1;
  const Int window_;

public:
  ExtractorEma(const Int window) : window_(window) {};

  /**
   * @param[in] df const を付けるとエラー。
   */
  template <typename FnRet, typename Df>
  Matrix<FnRet, Dynamic, Dynamic> Extract(Df& df) {
    const Real alpha = 2.0 / (static_cast<Real>(window_) + 1.0);
    Matrix<Real, rolling_window_recursive_, size_columns_> initial_values;
    // 通常、シンプル移動平均を初期値に用いるが、pandas に合わせて以下。
    initial_values(0, 0) = get<0>(df.columns_)(0);

    const auto ma = df.template RollRecursion<FnRet>(
      [&alpha](const auto& b, const auto& r) {
        return r(0, 0) + alpha * (b(0, 0) - r(0, 0));
      },
      rolling_window_, rolling_window_recursive_, initial_values
    );

    return ma;
  };
};

}