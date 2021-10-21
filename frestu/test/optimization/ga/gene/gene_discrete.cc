#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "frestu/data_type/data_type.h"
#include "frestu/optimization/ga/gene/gene_discrete.h"
#include "frestu/optimization/ga/crossover.h"

namespace {

using Real = frestu::data_type::Real;

using std::vector;
using std::cout;
using std::endl;

TEST(GeneDiscrete, Realize) {
  using namespace frestu::optimization::ga::gene;
  using namespace frestu::optimization::ga::crossover;

  using vs = vector<string>;
  vs candidates {"hop", "step", "jump"};
  auto gene = GeneDiscrete<vs>(
    candidates, CrossoverUniform<vs>, false, 3, 0.01);
  gene.Realize();
  auto values = gene.values_;

  for (const auto& val : values) {
    // cout << val << endl;
    EXPECT_TRUE(
      (val == candidates[0])
        || (val == candidates[1])
        || (val == candidates[2])
    );
  }
}

TEST(GeneDiscrete, Mutate) {
  using namespace frestu::optimization::ga::gene;
  using namespace frestu::optimization::ga::crossover;

  using vs = vector<string>;
  vs candidates {"hop", "step", "jump"};
  // 復元抽出
  auto gene_with = GeneDiscrete<vs>(
    candidates, CrossoverUniform<vs>, true, 3, 0.5);
  gene_with.Realize();
  // for (const auto& val : gene_with.values_) {
  //     cout << val << endl;
  // }
  // cout << "----" << endl;
  gene_with.Mutate();
  for (const auto& val : gene_with.values_) {
    // cout << val << endl;
    // あまりテストになっていない。
    EXPECT_TRUE(
      (val == candidates[0])
        || (val == candidates[1])
        || (val == candidates[2])
    );
  }

  // cout << "----" << endl;

  // 非復元抽出
  auto gene_without = GeneDiscrete<vs>(
    candidates, CrossoverUniform<vs>, false, 3, 0.5);
  gene_without.Realize();
  // for (const auto& val : gene_without.values_) {
  //     cout << val << endl;
  // }
  // cout << "----" << endl;
  gene_without.Mutate();
  for (const auto& val : gene_without.values_) {
    // cout << val << endl;
    // あまりテストになっていない。
    EXPECT_TRUE(
      (val == candidates[0])
        || (val == candidates[1])
        || (val == candidates[2])
    );
  }
}

TEST(GeneDiscrete, Crossover) {
    using namespace frestu::optimization::ga::gene;
    using namespace frestu::optimization::ga::crossover;

    using vs = vector<string>;
    vs candidates {"hop", "step", "jump"};

    auto gene_parent_1 = GeneDiscrete<vs>(
      candidates, CrossoverUniform<vs>, true, 3, 0.01);
    gene_parent_1.Realize();

    auto gene_parent_2 = GeneDiscrete<vs>(
      candidates, CrossoverUniform<vs>, true, 3, 0.01);
    gene_parent_2.Realize();

    unordered_map<string, Real> kwargs;
    auto gene_child = GeneDiscrete<vs>(
      candidates, CrossoverUniform<vs>, true, 3, 0.01);
    gene_child.Crossover(&gene_parent_1, &gene_parent_2, kwargs);

    // cout << "---- gene" << endl;
    // for (const auto& v : gene_parent_1.values_) {
    //     cout << v << endl;
    // }
    // cout << "---- gene_partner" << endl;
    // for (const auto& v : gene_parent_2.values_) {
    //     cout << v << endl;
    // }
    // cout << "---- gene_child" << endl;
    // for (const auto& v : gene_child.values_) {
    //     cout << v << endl;
    // }
    for (const auto& val : gene_child.values_) {
      // あまりテストになっていない。
      EXPECT_TRUE(
        (val == candidates[0])
          || (val == candidates[1])
          || (val == candidates[2])
      );
    }
}

}