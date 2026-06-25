#pragma once
#include <vector>
#include <algorithm>
#include <limits>
#include "tsp_graph.hpp"

const double TSP_INF = std::numeric_limits<double>::infinity();

struct TSPResult {
    double optimal_cost;
    std::vector<int> tour; // includes return to start, e.g. [0,2,1,0]
};

// ── Helper: get cost of a full tour given node ordering ──────────────────────
// nodes: the subset of nodes to visit (in some order), first is the fixed start
static double tour_cost(const std::vector<int>& order,
                        const std::vector<std::vector<double>>& dist)
{
    double cost = 0.0;
    for (int i = 0; i + 1 < (int)order.size(); i++)
        cost += dist[order[i]][order[i+1]];
    cost += dist[order.back()][order[0]]; // return to start
    return cost;
}

// ── Brute Force: try all (N-1)! permutations ─────────────────────────────────
TSPResult tsp_brute_force(const std::vector<int>& nodes,
                          const std::vector<std::vector<double>>& dist)
{
    // Fix nodes[0] as start, permute the rest
    int start = nodes[0];
    std::vector<int> rest(nodes.begin() + 1, nodes.end());
    std::sort(rest.begin(), rest.end()); // ensure we start from lexicographic first permutation

    double best_cost = TSP_INF;
    std::vector<int> best_order;

    do {
        std::vector<int> order = {start};
        order.insert(order.end(), rest.begin(), rest.end());

        double cost = tour_cost(order, dist);
        if (cost < best_cost) {
            best_cost = cost;
            best_order = order;
        }
    } while (std::next_permutation(rest.begin(), rest.end()));

    // Build tour: order + return to start
    std::vector<int> tour = best_order;
    tour.push_back(start);

    return {best_cost, tour};
}

// ── Held-Karp: exact DP with bitmask ─────────────────────────────────────────
// Time: O(2^N * N^2), Space: O(2^N * N)
// Works well up to N~20
TSPResult tsp_held_karp(const std::vector<int>& nodes,
                        const std::vector<std::vector<double>>& dist)
{
    int N = nodes.size();

    // dp[mask][i] = min cost to visit all nodes in mask, ending at node index i
    // mask is a bitmask over the N nodes (bit 0 = nodes[0], bit 1 = nodes[1], ...)
    // We always start at index 0 (nodes[0])

    int full_mask = (1 << N) - 1;
    std::vector<std::vector<double>> dp(1 << N, std::vector<double>(N, TSP_INF));
    std::vector<std::vector<int>>    parent(1 << N, std::vector<int>(N, -1));

    // Base case: start at node index 0, only node 0 visited
    dp[1][0] = 0.0;

    // Fill DP
    for (int mask = 1; mask <= full_mask; mask++) {
        for (int u = 0; u < N; u++) {
            if (!(mask & (1 << u))) continue;       // u not in mask
            if (dp[mask][u] == TSP_INF) continue;   // unreachable

            // Try extending to v
            for (int v = 0; v < N; v++) {
                if (mask & (1 << v)) continue;      // v already visited
                int new_mask = mask | (1 << v);
                double new_cost = dp[mask][u] + dist[nodes[u]][nodes[v]];
                if (new_cost < dp[new_mask][v]) {
                    dp[new_mask][v] = new_cost;
                    parent[new_mask][v] = u;
                }
            }
        }
    }

    // Find best last node to return from (back to nodes[0])
    double best_cost = TSP_INF;
    int last = -1;
    for (int u = 1; u < N; u++) {
        if (dp[full_mask][u] == TSP_INF) continue;
        double cost = dp[full_mask][u] + dist[nodes[u]][nodes[0]];
        if (cost < best_cost) {
            best_cost = cost;
            last = u;
        }
    }

    // Reconstruct tour by tracing parents
    std::vector<int> tour_indices;
    int mask = full_mask;
    int cur  = last;
    while (cur != -1) {
        tour_indices.push_back(cur);
        int prev = parent[mask][cur];
        mask ^= (1 << cur);
        cur = prev;
    }
    std::reverse(tour_indices.begin(), tour_indices.end());

    // Convert indices back to actual node IDs and add return to start
    std::vector<int> tour;
    for (int idx : tour_indices) tour.push_back(nodes[idx]);
    tour.push_back(nodes[0]);

    return {best_cost, tour};
}
