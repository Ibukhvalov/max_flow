#pragma once

#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>


struct Edge {
    size_t to;
    size_t reverse_index;
    size_t capacity;
};

class FlowGraph {
public:
    static FlowGraph parse_dimacs(const std::string& filename) {
        std::ifstream file(filename);
        std::string buffer;
        FlowGraph graph;

        while(std::getline(file, buffer)) {
            std::stringstream line(buffer);
            char c;
            if (!(line >> c)) continue;
            switch (c) {
                case 'p': {
                    std::string problem_type;
                    size_t nodes, arcs;
                    line >> problem_type >> nodes >> arcs;
                    assert(problem_type == std::string("max"));
                    std::cout << "Nodes: " << nodes << '\t' << ", Arcs: " << arcs << std::endl;
                    graph.set_node_count(nodes);
                    break;
                }
                case 'c':
                    break;
                case 'n':
                    size_t node;
                    char t;
                    line >> node >> t;
                    if(t == 's') {
                        graph.source = node;
                    } else if (t == 't') {
                        graph.sink = node;
                    } else {
                        assert(false);
                    }
                    break;
                case 'a':
                    size_t from, to;
                    long long res_cap;
                    line >> from >> to >> res_cap;
                    graph.add_arc(from, to, res_cap);
                    break;
            }
        }

        return graph;
    }
    size_t FindMaxFlow() {
        assert(adjacency.size() > 1);
        assert(is_valid_node(source));
        assert(is_valid_node(sink));
        assert(source != sink);

        size_t max_flow = 0;

        while (auto path = find_augmenting_path()) {
            const size_t augment = path_capacity(*path);
            augment_path(*path, augment);
            max_flow += augment;
        }

        return max_flow;
    }
private:
    struct PathStep {
        size_t previous = 0;
        size_t edge_index = 0;

        bool reached() const {
            return previous != 0;
        }
    };

    size_t source = 0;
    size_t sink = 0;
    std::vector<std::vector<Edge>> adjacency;

    void set_node_count(size_t node_count) {
        adjacency.assign(node_count + 1, std::vector<Edge>{});
    }

    bool is_valid_node(size_t node) const {
        return node != 0 && node < adjacency.size();
    }

    void add_arc(size_t from, size_t to, long long capacity) {
        if (capacity == 0) {
            return;
        }

        if (capacity < 0) {
            std::swap(from, to);
            capacity = -capacity;
        }

        assert(is_valid_node(from));
        assert(is_valid_node(to));
        if (from == to) {
            return;
        }

        const size_t forward_index = adjacency[from].size();
        const size_t reverse_index = adjacency[to].size();

        adjacency[from].push_back({to, reverse_index, static_cast<size_t>(capacity)});
        adjacency[to].push_back({from, forward_index, 0});
    }

    std::optional<std::vector<PathStep>> find_augmenting_path() const {
        std::vector<PathStep> path (adjacency.size());
        std::queue<size_t> queue;

        path[source].previous = source;
        queue.push(source);

        while (!queue.empty() && !path[sink].reached()) {
            const size_t current = queue.front();
            queue.pop();

            for (size_t i = 0; i < adjacency[current].size(); ++i) {
                const Edge& edge = adjacency[current][i];

                if (path[edge.to].reached() || edge.capacity == 0) {
                    continue;
                }

                path[edge.to] = {current, i};

                if (edge.to == sink) {
                    return path;
                }

                queue.push(edge.to);
            }
        }

        if(path[sink].reached()) {
            return path;
        }
        return std::nullopt;
    }

    size_t path_capacity(const std::vector<PathStep>& path) const {
        size_t capacity = std::numeric_limits<size_t>::max();

        for (size_t node = sink; node != source; node = path[node].previous) {
            const PathStep& step = path[node];
            capacity = std::min(capacity, adjacency[step.previous][step.edge_index].capacity);
        }

        return capacity;
    }

    void augment_path(const std::vector<PathStep>& path, size_t amount) {
        for (size_t node = sink; node != source; node = path[node].previous) {
            const PathStep& step = path[node];
            Edge& edge = adjacency[step.previous][step.edge_index];
            Edge& reverse = adjacency[edge.to][edge.reverse_index];

            edge.capacity -= amount;
            reverse.capacity += amount;
        }
    }
};
