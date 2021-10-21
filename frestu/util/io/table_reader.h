#pragma once

#include <fstream>
#include <functional>
#include <regex>
#include <string>
#include <vector>
#include "frestu/data_type/data_type.h"

namespace frestu::util::io {

using std::function;
using std::ifstream;
using std::regex;
using std::sregex_token_iterator;
using std::string;
using std::vector;
using frestu::data_type::Int;

class TableReader {
  const string path_;
  const bool header_;
  const regex rx_;
  Int n_rows_;

public:
  TableReader(const string path, const bool header=false, const string sep=",");

  Int n_rows(void);

  vector<string> ReadColumn(const Int ix_column);

  template <typename ColumnType>
  vector<ColumnType> ReadColumn(
      const Int ix_column,
      const function<ColumnType(string)>& convert);
  
  template <typename ColumnType>
  vector<ColumnType> ReadColumns(
      const vector<Int>& ixs_column,
      const function<ColumnType(string)>& convert);
};

template <typename ColumnType>
vector<ColumnType> TableReader::ReadColumn(
    const Int ix_column, const function<ColumnType(string)>& convert) {
  vector<ColumnType> vec(n_rows_);
  string line;
  ifstream infile(path_);

  // ヘッダーの読み込み
  if (header_) getline(infile, line);

  Int i_row = 0;
  while (getline(infile, line)) {
    sregex_token_iterator it(line.begin(), line.end(), rx_, -1);
    sregex_token_iterator end;

    // 列位置の初期化
    for (Int i = 0; i < ix_column; i++) it++;

    vec[i_row] = convert(*(it++));

    i_row++;
  }

  infile.close();

  return vec;
};

template <typename ColumnType>
vector<ColumnType> TableReader::ReadColumns(
    const vector<Int>& ixs_column,
    const function<ColumnType(string)>& convert) {
  size_t n_columns = ixs_column.size();
  vector<ColumnType> vec(n_rows_ * n_columns);
  string line;

  for (size_t i_column = 0; i_column < n_columns; i_column++) {
    ifstream infile(path_);

    // ヘッダーの読み込み
    if (header_) getline(infile, line);

    Int i_row = 0;
    while (getline(infile, line)) {
      sregex_token_iterator it(line.begin(), line.end(), rx_, -1);
      sregex_token_iterator end;

      // 列位置の初期化
      Int ix_column = ixs_column[i_column];
      for (Int i = 0; i < ix_column; i++) it++;

      vec[n_rows_ * i_column + i_row] = convert(*(it++));

      i_row++;
    }

    infile.close();
  }

  return vec;
};

}