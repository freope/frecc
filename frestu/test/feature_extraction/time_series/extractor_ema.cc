#include <iostream>
#include <Eigen/Dense>
#include "gtest/gtest.h"
#include "frestu/feature_extraction/time_series/extractor_ema.h"
#include "frestu/util/io/table_reader.h"
#include "frestu/data_type/data_frame/data_frame.h"

namespace {

using Real = double;

using std::cout;
using std::endl;
using std::make_tuple;
using Eigen::Map;
using Eigen::VectorXd;
using frestu::data_type::data_frame::DataFrame;
using frestu::feature_extraction::time_series::ExtractorEma;
using frestu::util::io::TableReader;

TEST(ExtractorEma, Extract) {
  auto tr = TableReader("./data/ts.dat");
  auto value = tr.ReadColumn<double>(1, [](auto v) { return stod(v); });
  VectorXd columns = Map<VectorXd>(&value[0], value.size());
  auto df = DataFrame(make_tuple(columns));
  auto extractor = ExtractorEma(2);
  auto mat = extractor.Extract<Real>(df);

  EXPECT_NEAR(mat(0, 0), 0, 0.1);
  EXPECT_NEAR(mat(1, 0), 6.6, 0.1);
  EXPECT_NEAR(mat(9, 0), 47.7, 0.1);
  EXPECT_NEAR(mat(10, 0), 62.5, 0.1);
}

}