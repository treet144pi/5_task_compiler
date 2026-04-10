#include "graph.hpp"

std::vector<std::string> parse_words(const std::string& line) {
    std::vector<std::string> res;
    std::string cur;

    for (char c : line) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            cur += c;
        } else {
            if (!cur.empty()) {
                res.push_back(cur);
                cur.clear();
            }
        }
    }

    if (!cur.empty()) {
        res.push_back(cur);
    }

    return res;
}

int get_id(Graph& g, const std::string& s) {
    if (g.id.count(s)) {
        return g.id[s];
    }

    int v = g.name.size();
    g.id[s] = v;
    g.name.push_back(s);
    g.succ.push_back({});
    g.pred.push_back({});

    return v;
}

Graph read_graph_from_line(const std::string& line) {
    Graph g;
    std::vector<std::string> tokens = parse_words(line);

    for (int i = 0; i + 1 < static_cast<int>(tokens.size()); i += 2) {
        int u = get_id(g, tokens[i]);
        int v = get_id(g, tokens[i + 1]);
        g.succ[u].push_back(v);
        g.pred[v].push_back(u);
    }

    return g;
}
