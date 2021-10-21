#include <chrono>
#include <functional>
#include <iostream>
#include "frestu/data_type/data_type.h"
#include "frestu/optimization/ga/population.h"
#include "frestu/optimization/ga/individual.h"
#include "frestu/optimization/ga/gene/gene_discrete.h"
#include "frestu/optimization/ga/crossover.h"
#include "frestu/optimization/ga/selection.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using frestu::optimization::ga::Population;
using frestu::optimization::ga::Individual;
using frestu::optimization::ga::gene::GeneDiscrete;
using frestu::optimization::ga::crossover::CrossoverUniform;
using frestu::optimization::ga::selection::SelectRanking;

int main(void) {
  using frestu::data_type::Int;
  using frestu::data_type::Real;

  // パラメータ設定
  const Int kN = 50;
  const Int kPOP_SIZE = 1000;
  const Int kN_ELETES = 3;
  const Int kGEN_MAX = 100000;
  const Int kPATIENCE = 1000;

  // 遺伝子の設計
  using GeneValues = std::vector<Int>;
  GeneValues candidates {0, 1};
  const bool replacement = true;
  const Int dimension = kN - 1;
  const Real mutate_prob = 0.01;
  auto gene_selection = GeneDiscrete<GeneValues>(
    candidates, CrossoverUniform<GeneValues>,
    replacement, dimension, mutate_prob);

  // 染色体の設計
  auto chromosome = std::make_tuple(gene_selection);
  using Chrom = decltype(chromosome);

  // 遺伝子の翻訳関数
  std::function<GeneValues(Chrom)> translate_chromosome = [](auto chrom) {
    return std::get<0>(chrom).values_;
  };

  // 遺伝子の評価関数
  std::function<Real(Chrom)> evaluate = [&](auto chrom) {
    GeneValues selection = translate_chromosome(chrom);
    Real sum_1 = 1;
    Real sum_2 = 0;

    for (Int i = 0; i < dimension; i++) {
      const Real num = std::sqrt(i + 2);
      if (selection[i] == 0) {
        sum_1 += num;
      } else {
        sum_2 += num;
      }
    }

    const Real fitness = -std::abs(sum_1 - sum_2);
    return fitness;
  };

  // 個体の設計
  auto individual_prototype = Individual<Chrom>(chromosome, evaluate);

  // 集合の設計
  using Fitnesses = std::vector<double>;
  auto population = Population<Chrom, Fitnesses>::Create(
    individual_prototype, SelectRanking<Fitnesses>, kPOP_SIZE, kN_ELETES);
  population.Realize();

  // 第 0 世代の計算
  Int best_gen = 0;
  Int i_gen = 0;

  population.Evaluate("_OpenMP");
  Real best_fitness = (*(population.individuals_))[0].fitness();

  const auto t_0 = std::chrono::high_resolution_clock::now();
  std::cout << "GENERATION: " << i_gen << std::endl;
  std::cout << "BEST_FITNESS: " << best_fitness << std::endl;
  std::cout << "----" << std::endl;

  // 第 1 世代以降を計算
  for (Int i_gen = 0; i_gen < kGEN_MAX; i_gen++) {
    population.Alternate();
    population.Mutate();
    population.Evaluate("_OpenMP");
    population.Reshuffle(kPOP_SIZE/2);

    Real best_generation_fitness = (*(population.individuals_))[0].fitness();
    if (best_fitness < best_generation_fitness) {
      best_fitness = best_generation_fitness;
      best_gen = i_gen;
      auto t_1 = std::chrono::high_resolution_clock::now();
      auto duration = duration_cast<milliseconds>(t_1 - t_0).count() / 1000.;
      std::cout << "DURATION: " << duration << std::endl;
      std::cout << "GENERATION: " << i_gen << std::endl;
      std::cout << "BEST_FITNESS: " << best_fitness << std::endl;
      std::cout << "----" << std::endl;
    }

    // 世代交代を繰り返しても、指定回数、最良個体の適応度が上がらなければ終了
    if (kPATIENCE < i_gen - best_gen) {
      auto t_2 = std::chrono::high_resolution_clock::now();
      auto duration = duration_cast<milliseconds>(t_2 - t_0).count() / 1000.;
      std::cout << "DURATION: " << duration << std::endl;
      break;
    }
  }

  return 0;
}
