# The Optimal Odyssey

This repository contains my solutions to the 6-week guided project on graph algorithms and the Travelling Salesman Problem, implemented in C++.

---

## Structure

```
Week 1/   — BFS, DFS
Week 2/   — Dijkstra, Prim's, Kruskal's, DSU
Week 3/   — A* (Dijkstra / Euclidean / Manhattan heuristics), JSON I/O
Week 4/   — TSP: Brute Force + Held-Karp bitmask DP
Week 5/   — TSP: Christofides algorithm (1.5× approximation)
Week 6/   — TSP: Simulated Annealing with 2-opt
```

Each week contains a `driver.cpp`, algorithm headers, test cases under `Tests/`, and a `visualize.py` for comparing outputs.

---

## Algorithms

### Weeks 1–2: Graph Foundations
BFS/DFS traversal, single-source shortest paths (Dijkstra), and minimum spanning trees (Prim's, Kruskal's) with Union-Find.

### Week 3: A\*
A* on grid graphs with three heuristics. All three run per query and output nodes explored + time, so you can see concretely how much the heuristic prunes the search.

### Week 4: Exact TSP
Two solvers on the same inputs:
- **Brute Force** — enumerate all (n−1)! permutations; correct, impractical past ~11 nodes.
- **Held-Karp** — bitmask DP, O(n² · 2ⁿ); exact up to ~20–22 nodes.

Floyd-Warshall runs first to build a full pairwise distance matrix from the sparse input graph, so neither solver needs to care about connectivity.

### Week 5: Christofides (1.5× Approximation)
For metric TSP (distances satisfy triangle inequality). Pipeline: Floyd-Warshall → MST (Prim's) → identify odd-degree vertices → greedy minimum-weight perfect matching → Eulerian circuit → shortcut to Hamiltonian cycle. Guaranteed ≤ 1.5× optimal.

### Week 6: Simulated Annealing
Metaheuristic that scales to hundreds of nodes. Starts from a nearest-neighbor tour, perturbs with random 2-opt moves, and accepts worse solutions early on with probability exp(−Δ/T) to escape local minima. Temperature decays geometrically each iteration. Quality validated against Week 4/5 exact solvers on small inputs.

---

## Building & Running

```bash
g++ -O3 driver.cpp -o driver
./driver <graph.json> <queries.json> <output.json>
python3 visualize.py reference.json output.json
```

`-O3` is necessary — SA's inner loop runs thousands of iterations per query.

---

## I/O Format

Input: two JSON files — `graph.json` (nodes + weighted edges) and `queries.json` (TSP subsets to solve).  
Output: JSON with cost, tour (node IDs), and runtime in microseconds.

```json
{
  "results": [
    {
      "id": 1,
      "simulated_annealing": {
        "cost": 274.26,
        "tour": [0, 4, 1, 2, 3, 0],
        "time_us": 30568.45
      }
    }
  ]
}
```

---

## Dependencies

- [nlohmann/json](https://github.com/nlohmann/json) — single-header JSON parsing (`json.hpp` included in each week)
- Python 3 + matplotlib — for `visualize.py`
