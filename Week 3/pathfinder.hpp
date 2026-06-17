#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <functional>
#include "graph.hpp"

struct SearchResult {
    bool path_found;
    int path_length;
    int nodes_explored;
};

// ── Heuristics ────────────────────────────────────────────────────────────────

inline double h_zero(const Node&, const Node&) { return 0.0; }

inline double h_euclidean(const Node& a, const Node& b) {
    double dy = a.y - b.y, dx = a.x - b.x;
    return std::sqrt(dy * dy + dx * dx);
}

inline double h_manhattan(const Node& a, const Node& b) {
    return std::abs(a.y - b.y) + std::abs(a.x - b.x);
}

// ── Generic A* (Dijkstra when heuristic = h_zero) ────────────────────────────

SearchResult astar(
    const Graph& map,
    const Node& start,
    const Node& goal,
    std::function<double(const Node&, const Node&)> heuristic)
{
    using Entry = std::pair<double, Node>; // (f, node)

    // encode node to single int for O(1) map lookups
    auto enc = [&](const Node& n) { return n.y * map.cols + n.x; };

    std::unordered_map<int, double> g_score;
    std::unordered_map<int, int>    came_from; // encoded parent
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> open;

    int nodes_explored = 0;

    g_score[enc(start)] = 0.0;
    open.push({heuristic(start, goal), start});

    while (!open.empty()) {
        auto [f, current] = open.top();
        open.pop();

        nodes_explored++;

        if (current == goal) {
            // Reconstruct path length
            int length = 0;
            int cur = enc(goal);
            int s   = enc(start);
            while (cur != s) {
                cur = came_from[cur];
                length++;
            }
            return {true, length, nodes_explored};
        }

        double g_cur = g_score[enc(current)];

        for (const Node& nb : map.get_neighbors(current)) {
            int nb_enc = enc(nb);
            double tentative_g = g_cur + 1.0; // unit cost grid

            auto it = g_score.find(nb_enc);
            if (it == g_score.end() || tentative_g < it->second) {
                g_score[nb_enc]   = tentative_g;
                came_from[nb_enc] = enc(current);
                double f_new = tentative_g + heuristic(nb, goal);
                open.push({f_new, nb});
            }
        }
    }

    return {false, 0, nodes_explored};
}

// ── Named wrappers ────────────────────────────────────────────────────────────

inline SearchResult dijkstra(const Graph& map, const Node& s, const Node& g) {
    return astar(map, s, g, h_zero);
}

inline SearchResult astar_euclidean(const Graph& map, const Node& s, const Node& g) {
    return astar(map, s, g, h_euclidean);
}

inline SearchResult astar_manhattan(const Graph& map, const Node& s, const Node& g) {
    return astar(map, s, g, h_manhattan);
}
