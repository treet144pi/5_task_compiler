#include "loops.hpp"
#include "union_find.hpp"

#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace {
// DFS from start implementation
void dfs_impl(const Graph& g, int v,
                              std::vector<int>& number,
                              std::vector<int>& node,
                              std::vector<int>& parent,
                              std::vector<int>& last,
                              int& timer) {

    number[v] = timer;
    node[timer] = v;
    ++timer;

    for (int u : g.succ[v]) {
        if (number[u] == -1) {
            parent[u] = v;
            dfs_impl(g, u, number, node, parent, last, timer);
        }
    }

    last[v] = timer - 1;
}

};

std::string type_name(Type t) {
    if (t == Type::IRREDUCIBLE) {
        return "irreducible";
    }

    return "reducible";
}

DfsData dfs_run(const Graph& g, int start) {
    int n = static_cast<int>(g.name.size());

    DfsData data;
    data.number.assign(n, -1); // preorder number of v in DFS tree
    data.node.assign(n, -1); // vertex with preorder number i
    data.parent.assign(n, -1); // parent of v in DFS tree
    data.last.assign(n, -1); // preorder number of the last descendant v

    int timer = 0;
    dfs_impl(g, start, data.number, data.node,
                       data.parent, data.last, timer);

    return data;
}

bool is_ancestor(int a, int b,
                 const std::vector<int>& number,
                 const std::vector<int>& last) {
    return number[a] <= number[b] && number[b] <= last[a];
}

LoopResult analyze_loops(const Graph& g, int start) {
    int n = static_cast<int>(g.name.size());

    DfsData dfs = dfs_run(g, start);

    std::vector<std::vector<int>> backPreds(n); // sources of backedges entering w
    std::vector<std::vector<int>> nonBackPreds(n);

    for (int w = 0; w < n; ++w) {
        for (int v : g.pred[w]) {
            if (is_ancestor(w, v, dfs.number, dfs.last)) {
                backPreds[w].push_back(v);
            }
            else {
                nonBackPreds[w].push_back(v);
            }
        }
    }

    std::vector<int> header(n, start);
    std::vector<Type> type(n, Type::NONHEADER);
    header[start] = -1;

    UnionFind dsu(n);

    for (int wi = n - 1; wi >= 0; --wi) {
        int w = dfs.node[wi];
        if (w == -1) continue;

        std::set<int> P;

        for (int v : backPreds[w]) {
            if (v != w) {
                P.insert(dsu.find(v));
            }
            else {
                type[w] = Type::SELF;
            }
        }

        if (!P.empty() && type[w] != Type::SELF) {
            type[w] = Type::REDUCIBLE;
        }

        std::queue<int> q;
        for (int x : P) {
            q.push(x);
        }

        while (!q.empty()) {
            int x = q.front();
            q.pop();

            for (int y : nonBackPreds[x]) {
                int y0 = dsu.find(y);

                // if we reach a node that is outside the DFS subtree of node w,
                // then cycle has one more entry and must be irreducible
                if (!is_ancestor(w, y0, dfs.number, dfs.last)) {
                    type[w] = Type::IRREDUCIBLE;
                    nonBackPreds[w].push_back(y0);
                }
                else if (y0 != w && !P.count(y0)) {
                    P.insert(y0);
                    q.push(y0);
                }
            }
        }

        for (int x : P) {
            header[x] = w;
            dsu.merge_into(x, w);
        }
    }

    std::vector<int> loop_headers;
    for (int v = 0; v < n; ++v) {
        if (type[v] == Type::SELF || type[v] == Type::REDUCIBLE
                                  || type[v] == Type::IRREDUCIBLE) {
            loop_headers.push_back(v);
        }
    }

    std::vector<int> loops;
    loops.push_back(-1);
    for (int h : loop_headers) {
        loops.push_back(h);
    }

    std::unordered_map<int, int> header_to_loop;
    for (int i = 1, len_loop = static_cast<int>(loops.size()); i < len_loop; ++i) {
        header_to_loop[loops[i]] = i;
    }

    std::vector<std::pair<int, int>> loop_tree;
    for (int h : loop_headers) {
        int child_id = header_to_loop[h];
        int parent_id = 0;

        if (header[h] != -1 && header_to_loop.count(header[h])) {
            parent_id = header_to_loop[header[h]];
        }

        loop_tree.push_back({parent_id, child_id});
    }

    std::vector<std::vector<int>> blocks(loops.size());
    std::set<int> is_loop_header(loop_headers.begin(), loop_headers.end());

    for (int v = 0; v < n; ++v) {
        if (is_loop_header.count(v)) {
            int id = header_to_loop[v];
            blocks[id].push_back(v);
        }
        else {
            if (header_to_loop.count(header[v])) {
                blocks[header_to_loop[header[v]]].push_back(v);
            }
            else {
                blocks[0].push_back(v);
            }
        }
    }

    LoopResult result;
    result.header       = std::move(header);
    result.type         = std::move(type);
    result.loop_headers = std::move(loop_headers);
    result.loops        = std::move(loops);
    result.loop_tree    = std::move(loop_tree);
    result.blocks       = std::move(blocks);

    return result;
}

// OUTPUT
void print_loop_result(const Graph& g, const LoopResult& result) {
    std::cout << "Loop blocks:\n";
    for (int i = 0, res_block_sz = static_cast<int>(result.blocks.size()); i < res_block_sz;  ++i) {
        std::cout << "[" << i << "] -> {";

        for (int j = 0, res_blocki_sz = static_cast<int>(result.blocks[i].size()); j < res_blocki_sz; ++j) {
            std::cout << g.name[result.blocks[i][j]];

            if (j + 1 < res_blocki_sz) {
                std::cout << ", ";
            }
        }
        std::cout << "}";

        if (i != 0) {
            int h = result.loops[i];
            std::cout << ", header: " << g.name[h]
                      << ", " << type_name(result.type[h]);
        }

        std::cout << "\n";
    }

    std::cout << "Loop tree:\n";
    for (auto [p, c] : result.loop_tree) {
        std::cout << "{" << p << "," << c << "}\n";
    }
}
