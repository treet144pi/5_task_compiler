#include "ssa.hpp"

#include <algorithm>
#include <queue>
#include <set>
#include <vector>

// Intersection of dominator sets of all predecessors of a vertex.
std::vector<bool> intersection(const std::vector<int>& preds,
                               const std::vector<std::vector<bool>>& dom) {
    size_t n = dom.size();

    if (preds.empty()) {
        return std::vector<bool>(n, false);
    }

    std::vector<bool> res(n, true);

    for (int p : preds) {
        for (size_t i = 0; i < n; ++i) {
            res[i] = res[i] && dom[p][i];
        }
    }

    return res;
}

std::vector<std::vector<bool>> compute_dom(const Graph& g, int start) {
    int n = static_cast<int>(g.name.size());

    std::vector<std::vector<bool>> dom(n, std::vector<bool>(n, true));

    for (int v = 0; v < n; ++v) {
        if (v == start) {
            std::fill(dom[v].begin(), dom[v].end(), false);
            dom[v][start] = true;
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;

        for (int v = 0; v < n; ++v) {
            if (v == start) {
                continue;
            }

            std::vector<bool> new_set = intersection(g.pred[v], dom);
            new_set[v] = true;

            if (new_set != dom[v]) {
                dom[v] = new_set;
                changed = true;
            }
        }
    }

    return dom;
}

std::vector<int> compute_idom(const std::vector<std::vector<bool>>& dom, int start) {
    int n = static_cast<int>(dom.size());
    std::vector<int> idom(n, -1);

    for (int v = 0; v < n; ++v) {
        if (v == start) {
            continue;
        }

        for (int cand = 0; cand < n; ++cand) {
            if (cand == v || !dom[v][cand]) {
                continue;
            }

            bool skip = false;

            for (int other = 0; other < n; ++other) {
                if (other == v || other == cand || !dom[v][other]) {
                    continue;
                }

                if (dom[other][cand]) {
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                idom[v] = cand;
                break;
            }
        }
    }

    return idom;
}

// Compute dominance frontier using the idom tree.
std::vector<std::set<int>> compute_df(const Graph& g,
                                      const std::vector<int>& idom) {
    int n = static_cast<int>(g.name.size());
    std::vector<std::set<int>> df(n);

    for (int y = 0; y < n; ++y) {
        if (g.name[y] == "start" || g.name[y] == "stop") {
            continue;
        }
        if (g.pred[y].size() < 2) {
            continue;
        }

        for (int p : g.pred[y]) {
            int runner = p;
            while (runner != -1 && runner != idom[y]) {
                df[runner].insert(y);
                runner = idom[runner];
            }
        }
    }

    return df;
}

std::set<int> compute_phi(const std::vector<std::set<int>>& df,
                          const std::vector<int>& defs) {
    std::set<int> phi;
    std::queue<int> q;
    std::set<int> defs_set(defs.begin(), defs.end());

    for (int v : defs) {
        q.push(v);
    }

    while (!q.empty()) {
        int x = q.front();
        q.pop();

        for (int y : df[x]) {
            if (!phi.count(y)) {
                phi.insert(y);
                if (!defs_set.count(y)) {
                    q.push(y);
                }
            }
        }
    }

    return phi;
}

SSAResult analyze_ssa(const Graph& g, int start, const std::vector<int>& defs) {
    SSAResult result;

    result.dom = compute_dom(g, start);
    result.idom = compute_idom(result.dom, start);
    result.df = compute_df(g, result.idom);
    result.phi = compute_phi(result.df, defs);

    return result;
}
