#include "graph.hpp"
#include "ssa.hpp"

#include <iostream>
#include <string>
#include <vector>

int main() {
    Graph g;
    std::string cfg_line;
    std::string defs_line;

    std::getline(std::cin, cfg_line);
    std::getline(std::cin, defs_line);

    std::vector<std::string> cfg         = parse_words(cfg_line);
    std::vector<std::string> defs_tokens = parse_words(defs_line);

    size_t cfg_start = 0;
    if (!cfg.empty() && cfg[0] == "CFG") {
        cfg_start = 1;
    }

    for (size_t i = cfg_start; i + 1 < cfg.size(); i += 2) {
        int u = get_id(g, cfg[i]);
        int v = get_id(g, cfg[i + 1]);
        g.succ[u].push_back(v);
        g.pred[v].push_back(u);
    }

    std::vector<int> defs;
    size_t defs_start = 0;
    if (!defs_tokens.empty() && defs_tokens[0] == "Defs") {
        defs_start = 1;
    }

    for (size_t i = defs_start; i < defs_tokens.size(); ++i) {
        defs.push_back(get_id(g, defs_tokens[i]));
    }

    if (g.name.empty()) {
        return 0;
    }

    int start = g.id["start"];
    SSAResult result = analyze_ssa(g, start, defs);

    // OUTPUT
    std::cout << "{";
    bool first = true;
    for (int v : result.phi) {
        if (!first) {
            std::cout << ", ";
        }
        first = false;
        std::cout << g.name[v];
    }
    std::cout << "}\n";

    return 0;
}
