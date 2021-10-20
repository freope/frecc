#pragma once

#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
#include <Eigen/Dense>

namespace frestu::data_type::data_frame {

using Int = int;

using std::get;
using std::index_sequence;
using std::make_index_sequence;
using std::tuple_size;
using Eigen::Dynamic;
using Eigen::Matrix;
using Eigen::Vector;

template <typename Columns>
class DataFrame {
  static constexpr size_t size_columns_ = tuple_size<Columns>::value;
  using ixs_column = make_index_sequence<size_columns_>;

  const Int alternative_to_nan_;
  Int n_rows_;
  Int n_columns_;

  template <typename FnRet, typename Fn, size_t... ixs>
  Vector<FnRet, Dynamic> Apply(Fn func, index_sequence<ixs...>) {
    Vector<FnRet, Dynamic> vec(n_columns_);

    using swallow = int[];
    (void)swallow {
      ((vec[ixs] = func(get<ixs>(columns_))), 0)...
    };

    return vec;
  };

  template <typename FnRet, typename Fn, size_t... ixs>
  Vector<FnRet, Dynamic> Apply(
      Fn func, Int first, Int last, index_sequence<ixs...>) {
    Vector<FnRet, Dynamic> vec(n_columns_);

    using swallow = int[];
    (void)swallow {
      (
        [&]() {
          auto column = get<ixs>(columns_);
          vec[ixs] = func(column.block(first, 0, last - first, column.cols()));
        }(),
        0
      )...
    };

    return vec;
  };

  template <typename FnRet, typename Fn, size_t... ixs>
  Matrix<FnRet, Dynamic, size_columns_> Roll(
      Fn func, Int window, index_sequence<ixs...>) {
    Matrix<FnRet, Dynamic, Dynamic> mat(n_rows_, n_columns_);

    using swallow = int[];
    (void)swallow {
      (
        [&]() {
          auto column = get<ixs>(columns_);

          // 計算できない行に alternative_to_nan_ を入れる。
          Int n_nans = window - 1;
          for (Int i_row = 0; i_row < n_nans; i_row++) {
            mat(i_row, ixs) = static_cast<FnRet>(alternative_to_nan_);
          }

          // ローリング
          for (Int i_row = n_nans; i_row < n_rows_; i_row++) {
            mat(i_row, ixs) = func(column.block(
              i_row - n_nans, 0, window, column.cols()));
          }
        }(),
        0
      )...
    };

    return mat;
  };

public:
  Columns columns_;

  DataFrame(Columns& columns, Int alternative_to_nan=9999)
      : alternative_to_nan_(alternative_to_nan), columns_(columns) {
    n_rows_ = get<0>(columns_).rows();
    n_columns_ = size_columns_;
  };

  template <typename FnRet, typename Fn>
  Vector<FnRet, Dynamic> Apply(Fn func) {
    return Apply<FnRet>(func, ixs_column());
  };

  template <typename FnRet, typename Fn>
  Vector<FnRet, Dynamic> Apply(Fn func, Int first, Int last) {
    return Apply<FnRet>(func, first, last, ixs_column());
  };

  template <typename FnRet, typename Fn>
  Matrix<FnRet, Dynamic, size_columns_> Roll(Fn func, Int window) {
    return Roll<FnRet>(func, window, ixs_column());
  };
};

}