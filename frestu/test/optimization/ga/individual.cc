#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/data_type/data_type.h"
#include "frestu/optimization/ga/individual.h"
#include "frestu/optimization/ga/gene/gene_discrete.h"
#include "frestu/optimization/ga/gene/gene_continuous.h"
#include "frestu/optimization/ga/gene/gene_continuous_logscale.h"
#include "frestu/optimization/ga/crossover.h"

namespace {

using Int = frestu::data_type::Int;
using Real = frestu::data_type::Real;

using std::cout;
using std::endl;
using std::get;
using std::function;
using std::index_sequence;
using std::make_index_sequence;
using std::make_tuple;
using std::string;
using std::tuple;
using std::tuple_size;
using std::unordered_map;
using std::vector;
using frestu::optimization::ga::Individual;
using frestu::optimization::ga::gene::GeneDiscrete;
using frestu::optimization::ga::gene::GeneContinuous;
using frestu::optimization::ga::gene::GeneContinuousLogscale;
using frestu::optimization::ga::crossover::CrossoverUniform;

// Discrete
using vs = vector<string>;
vs candidates {"rbf", "poly", "linear"};
Int dimension_d = 1;
Real mutate_probability_d = 0.5;
auto gene_d = GeneDiscrete<vs>(
  candidates, CrossoverUniform<vs>, false, dimension_d, mutate_probability_d);

// Continuous
Real minimum_c = 0.0;
Real maximum_c = 1.0;
bool random_realization_c = true;
Int dimension_c = 2;
Real mutate_probability_c = 0.5;
auto gene_c = GeneContinuous<vector<Real>, Real>(
  minimum_c, maximum_c, random_realization_c,
  dimension_c, mutate_probability_c);

// Continuous Logscale
Real minimum_cl = 0.1;
Real maximum_cl = 10.0;
bool random_realization_cl = true;
Int dimension_cl = 2;
Real mutate_probability_cl = 0.5;
auto gene_cl = GeneContinuousLogscale<vector<Real>, Real>(
  minimum_cl, maximum_cl, random_realization_cl,
  dimension_cl, mutate_probability_cl);

auto chromosome = make_tuple(gene_d, gene_c, gene_cl);
using Chrom = decltype(chromosome);
function<Real(Chrom)> evaluate = [](Chrom chrom) {
  return 1.0;
};
Real learning_rate = 1.5;
auto individual = Individual<Chrom>(chromosome, evaluate, learning_rate);

TEST(Individual, Realize) {
  auto ind(individual);

  // Realize 前
  EXPECT_EQ(get<0>(ind.chromosome_).values_.size(), 0);
  EXPECT_EQ(get<1>(ind.chromosome_).values_.size(), 0);
  EXPECT_EQ(get<2>(ind.chromosome_).values_.size(), 0);

  ind.Realize();

  // Realize 後
  EXPECT_EQ(get<0>(ind.chromosome_).values_.size(), dimension_d);
  EXPECT_EQ(get<1>(ind.chromosome_).values_.size(), dimension_c);
  EXPECT_EQ(get<2>(ind.chromosome_).values_.size(), dimension_cl);

//  // Discrete
//   for (const auto& c : get<0>(ind.chromosome_).values_) {
//     cout << c << endl;
//   }
//   // Continuous
//   for (const auto& c : get<1>(ind.chromosome_).values_) {
//     cout << c << endl;    
//   }
//   // Continuous Logscale  
//   for (const auto& c : get<2>(ind.chromosome_).values_) {
//     cout << c << endl;    
//   }
}

TEST(Individual, Mutate) {
  auto ind(individual);
  ind.Realize();

  // // Mutate 前
  // // Discrete
  // for (const auto& c : get<0>(ind.chromosome_).values_) {
  //   cout << c << endl;
  // }
  // // Continuous
  // for (const auto& c : get<1>(ind.chromosome_).values_) {
  //   cout << c << endl;    
  // }
  // // Continuous Logscale  
  // for (const auto& c : get<2>(ind.chromosome_).values_) {
  //   cout << c << endl;    
  // }

  // 動作確認のみ
  ind.Mutate();

  // // Mutate 後
  // // Discrete
  // for (const auto& c : get<0>(ind.chromosome_).values_) {
  //   cout << c << endl;
  // }
  // // Continuous
  // for (const auto& c : get<1>(ind.chromosome_).values_) {
  //   cout << c << endl;    
  // }
  // // Continuous Logscale  
  // for (const auto& c : get<2>(ind.chromosome_).values_) {
  //   cout << c << endl;    
  // }
}

TEST(Individual, Crossover) {
  // Discrete
  using vs = vector<string>;
  vs candidates {"rbf", "poly", "linear"};
  auto gd = GeneDiscrete<vs>(candidates, CrossoverUniform<vs>, false, 1, 0.01);

  // Continuous
  auto gc = GeneContinuous<vector<Real>, Real>(0.0, 1.0, true, 1, 0.01);

  // Continuous Logscale
  auto gcl = GeneContinuousLogscale<vector<Real>, Real>(
    0.1, 10.0, true, 1, 0.01);

  auto chromosome = make_tuple(gd, gc, gcl);
  using Chrom = decltype(chromosome);
  function<Real(Chrom)> evaluate = [](Chrom chrom) {
    return get<1>(chrom).values_[0];
  };
  Real learning_rate = 1.5;
  auto individual = Individual<Chrom>(chromosome, evaluate, learning_rate);

  auto ind(individual);
  auto parent_1(individual);
  auto parent_2(individual);

  ind.Realize();
  parent_1.Realize();
  parent_2.Realize();
  
  Real p11 = 0.2;
  Real p21 = 0.3;
  get<1>(ind.chromosome_).values_[0] = 0;
  get<1>(parent_1.chromosome_).values_[0] = p11;
  get<1>(parent_2.chromosome_).values_[0] = p21;

  Real p12 = 0.5;
  Real p22 = 1.0;
  get<2>(ind.chromosome_).values_[0] = 0.1;
  get<2>(parent_1.chromosome_).values_[0] = p12;
  get<2>(parent_2.chromosome_).values_[0] = p22;

  ind.Evaluate();
  parent_1.Evaluate();
  parent_2.Evaluate();

  // cout << get<0>(ind.chromosome_).values_[0] << endl;
  // cout << get<0>(parent_1.chromosome_).values_[0] << endl;
  // cout << get<0>(parent_2.chromosome_).values_[0] << endl;
  // cout << "----" << endl;
  // cout << get<1>(ind.chromosome_).values_[0] << endl;
  // cout << get<1>(parent_1.chromosome_).values_[0] << endl;
  // cout << get<1>(parent_2.chromosome_).values_[0] << endl;
  // cout << "----" << endl;
  // cout << get<2>(ind.chromosome_).values_[0] << endl;
  // cout << get<2>(parent_1.chromosome_).values_[0] << endl;
  // cout << get<2>(parent_2.chromosome_).values_[0] << endl;
  // cout << "----" << endl;

  ind.Crossover(parent_1, parent_2);

  // cout << get<0>(ind.chromosome_).values_[0] << endl;
  // cout << get<0>(parent_1.chromosome_).values_[0] << endl;
  // cout << get<0>(parent_2.chromosome_).values_[0] << endl;
  // cout << "----" << endl;
  // cout << get<1>(ind.chromosome_).values_[0] << endl;
  // cout << get<1>(parent_1.chromosome_).values_[0] << endl;
  // cout << get<1>(parent_2.chromosome_).values_[0] << endl;
  // cout << "----" << endl;
  // cout << get<2>(ind.chromosome_).values_[0] << endl;
  // cout << get<2>(parent_1.chromosome_).values_[0] << endl;
  // cout << get<2>(parent_2.chromosome_).values_[0] << endl;
  // cout << "----" << endl;

  Real i1 = learning_rate * (p21 - p11) + p11;
  Real i2 = pow(10, learning_rate * (log10(p22) - log10(p12)) + log10(p12));

  EXPECT_NEAR(get<1>(ind.chromosome_).values_[0], i1, 0.1);
  EXPECT_NEAR(get<2>(ind.chromosome_).values_[0], i2, 0.1);
}

TEST(Individual, Evaluate) {
  auto ind(individual);
  ind.Realize();

  // 0 で初期化されている。
  EXPECT_NEAR(ind.fitness(), 0.0, 0.1);

  ind.Evaluate();

  // Evaluate 後には、1.0 に変わっている。
  EXPECT_NEAR(ind.fitness(), 1.0, 0.1);
}

}