#include "graph.hpp"
#include "loops.hpp"

#include <iostream>
#include <string>

int main() {
    std::string line;
    std::getline(std::cin, line);

    Graph g = read_graph_from_line(line);

    int start = g.id["start"];
    LoopResult result = analyze_loops(g, start);
    print_loop_result(g, result);

    return 0;
}
