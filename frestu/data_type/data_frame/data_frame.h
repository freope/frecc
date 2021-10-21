#pragma once

#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
#include <Eigen/Dense>
#include "frestu/data_type/data_type.h"

namespace frestu::data_type::data_frame {

using std::get;
using std::index_sequence;
using std::make_index_sequence;
using std::max;
using std::tuple_size;
using Eigen::Dynamic;
using Eigen::Matrix;
using Eigen::Vector;
using frestu::data_type::Int;

template <typename DfColumns>
class DataFrame {
  static constexpr size_t size_columns_ = tuple_size<DfColumns>::value;
  using ixs_column = make_index_sequence<size_columns_>;

  const Int alternative_to_nan_;
  Int n_rows_;

  template <typename FnRet, typename Fn, size_t... ixs>
  Vector<FnRet, Dynamic> Apply(const Fn& func, index_sequence<ixs...>) {
    Vector<FnRet, Dynamic> vec(size_columns_);

    using swallow = int[];
    (void)swallow {
      ((vec[ixs] = func(get<ixs>(columns_))), 0)...
    };

    return vec;
  };

  template <typename FnRet, typename Fn, size_t... ixs>
  Vector<FnRet, Dynamic> Apply(
      const Fn& func, const Int first, const Int last, index_sequence<ixs...>) {
    Vector<FnRet, Dynamic> vec(size_columns_);

    using swallow = int[];
    (void)swallow {
      (
        [&]() {
          // データフレームのカラム
          const auto column = get<ixs>(columns_);
          const size_t n_cols = column.cols();
          const auto b = column.block(first, 0, last - first, n_cols);
          vec[ixs] = func(b);
        }(),
        0
      )...
    };

    return vec;
  };

  template <typename FnRet, typename Fn, size_t... ixs>
  Matrix<FnRet, Dynamic, size_columns_> Roll(
      const Fn& func, const Int window, index_sequence<ixs...>) {
    Matrix<FnRet, Dynamic, Dynamic> mat(n_rows_, size_columns_);

    // ブロック生成時に使う、行のオフセット
    const Int offset = window - 1;
    
    // NaN である行数
    const Int n_nans = window - 1;

    using swallow = int[];
    (void)swallow {
      (
        [&]() {
          // データフレームのカラム
          const auto column = get<ixs>(columns_);
          // データフレームのカラムを構成するベクトルのカラム数
          const size_t n_cols = column.cols();

          // 計算できない行に alternative_to_nan_ を入れる。
          for (Int i_row = 0; i_row < n_nans; i_row++) {
            mat(i_row, ixs) = static_cast<FnRet>(alternative_to_nan_);
          }

          // ローリング
          for (Int i_row = n_nans; i_row < n_rows_; i_row++) {
            const auto b = column.block(i_row - offset, 0, window, n_cols);
            mat(i_row, ixs) = func(b);
          }
        }(),
        0
      )...
    };

    return mat;
  };

  template <typename FnRet, typename Fn, size_t... ixs>
  Matrix<FnRet, Dynamic, size_columns_> RollRecursion(
      const Fn& func, const Int window, const Int window_recursive,
      const Matrix<FnRet, Dynamic, size_columns_>& initial_values,
      index_sequence<ixs...>) {
    Matrix<FnRet, Dynamic, Dynamic> mat(n_rows_, size_columns_);

    // ブロック生成時に使う、行のオフセット
    const Int offset = window - 1;
    // 一つ前の値からなので -1 を付けない。
    const Int offset_recursive = window_recursive;

    // NaN である行数
    const Int window_max = max(window, window_recursive);
    const Int n_nans = window_max - 1;

    using swallow = int[];
    (void)swallow {
      (
        [&]() {
          // データフレームのカラム
          const auto column = get<ixs>(columns_);
          // データフレームのカラムを構成するベクトルのカラム数
          const size_t n_cols = column.cols();

          // 計算できない行に alternative_to_nan_ を入れる。
          for (Int i_row = 0; i_row < n_nans; i_row++) {
            mat(i_row, ixs) = static_cast<FnRet>(alternative_to_nan_);
          }

          // 初期計算
          const auto initial_b = column.block(0, 0, window, n_cols);
          const auto initial_values_recursive = initial_values.block(
            0, ixs, window_recursive, 1);
          mat(n_nans, ixs) = func(initial_b, initial_values_recursive);

          // 再帰ローリング
          for (Int i_row = window_max; i_row < n_rows_; i_row++) {
            const auto b = column.block(i_row - offset, 0, window, n_cols);
            const auto values_recursive = mat.block(
              i_row - offset_recursive, ixs, window_recursive, 1);
            mat(i_row, ixs) = func(b, values_recursive);
          }
        }(),
        0
      )...
    };

    return mat;    
  };

public:
  const DfColumns columns_;

  DataFrame(DfColumns& columns, Int alternative_to_nan=9999)
      : alternative_to_nan_(alternative_to_nan), columns_(columns) {
    n_rows_ = get<0>(columns_).rows();
  };

  DataFrame(DfColumns&& columns, Int alternative_to_nan=9999)
      : DataFrame(columns, alternative_to_nan) {};

  template <typename FnRet, typename Fn>
  Vector<FnRet, Dynamic> Apply(const Fn& func) {
    return Apply<FnRet>(func, ixs_column());
  };

  template <typename FnRet, typename Fn>
  Vector<FnRet, Dynamic> Apply(
      const Fn& func, const Int first, const Int last) {
    return Apply<FnRet>(func, first, last, ixs_column());
  };

  template <typename FnRet, typename Fn>
  Matrix<FnRet, Dynamic, size_columns_> Roll(const Fn& func, const Int window) {
    return Roll<FnRet>(func, window, ixs_column());
  };

  template <typename FnRet, typename Fn>
  Matrix<FnRet, Dynamic, size_columns_> RollRecursion(
      const Fn& func, const Int window, const Int window_recursive,
      const Matrix<FnRet, Dynamic, size_columns_>& initial_values) {
    return RollRecursion<FnRet>(
      func, window, window_recursive, initial_values, ixs_column());
  };
};

}