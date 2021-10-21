#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "gtest/gtest.h"
#include "frestu/feature_extraction/time_series/extractor_sstd.h"
#include "frestu/util/io/table_reader.h"
#include "frestu/data_type/data_frame/data_frame.h"

namespace {

using Real = double;

using std::cout;
using std::endl;
using std::get;
using std::make_tuple;
using std::vector;
using Eigen::Dynamic;
using Eigen::Map;
using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using frestu::data_type::data_frame::DataFrame;
using frestu::feature_extraction::time_series::ExtractorSstd;
using frestu::util::io::TableReader;

TEST(ExtractorSstd, Extract) {
  auto tr = TableReader("./data/ts.dat");
  auto value = tr.ReadColumn<double>(1, [](auto v){return stod(v);});
  VectorXd vec = Map<VectorXd>(&value[0], value.size());
  auto df = DataFrame(make_tuple(vec));
  auto extractor = ExtractorSstd(2);
  auto mat = extractor.Extract<Real>(df);

  auto s = df.Roll<Real>([](const auto& w) {
    double window = 2;
    double n = window - 1;

    auto a = (w.array() - w.mean()).matrix();
    auto b = (a.transpose() * a).array();
    auto c = (b / n).sqrt();

    return c(0,0);
  }, 2);

  EXPECT_NEAR(mat(0, 0), 9999, 0.1);
  EXPECT_NEAR(mat(1, 0), 7.07, 0.1);
  EXPECT_NEAR(mat(9, 0), 21.2, 0.1);
  EXPECT_NEAR(mat(10, 0), 7.07, 0.1);
}

}
