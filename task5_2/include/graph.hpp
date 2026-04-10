#pragma once

#include <unordered_map>
#include <vector>
#include <string>

struct Graph {
    std::vector<std::vector<int>> succ;
    std::vector<std::vector<int>> pred;
    std::vector<std::string> name;
    std::unordered_map<std::string, int> id;
};

std::vector<std::string> parse_words(const std::string& line);
int get_id(Graph& g, const std::string& s);
