#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "json.hpp"
#include "TSP.h"

class Graph {
public:
    int n;
    vector<vector<double>> adj;

    Graph(const nlohmann::json& graph_json) {
        n = graph_json["nodes"].size();
        adj.assign(n, vector<double>(n, INF));
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
        if (string(event["type"]) != "tsp") continue;

        vector<int> nodes;
        for (int nd : event["nodes"]) nodes.push_back(nd);

        // Christofides modifies adj via Floyd-Warshall, so pass a copy
        vector<vector<double>> adj_copy = map.adj;

        auto t0 = chrono::high_resolution_clock::now();
        vector<int> local_tour = christofides(adj_copy, nodes);
        auto t1 = chrono::high_resolution_clock::now();
        double time_us = chrono::duration<double, micro>(t1 - t0).count();

        // Map local indices back to original node IDs
        vector<int> tour;
        for (int idx : local_tour) tour.push_back(nodes[idx]);

        // Compute cost using the FW-updated adj_copy
        double cost = 0.0;
        for (int i = 0; i + 1 < (int)tour.size(); i++)
            cost += adj_copy[tour[i]][tour[i+1]];

        nlohmann::json out;
        out["id"]      = event["id"];
        out["tour"]    = tour;
        out["cost"]    = cost;
        out["time_us"] = time_us;
        output_json["results"].push_back(out);
    }

    ofstream out_file(argv[3]);
    if (!out_file.is_open()) { cerr << "Error: Could not open " << argv[3] << '\n'; return 1; }
    out_file << output_json.dump(4) << '\n';
    cout << "Output written to " << argv[3] << '\n';
    return 0;
}