#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "json.hpp"
#include "graph.hpp"
#include "pathfinder.hpp"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./{executable} <graph.json> <queries.json> <output.json>\n";
        return 1;
    }

    std::string graph_json_file = argv[1];
    std::string query_json_file = argv[2];
    std::string output_file     = argv[3];

    // ── Load graph ────────────────────────────────────────────────────────────
    std::ifstream file1(graph_json_file);
    if (!file1.is_open()) {
        std::cerr << "Error: Could not open " << graph_json_file << '\n';
        return 1;
    }
    nlohmann::json graph_json;
    file1 >> graph_json;

    Graph map(graph_json);

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

        if (type == "find_path") {
            Node start = {event["start"]["y"], event["start"]["x"]};
            Node goal  = {event["goal"]["y"],  event["goal"]["x"]};

            nlohmann::json out;
            out["id"] = event["id"];

            // Helper lambda: runs one search, records result + time
            auto run = [&](auto search_fn, const char* key) {
                auto t0 = std::chrono::high_resolution_clock::now();
                SearchResult res = search_fn(map, start, goal);
                auto t1 = std::chrono::high_resolution_clock::now();

                double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

                out[key] = {
                    {"path_found",     res.path_found},
                    {"path_length",    res.path_length},
                    {"nodes_explored", res.nodes_explored},
                    {"time_ms",        ms}
                };
            };

            run(dijkstra,         "dijkstra");
            run(astar_euclidean,  "astar_euclidean");
            run(astar_manhattan,  "astar_manhattan");

            output_json["results"].push_back(out);
        }
        // Add other event type handlers here if needed
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
