#include <iostream>
#include "graph.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: max_flow <dimacs-file>" << std::endl;
        return 1;
    }

    FlowGraph graph = FlowGraph::parse_dimacs(argv[1]);
    std::cout << "Max flow: " << graph.FindMaxFlow() << std::endl;
    return 0;
}
