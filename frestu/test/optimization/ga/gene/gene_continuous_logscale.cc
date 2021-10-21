#include <cmath>
#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/data_type/data_type.h"
#include "frestu/optimization/ga/crossover.h"
#include "frestu/optimization/ga/gene/gene_continuous_logscale.h"

namespace {

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;

TEST(GeneContinuousLogscale, Realize) {
  using namespace frestu::optimization::ga::gene;
  using namespace frestu::optimization::ga::crossover;

  Real minimum = 0.1;
  Real maximum = 100.0;
  bool random_realization = true;
  Int dimension = 5;
  Real mutate_probablility = 0.5;
  auto gene = GeneContinuousLogscale<vector<Real>, Real>(
    minimum, maximum, random_realization, dimension, mutate_probablility);

  gene.Realize();

  // for (const auto& c : gene.values_) {
  //     cout << c << endl;
  // }
  EXPECT_EQ(gene.values_.size(), 5);
}

TEST(GeneContinuousLogscale, Mutate) {
  using namespace frestu::optimization::ga::gene;
  using namespace frestu::optimization::ga::crossover;

  Real minimum = 0.1;
  Real maximum = 100.0;
  bool random_realization = true;
  Int dimension = 5;
  Real mutate_probablility = 0.5;
  auto gene = GeneContinuousLogscale<vector<Real>, Real>(
    minimum, maximum, random_realization, dimension, mutate_probablility);
  gene.Realize();
  // for (const auto& c : gene.values_) {
  //     cout << c << endl;
  // }
  // cout << "----" << endl;
  gene.Mutate();
  for (const auto& c : gene.values_) {
    // cout << c << endl;
    EXPECT_GT(c, minimum);
    EXPECT_LT(c, maximum);
  }
}

TEST(GeneContinuousLogscale, Crossover) {
  using namespace frestu::optimization::ga::gene;
  using namespace frestu::optimization::ga::crossover;

  Real minimum = 0.1;
  Real maximum = 100.0;
  bool random_realization = true;
  Int dimension = 5;
  Real mutate_probablility = 0.5;

  auto gene_parent_1 = GeneContinuousLogscale<vector<Real>, Real>(
    minimum, maximum, random_realization, dimension, mutate_probablility);
  gene_parent_1.Realize();

  auto gene_parent_2 = GeneContinuousLogscale<vector<Real>, Real>(
    minimum, maximum, random_realization, dimension, mutate_probablility);
  gene_parent_2.Realize();

  auto gene_child = GeneContinuousLogscale<vector<Real>, Real>(
    minimum, maximum, random_realization, dimension, mutate_probablility);
  unordered_map<string, Real> kwargs{
    { "fitness_parent_1", 1 },
    { "fitness_parent_2", 2 },
    { "learning_rate", 0.1 }
  };
  // Continuous の場合は、Realize しないとエラー
  gene_child.Realize();
  gene_child.Crossover(&gene_parent_1, &gene_parent_2, kwargs);

  Real log_child_value;
  for (Int ix = 0; ix < dimension; ix++) {
    log_child_value = kwargs["learning_rate"] \
      * (log10(gene_parent_2.values_[ix]) - log10(gene_parent_1.values_[ix])) \
      + log10(gene_parent_1.values_[ix]);
    // cout << log_child_value << endl;
    EXPECT_EQ(gene_child.values_[ix], pow(10, log_child_value));
  }
}

}