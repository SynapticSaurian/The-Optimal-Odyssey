#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "json.hpp"
#include "tsp_graph.hpp"
#include "tsp_solver.hpp"

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: ./{executable} <graph.json> <queries.json> <output.json> <brute|hk>\n";
        return 1;
    }

    std::string graph_json_file = argv[1];
    std::string query_json_file = argv[2];
    std::string output_file     = argv[3];
    std::string solver          = argv[4]; // "brute" or "hk"

    // ── Load graph ────────────────────────────────────────────────────────────
    std::ifstream file1(graph_json_file);
    if (!file1.is_open()) {
        std::cerr << "Error: Could not open " << graph_json_file << '\n';
        return 1;
    }
    nlohmann::json graph_json;
    file1 >> graph_json;

    TSPGraph map(graph_json);

    // ── Load queries ──────────────────────────────────────────────────────────
    std::ifstream file2(query_json_file);
    if (!file2.is_open()) {
        std::cerr << "Error: Could not open " << query_json_file << '\n';
        return 1;
    }
    nlohmann::json query_json;
    file2 >> query_json;

    // ── Prepare output ────────────────────────────────────────────────────────
    nlohmann::json output_json;
    output_json["meta"]    = {{"id", query_json["meta"]["id"]}};
    output_json["results"] = nlohmann::json::array();

    // ── Process events ────────────────────────────────────────────────────────
    for (auto& event : query_json["events"]) {
        std::string type = event["type"];

        if (type == "tsp") {
            std::vector<int> nodes;
            for (int node : event["nodes"]) nodes.push_back(node);

            TSPResult result;
            if (solver == "brute")
                result = tsp_brute_force(nodes, map.dist);
            else
                result = tsp_held_karp(nodes, map.dist);

            nlohmann::json out;
            out["query_id"] = event["id"];
            out["cost"]     = result.optimal_cost;
            out["tour"]     = result.tour;

            output_json["results"].push_back(out);
        }
    }

    // ── Write output ──────────────────────────────────────────────────────────
    std::ofstream out_file(output_file);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open " << output_file << " for writing\n";
        return 1;
    }
    out_file << output_json.dump(4) << '\n';
    std::cout << "Output written to " << output_file << '\n';

    return 0;
}
