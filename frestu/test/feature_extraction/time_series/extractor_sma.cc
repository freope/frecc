#include <iostream>
#include <Eigen/Dense>
#include "gtest/gtest.h"
#include "frestu/data_type/data_frame/data_frame.h"
#include "frestu/feature_extraction/time_series/extractor_sma.h"
#include "frestu/util/io/table_reader.h"

namespace {

using std::cout;
using std::endl;
using std::get;
using std::make_tuple;
using Eigen::Map;
using Eigen::VectorXd;
using frestu::data_type::Real;
using frestu::data_type::data_frame::DataFrame;
using frestu::feature_extraction::time_series::ExtractorSma;
using frestu::util::io::TableReader;

TEST(ExtractorSma, Extract) {
  auto tr = TableReader("./data/ts.dat");
  auto value = tr.ReadColumn<double>(1, [](auto v){ return stod(v); });
  VectorXd columns = Map<VectorXd>(&value[0], value.size());
  auto df = DataFrame(make_tuple(columns));
  auto extractor = ExtractorSma(2);
  auto mat = extractor.Extract<Real>(df);

  EXPECT_NEAR(mat(0, 0), 9999, 0.1);
  EXPECT_NEAR(mat(1, 0), 5, 0.1);
  EXPECT_NEAR(mat(9, 0), 45, 0.1);
  EXPECT_NEAR(mat(10, 0), 65, 0.1);
}

}
