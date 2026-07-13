#pragma once
#include <vector>
#include <limits>
#include <algorithm>
#include <stack>
using namespace std;

const double INF = numeric_limits<double>::infinity();

vector<vector<double>> floyd_warshall(vector<vector<double>>& adj, vector<int>& nodes) {
    int n = adj.size();
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (adj[i][k] != INF && adj[k][j] != INF)
                    adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);

    int m = nodes.size();
    vector<vector<double>> sub(m, vector<double>(m));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            sub[i][j] = adj[nodes[i]][nodes[j]];
    return sub;
}

vector<pair<int,int>> prim_mst(vector<vector<double>>& dist) {
    int m = dist.size();
    vector<bool> visited(m, false);
    visited[0] = true;
    vector<pair<int,int>> edges;

    for (int iter = 0; iter < m - 1; iter++) {
        double best = INF;
        int best_u = -1, best_v = -1;
        for (int u = 0; u < m; u++) {
            if (!visited[u]) continue;
            for (int v = 0; v < m; v++) {
                if (visited[v]) continue;
                if (dist[u][v] < best) {
                    best = dist[u][v];
                    best_u = u;
                    best_v = v;
                }
            }
        }
        visited[best_v] = true;
        edges.push_back({best_u, best_v});
    }
    return edges;
}

vector<pair<int,int>> perfect_matching(vector<int>& odds, vector<vector<double>>& dist) {
    vector<bool> matched(odds.size(), false);
    vector<pair<int,int>> matching;
    int n = odds.size();

    for (int i = 0; i < n; i++) {
        if (matched[i]) continue;
        matched[i] = true;
        double best = INF;
        int best_j = -1;
        for (int j = 0; j < n; j++) {
            if (matched[j]) continue;
            if (dist[odds[i]][odds[j]] < best) {
                best = dist[odds[i]][odds[j]];
                best_j = j;
            }
        }
        matching.push_back({odds[i], odds[best_j]});
        matched[best_j] = true;
    }
    return matching;
}

vector<int> eulerian_circuit(int m, vector<vector<pair<int,int>>>& adj_list) {
    int total_edges = 0;
    for (int u = 0; u < m; u++)
        total_edges += adj_list[u].size();
    vector<bool> used_edge(total_edges, false);
    vector<int> ptr(m, 0);

    vector<int> stk = {0};
    vector<int> circuit;

    while (!stk.empty()) {
        int u = stk.back();
        bool found = false;
        while (ptr[u] < (int)adj_list[u].size()) {
            auto [v, eid] = adj_list[u][ptr[u]];
            ptr[u]++;
            if (!used_edge[eid]) {
                used_edge[eid] = true;
                stk.push_back(v);
                found = true;
                break;
            }
        }
        if (!found) {
            circuit.push_back(u);
            stk.pop_back();
        }
    }
    reverse(circuit.begin(), circuit.end());
    return circuit;
}

double tour_cost(const vector<int>& tour, vector<vector<double>>& dist) {
    double cost = 0.0;
    for (int i = 0; i + 1 < (int)tour.size(); i++)
        cost += dist[tour[i]][tour[i+1]];
    return cost;
}

vector<int> christofides(vector<vector<double>>& adj, vector<int>& nodes) {
    int m = nodes.size();

    // 1. Floyd-Warshall + extract submatrix
    vector<vector<double>> dist = floyd_warshall(adj, nodes);

    // 2. MST
    vector<pair<int,int>> mst_edges = prim_mst(dist);

    // 3. Find odd-degree vertices
    vector<int> degree(m, 0);
    for (auto [u, v] : mst_edges) {
        degree[u]++;
        degree[v]++;
    }
    vector<int> odds;
    for (int i = 0; i < m; i++)
        if (degree[i] % 2 == 1)
            odds.push_back(i);

    // 4. Greedy matching on odd vertices
    vector<pair<int,int>> matching = perfect_matching(odds, dist);

    // 5. Build multigraph
    vector<vector<pair<int,int>>> multigraph(m);
    int eid = 0;
    for (auto [u, v] : mst_edges) {
        multigraph[u].push_back({v, eid});
        multigraph[v].push_back({u, eid});
        eid++;
    }
    for (auto [u, v] : matching) {
        multigraph[u].push_back({v, eid});
        multigraph[v].push_back({u, eid});
        eid++;
    }

    // 6. Eulerian circuit
    vector<int> circuit = eulerian_circuit(m, multigraph);

    // 7. Shortcut to Hamiltonian cycle
    vector<bool> visited(m, false);
    vector<int> tour;
    for (int node : circuit) {
        if (!visited[node]) {
            visited[node] = true;
            tour.push_back(node);
        }
    }
    tour.push_back(tour[0]); // return to start

    return tour;
}