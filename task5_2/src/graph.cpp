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
