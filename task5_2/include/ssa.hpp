#pragma once

#include "graph.hpp"

#include <set>
#include <vector>

struct SSAResult {
    std::vector<std::vector<bool>> dom;
    std::vector<int> idom;
    std::vector<std::set<int>> df;
    std::set<int> phi;
};

std::vector<bool> intersection(const std::vector<int>& preds,
                               const std::vector<std::vector<bool>>& dom);
std::vector<std::vector<bool>> compute_dom(const Graph& g, int start);
std::vector<int> compute_idom(const std::vector<std::vector<bool>>& dom, int start);
std::vector<std::set<int>> compute_df(const Graph& g, const std::vector<int>& idom);
std::set<int> compute_phi(const std::vector<std::set<int>>& df, const std::vector<int>& defs);
SSAResult analyze_ssa(const Graph& g, int start, const std::vector<int>& defs);
