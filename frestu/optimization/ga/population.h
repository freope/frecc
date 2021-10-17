#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <regex>
#include <typeinfo>
#include <vector>
#include "frestu/optimization/ga/individual.h"

namespace frestu::optimization::ga {

using Int = int;
using Real = double;

using std::endl;
using std::function;
using std::ios_base;
using std::ifstream;
using std::ofstream;
using std::regex;
using std::sregex_token_iterator;
using std::sort;
using std::vector;
using frestu::optimization::ga::Individual;

template <typename Chrom, typename SelectedItems>
class Population {
  static constexpr size_t size_chrom = std::tuple_size<Chrom>::value;
  using ixs_chrom = make_index_sequence<size_chrom>;
  using Individuals = vector<Individual<Chrom>>;
  using Selecting = function<Int(SelectedItems)>;

  Individuals inds_1_;
  Individuals inds_2_;

  const Selecting select_;
  const Int n_eletes_;
  const Int pop_size_;

  // HACK: vector を指すアドレスなので、デストラクタは不要でOK?
  Individuals* individuals_next_;
  Individuals* individuals_tmp_;

  template <typename Itr>
  void Evaluate(Itr ind_first, Itr ind_last) {
    for (Itr ind_it = ind_first; ind_it != ind_last; ind_it++) {
      ind_it->Evaluate();      
    }
    // 評価後に必ずソート
    SortIndividuals();
  };

  template <typename Itr>
  void EvaluateWithOpenMP(Itr ind_first, Itr ind_last) {
    #pragma omp parallel
    #pragma omp for schedule(guided)
    for (Itr ind_it = ind_first; ind_it != ind_last; ind_it++) {
      ind_it->Evaluate();
    }
    // 評価後に必ずソート
    SortIndividuals();
  }

  void SortIndividuals(void) {
    auto& inds = *individuals_;
    sort(inds.begin(), inds.end(), [](const auto& ind_1, const auto& ind_2) {
      return ind_2.fitness() < ind_1.fitness();
    });
  };

  template <size_t... ixs>
  void SaveIndividuals(
      const string path, const string sep, index_sequence<ixs...>) {
    ofstream outfile(path, ios_base::out);
    // 1 行ごとに 1 個体を記録
    for (const auto& ind : *individuals_) {
      using swallow = int[];
      (void)swallow {
        ([&](){
          // gene 間と values_ 間の区別なくつなげる。
          auto gene = get<ixs>(ind.chromosome_);
          for (const auto& value : gene.values_) {              
            outfile << value;
            outfile << sep;
          }
        }(),
        0)...
      };
      // 最後の列に、個体の適応度を記録。
      outfile << ind.fitness();
      outfile << endl;
    }
    outfile.close();
  };

  /**
   * @details 異なる型での読み込みエラーを回避するため、gene 毎に読み込むようにした。
   *          gene の values_ は全て同じ型なので、これで対処できた。
   */
  template <typename Convertings, size_t... ixs>
  void LoadIndividuals(
      const Convertings& converts,
      const string path, const string sep, index_sequence<ixs...>) {

    auto& inds = *individuals_;

    regex rx(sep);
    string line;
    Int start_ix_col = 0;    
    // ixs のループ
    using swallow = int[];
    (void)swallow {
      ([&]() {
        auto& typical_gene = get<ixs>(inds[0].chromosome_);
        Int dimension = typical_gene.dimension();
        start_ix_col += dimension;

        Int ix_ind = 0;
        ifstream infile(path);
        // 行のループ
        while (getline(infile, line)) {
          sregex_token_iterator it(line.begin(), line.end(), rx, -1);
          sregex_token_iterator end;
          // 列位置の初期化
          for (Int i = 2; i < start_ix_col; i++) {
            it++;
          }
          // dim のループ
          for (Int ix_dim = 0; ix_dim < dimension; ix_dim++) {
            auto& gene = get<ixs>(inds[ix_ind].chromosome_);
            gene.values_[ix_dim] = get<ixs>(converts)(*(it++));
          }
          ix_ind++;
        }
        // NOTE: infile.seekg(0); でファイルの先頭に行けんの？
        infile.close();
      }(),
      0)...
    };
  };

public:
  Individuals* individuals_;

  Population(
      Individuals& inds, const Selecting& select, Int n_eletes)
      : inds_1_(inds), inds_2_(inds),
        select_(select), n_eletes_(n_eletes), pop_size_(inds.size()) {
    individuals_ = &inds_1_;
    individuals_next_ = &inds_2_;
  };

  void Alternate(void) {
    // individuals_ が事前にソートされている必要がある。
    auto& inds = *individuals_;
    auto& inds_next = *individuals_next_;

    // エリート保存戦略
    for (Int i = 0; i < n_eletes_; i++) {
      inds_next[i] = inds[i];
    }

    // 選択に用いる適応度
    vector<Real> fitnesses(pop_size_);
    for (Int i = 0; i < pop_size_; i++) {
      fitnesses[i] = inds[i].fitness();
    }

    // 選択と交叉
    for (Int i = n_eletes_; i < pop_size_; i++) {
      // 親を選択
      auto parent_1 = inds[select_(fitnesses)];
      auto parent_2 = inds[select_(fitnesses)];
      // 交叉
      inds_next[i].Crossover(parent_1, parent_2);
    }
    
    // 世代交代。ポインタでスワップ。
    individuals_tmp_ = individuals_;
    individuals_ = individuals_next_;
    individuals_next_ = individuals_tmp_;
  };

  void Mutate(void) {
    // エリートにも突然変異が起き得る
    for(auto& ind : *individuals_) {
      ind.Mutate();
    }
  };

  void Realize(void) {
    for (auto& ind : *individuals_) {
      ind.Realize();
    }
    // 次世代も初期化しておかないと Alternate できない。
    for (auto& ind_next : *individuals_next_) {
      ind_next.Realize();
    }
  };

  void Reshuffle(const Int n_subordinates, const string async_evaluation="") {
    // individuals_ が事前にソートされている必要がある。
    auto& inds = *(individuals_);
    for (Int i = n_subordinates; i < pop_size_; i++) {
      inds[i].Realize();
    }

    if (async_evaluation == "OpenMP") {
      EvaluateWithOpenMP(inds.begin() + n_subordinates, inds.end());
    } else {
      Evaluate(inds.begin() + n_subordinates, inds.end());
    }
  };

  void Evaluate(const string async_evaluation="") {
    auto& inds = *(individuals_);
    if (async_evaluation == "OpenMP") {
      EvaluateWithOpenMP(inds.begin(), inds.end());
    } else {
      Evaluate(inds.begin(), inds.end());
    }
  };

  void Save(const string path, const string sep=",") {
    SaveIndividuals(path, sep, ixs_chrom());
  };

  template <typename Convertings>
  void Load(Convertings converts, const string path, const string sep=",") {
    LoadIndividuals(converts, path, sep, ixs_chrom());
  };

  static Population Create(
      const Individual<Chrom>& individual_prototype,
      const Selecting& select, const Int pop_size, const Int n_eletes) {
    // 要素数を指定して Individuals inds(pop_size) だとエラー。
    Individuals inds;
    inds.reserve(pop_size);

    for (Int i = 0; i < pop_size; i++) {
      Individual<Chrom> ind(individual_prototype);
      inds.push_back(ind);
    }

    return Population(inds, select, n_eletes);
  };
};

}