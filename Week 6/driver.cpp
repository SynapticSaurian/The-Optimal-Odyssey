#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>
#include "json.hpp"
#include "SA.h"

class Graph {
public:
    int n;
    vector<vector<double>> adj;
    Graph(const nlohmann::json& graph_json) {
        n = graph_json["nodes"].size();
        adj.assign(n, vector<double>(n, SA_INF));
        for (int i = 0; i < n; i++) adj[i][i] = 0.0;
        for (const auto& edge : graph_json["edges"]) {
            int u = edge["u"], v = edge["v"];
            double w = edge["w"];
            adj[u][v] = min(adj[u][v], w);
            adj[v][u] = min(adj[v][u], w);
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: ./{executable} <graph.json> <queries.json> <output.json>\n";
        return 1;
    }

    ifstream file1(argv[1]);
    if (!file1.is_open()) { cerr << "Error: Could not open " << argv[1] << '\n'; return 1; }
    nlohmann::json graph_json;
    file1 >> graph_json;
    Graph map(graph_json);

    ifstream file2(argv[2]);
    if (!file2.is_open()) { cerr << "Error: Could not open " << argv[2] << '\n'; return 1; }
    nlohmann::json query_json;
    file2 >> query_json;

    nlohmann::json output_json;
    output_json["meta"]    = {{"id", query_json["meta"]["id"]}};
    output_json["results"] = nlohmann::json::array();

    for (auto& event : query_json["events"]) {
        string type = event["type"];
        if (type != "tsp") continue;

        vector<int> nodes = event["nodes"].get<vector<int>>();

        // Build distance submatrix via Floyd-Warshall
        vector<vector<double>> adj_copy = map.adj;
        vector<vector<double>> dist = fw_submatrix(adj_copy, nodes);

        // Run SA
        auto t0 = chrono::high_resolution_clock::now();
        vector<int> local_tour = simulated_annealing(dist);
        auto t1 = chrono::high_resolution_clock::now();
        double time_us = chrono::duration<double, micro>(t1 - t0).count();

        // Map local indices back to original node IDs
        vector<int> tour;
        for (int i : local_tour) tour.push_back(nodes[i]);

        double cost = tour_cost(local_tour, dist);

        nlohmann::json out;
        out["id"] = event["id"];
        out["simulated_annealing"] = {
            {"cost",    cost},
            {"tour",    tour},
            {"time_us", time_us}
        };
        output_json["results"].push_back(out);
    }

    ofstream fout(argv[3]);
    if (!fout.is_open()) { cerr << "Error: Could not open " << argv[3] << '\n'; return 1; }
    fout << output_json.dump(4) << '\n';
    cout << "Output written to " << argv[3] << '\n';
    return 0;
}
