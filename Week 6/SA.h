#pragma once
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <limits>
using namespace std;

const double SA_INF = numeric_limits<double>::infinity();

// ── Floyd-Warshall + submatrix extraction ────────────────────────────────────
vector<vector<double>> fw_submatrix(vector<vector<double>>& adj, vector<int>& nodes) {
    int n = adj.size();
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (adj[i][k] != SA_INF && adj[k][j] != SA_INF)
                    adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
    int m = nodes.size();
    vector<vector<double>> sub(m, vector<double>(m));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            sub[i][j] = adj[nodes[i]][nodes[j]];
    return sub;
}

// ── Tour cost ────────────────────────────────────────────────────────────────
double tour_cost(const vector<int>& tour, vector<vector<double>>& dist) {
    double cost = 0.0;
    for (int i = 0; i + 1 < (int)tour.size(); i++)
        cost += dist[tour[i]][tour[i+1]];
    return cost;
}

// ── Nearest neighbor starting tour ───────────────────────────────────────────
vector<int> nearest_neighbor_tour(vector<vector<double>>& dist) {
    int m = dist.size();
    vector<bool> visited(m, false);
    vector<int> tour = {0};
    visited[0] = true;
    for (int step = 0; step < m - 1; step++) {
        int cur = tour.back();
        double best = SA_INF;
        int best_next = -1;
        for (int v = 0; v < m; v++) {
            if (!visited[v] && dist[cur][v] < best) {
                best = dist[cur][v];
                best_next = v;
            }
        }
        tour.push_back(best_next);
        visited[best_next] = true;
    }
    tour.push_back(0); // return to start
    return tour;
}

// ── 2-opt neighbor ───────────────────────────────────────────────────────────
// Picks random i < j in [1..m-1] and reverses the segment between them.
// Node 0 stays fixed as start/end.
vector<int> two_opt_neighbor(const vector<int>& current) {
    static mt19937 rng(42);
    int m = current.size() - 1; // exclude the closing node
    uniform_int_distribution<int> dist(1, m - 1);
    int i = dist(rng);
    int j = dist(rng);
    if (i > j) swap(i, j);
    if (i == j) return current;

    vector<int> next = current;
    reverse(next.begin() + i, next.begin() + j + 1);
    return next;
}

// ── Acceptance probability ───────────────────────────────────────────────────
double acceptance_probability(double old_cost, double new_cost, double temperature) {
    if (new_cost < old_cost) return 1.0;
    return exp(-(new_cost - old_cost) / temperature);
}

// ── Simulated annealing ──────────────────────────────────────────────────────
vector<int> simulated_annealing(vector<vector<double>>& dist,
                                 double initial_temp  = 10000.0,
                                 double cooling_rate  = 0.995,
                                 double min_temp      = 1e-6,
                                 int    iters_per_temp = 200)
{
    static mt19937 rng(123);
    uniform_real_distribution<double> uni(0.0, 1.0);

    vector<int> current = nearest_neighbor_tour(dist);
    vector<int> best    = current;
    double current_cost = tour_cost(current, dist);
    double best_cost    = current_cost;
    double T            = initial_temp;

    while (T > min_temp) {
        for (int iter = 0; iter < iters_per_temp; iter++) {
            vector<int> neighbor = two_opt_neighbor(current);
            double neighbor_cost = tour_cost(neighbor, dist);
            double delta = neighbor_cost - current_cost;

            if (acceptance_probability(current_cost, neighbor_cost, T) > uni(rng)) {
                current      = neighbor;
                current_cost = neighbor_cost;
            }

            if (current_cost < best_cost) {
                best      = current;
                best_cost = current_cost;
            }
        }
        T *= cooling_rate;
    }

    return best;
}
