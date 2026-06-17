#pragma once
#include <vector>
#include <unordered_set>
#include "json.hpp"

struct Node {
    int y, x;
    bool operator==(const Node& o) const { return y == o.y && x == o.x; }
    bool operator!=(const Node& o) const { return !(*this == o); }
    bool operator<(const Node& o)  const { return y != o.y ? y < o.y : x < o.x; }
};

// Hash for Node so it can be used in unordered_set/map
struct NodeHash {
    std::size_t operator()(const Node& n) const {
        return std::hash<int>()(n.y) ^ (std::hash<int>()(n.x) << 16);
    }
};

class Graph {
public:
    int rows, cols;
    std::unordered_set<int> obstacles; // encoded as y * cols + x

    Graph(const nlohmann::json& graph_json) {
        rows = graph_json["grid_size"]["rows"];
        cols = graph_json["grid_size"]["cols"];
        for (const auto& obs : graph_json["obstacles"]) {
            int y = obs["y"], x = obs["x"];
            obstacles.insert(encode(y, x));
        }
    }

    bool in_bounds(int y, int x) const {
        return y >= 0 && y < rows && x >= 0 && x < cols;
    }

    bool is_obstacle(int y, int x) const {
        return obstacles.count(encode(y, x)) > 0;
    }

    bool walkable(int y, int x) const {
        return in_bounds(y, x) && !is_obstacle(y, x);
    }

    // 4-directional neighbors
    std::vector<Node> get_neighbors(Node current) const {
        std::vector<Node> neighbors;
        const int dy[] = {-1, 1, 0, 0};
        const int dx[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; d++) {
            int ny = current.y + dy[d];
            int nx = current.x + dx[d];
            if (walkable(ny, nx)) {
                neighbors.push_back({ny, nx});
            }
        }
        return neighbors;
    }

private:
    int encode(int y, int x) const { return y * cols + x; }
};
