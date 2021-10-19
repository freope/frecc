#include <iostream>
#include "gtest/gtest.h"
#include "frestu/optimization/ga/population.h"
#include "frestu/optimization/ga/individual.h"
#include "frestu/optimization/ga/gene/gene_discrete.h"
#include "frestu/optimization/ga/gene/gene_continuous.h"
#include "frestu/optimization/ga/gene/gene_continuous_logscale.h"
#include "frestu/optimization/ga/crossover.h"
#include "frestu/optimization/ga/selection.h"

namespace {

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
using frestu::optimization::ga::Population;
using frestu::optimization::ga::Individual;
using frestu::optimization::ga::gene::GeneDiscrete;
using frestu::optimization::ga::gene::GeneContinuous;
using frestu::optimization::ga::gene::GeneContinuousLogscale;
using frestu::optimization::ga::crossover::CrossoverUniform;
using frestu::optimization::ga::selection::SelectRanking;

// Discrete
using vs = vector<string>;
vs candidates {"rbf", "poly", "linear"};
int dimension_d = 1;
double mutate_probability_d = 0.5;
auto gene_d = GeneDiscrete<vs>(
  candidates, CrossoverUniform<vs>, false, dimension_d, mutate_probability_d);

// Continuous
double minimum_c = 0.0;
double maximum_c = 1.0;
bool random_realization_c = true;
int dimension_c = 2;
double mutate_probability_c = 0.5;
auto gene_c = GeneContinuous<vector<double>, double>(
  minimum_c, maximum_c, random_realization_c,
  dimension_c, mutate_probability_c);

// Continuous Logscale
double minimum_cl = 0.1;
double maximum_cl = 10.0;
bool random_realization_cl = true;
int dimension_cl = 2;
double mutate_probability_cl = 0.5;
auto gene_cl = GeneContinuousLogscale<vector<double>, double>(
  minimum_cl, maximum_cl, random_realization_cl,
  dimension_cl, mutate_probability_cl);

auto chromosome = make_tuple(gene_d, gene_c, gene_cl);
using Chrom = decltype(chromosome);
function<double(Chrom)> evaluate = [](Chrom chrom) {
  return get<2>(chrom).values_[0];
};
double learning_rate = 1.5;
auto individual_prototype = Individual<Chrom>(
  chromosome, evaluate, learning_rate);

TEST(Population, Create) {
  using Fitnesses = vector<double>;
  auto population = Population<Chrom, Fitnesses>::Create(
    individual_prototype, SelectRanking<Fitnesses>, 100, 3);
  EXPECT_EQ(population.individuals_->size(), 100);
}

TEST(Population, Alternate) {
  int pop_size = 100;
  int n_eletes = 10;
  using Fitnesses = vector<double>;
  auto population = Population<Chrom, Fitnesses>::Create(
    individual_prototype, SelectRanking<Fitnesses>, pop_size, n_eletes);
  population.Realize();
  population.Evaluate();

  // Alternate 前
  // for (const auto& ind : *population.individuals_) {
  //   cout << ind.fitness() << endl;
  // }
  // cout << "----" << endl;

  // 実行できることだけ確認
  population.Alternate();
  population.Evaluate();

  // Alternate 後
  // for (const auto& ind : *population.individuals_) {
  //   cout << ind.fitness() << endl;
  // }
  // cout << "----" << endl;
}

TEST(Population, Mutate) {
  int pop_size = 100;
  using Fitnesses = vector<double>;
  auto population = Population<Chrom, Fitnesses>::Create(
    individual_prototype, SelectRanking<Fitnesses>, pop_size, 1);
  population.Realize();
  // 実行できることだけ確認
  population.Mutate();
}

TEST(Population, Realize) {
  int pop_size = 2;
  using Fitnesses = vector<double>;
  auto population = Population<Chrom, Fitnesses>::Create(
    individual_prototype, SelectRanking<Fitnesses>, pop_size, 1);
  auto& inds = *(population.individuals_);

  // Realize 前
  for (int i = 0; i < pop_size; i++) {
    EXPECT_EQ(get<0>(inds[i].chromosome_).values_.size(), 0);
    EXPECT_EQ(get<1>(inds[i].chromosome_).values_.size(), 0);
    EXPECT_EQ(get<2>(inds[i].chromosome_).values_.size(), 0);
  }

  population.Realize();

  // Realize 後
  for (int i = 0; i < pop_size; i++) {
    EXPECT_EQ(get<0>(inds[i].chromosome_).values_.size(), 1);
    EXPECT_EQ(get<1>(inds[i].chromosome_).values_.size(), 2);
    EXPECT_EQ(get<2>(inds[i].chromosome_).values_.size(), 2);
  }
}

TEST(Population, Reshuffle) {
  int pop_size = 100;
  using Fitnesses = vector<double>;
  auto population = Population<Chrom, Fitnesses>::Create(
    individual_prototype, SelectRanking<Fitnesses>, pop_size, 1);
  population.Realize();
  population.Evaluate();
  // 実行できることだけ確認
  population.Reshuffle(pop_size/2);
}

TEST(Population, Evaluate) {
  int pop_size = 100;
  using Fitnesses = vector<double>;
  auto population = Population<Chrom, Fitnesses>::Create(
    individual_prototype, SelectRanking<Fitnesses>, pop_size, 1);
  population.Realize();
  // 評価の前に参照を取得しておく
  auto& inds = *(population.individuals_);
  // 各個体を評価
  population.Evaluate();

  // fitness の降順に並んでいることの確認
  for (int i = 0; i < pop_size-1; i++) {
    // cout << inds[i].fitness() << endl;
    EXPECT_TRUE(inds[i].fitness() > inds[i+1].fitness());
  }
  // ソートの伴い fitness だけでなく values_ も並べ替えられていることの確認。
  // 今回は、Continuous Logscale の values_[0] を fitness にする評価関数を、
  // 用いているので、それを利用する。
  for (int i = 0; i < pop_size-1; i++) {
    auto v1 = get<2>(inds[i].chromosome_).values_[0];
    auto v2 = get<2>(inds[i+1].chromosome_).values_[0];
    // cout << v1 << endl;
    EXPECT_TRUE(v1 > v2);
  }
}

TEST(Population, Save) {
  int pop_size = 100;
  using Fitnesses = vector<double>;
  auto population = Population<Chrom, Fitnesses>::Create(
    individual_prototype, SelectRanking<Fitnesses>, pop_size, 1);
  population.Realize();
  population.Evaluate();
  // 動作確認のみ
  population.Save("pop.txt");
}

TEST(Population, Load) {
  int pop_size = 100;
  using Fitnesses = vector<double>;
  auto population = Population<Chrom, Fitnesses>::Create(
    individual_prototype, SelectRanking<Fitnesses>, pop_size, 1);
  population.Realize();
  auto& individuals = *(population.individuals_);

  auto converts = make_tuple(
    [](auto& item) {return item;},
    [](auto& item) {return stod(item);},
    [](auto& item) {return stod(item);}
  );
  // 動作確認のみ
  population.Load(converts, "pop.txt");

  // for (const auto& ind : individuals) {
  //   cout << get<0>(ind.chromosome_).values_[0] << endl;
  //   cout << get<1>(ind.chromosome_).values_[0] << endl;
  //   cout << get<1>(ind.chromosome_).values_[1] << endl;
  //   cout << get<2>(ind.chromosome_).values_[0] << endl;
  //   cout << get<2>(ind.chromosome_).values_[1] << endl;
  // }
  // cout << "----" << endl;
}

}