#pragma once
#include <vector>
#include <limits>
#include "json.hpp"

const double INF = std::numeric_limits<double>::infinity();

class TSPGraph {
public:
    int n; // number of nodes
    std::vector<std::vector<double>> dist; // full NxN shortest-path distance matrix

    TSPGraph(const nlohmann::json& graph_json) {
        n = graph_json["nodes"].size();

        // Initialize distance matrix with INF, 0 on diagonal
        dist.assign(n, std::vector<double>(n, INF));
        for (int i = 0; i < n; i++) dist[i][i] = 0.0;

        // Fill in direct edges (undirected)
        for (const auto& edge : graph_json["edges"]) {
            int u = edge["u"];
            int v = edge["v"];
            double w = edge["w"];
            dist[u][v] = std::min(dist[u][v], w);
            dist[v][u] = std::min(dist[v][u], w);
        }

        // Floyd-Warshall: find all-pairs shortest paths
        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (dist[i][k] != INF && dist[k][j] != INF)
                        dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
    }
};
