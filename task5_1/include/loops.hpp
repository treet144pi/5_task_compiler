#pragma once

#include "graph.hpp"

#include <string>
#include <utility>
#include <vector>

enum class Type {
    NONHEADER,
    SELF,
    REDUCIBLE,
    IRREDUCIBLE
};

struct DfsData {
    std::vector<int> number;
    std::vector<int> node;
    std::vector<int> parent;
    std::vector<int> last;
};

struct LoopResult {
    std::vector<int> header;
    std::vector<Type> type;
    std::vector<int> loop_headers;
    std::vector<int> loops;
    std::vector<std::pair<int, int>> loop_tree;
    std::vector<std::vector<int>> blocks;
};

std::string type_name(Type t);

DfsData dfs(const Graph& g, int start);

bool is_ancestor(int a, int b,
                 const std::vector<int>& number,
                 const std::vector<int>& last);

LoopResult analyze_loops(const Graph& g, int start);

void print_loop_result(const Graph& g, const LoopResult& result);
