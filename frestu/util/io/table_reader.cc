#include "frestu/util/io/table_reader.h"

namespace frestu::util::io {

TableReader::TableReader(const string path, const bool header, const string sep)
    : path_(path), header_(header), rx_(sep) {
  string line;
  ifstream infile(path_);

  // ヘッダーの読み込み
  if (header_) getline(infile, line);

  n_rows_ = 0;
  while (getline(infile, line)) { n_rows_++; };

  infile.close();
};

Int TableReader::n_rows(void) {
  return n_rows_;
};

vector<string> TableReader::ReadColumn(const Int i_column) {
  vector<string> vec(n_rows_);
  string line;
  ifstream infile(path_);

  // ヘッダーの読み込み
  if (header_) getline(infile, line);

  Int i_row = 0;
  while (getline(infile, line)) {
    sregex_token_iterator it(line.begin(), line.end(), rx_, -1);
    sregex_token_iterator end;

    // 列位置の初期化
    for (Int i = 0; i < i_column; i++) it++;

    vec[i_row] = *(it++);

    i_row++;
  }

  infile.close();

  return vec;
};

}