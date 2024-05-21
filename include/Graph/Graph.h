#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <iterator>
#include <queue>
#include <limits>
#include <map>

template<typename Vertex, typename Distance = double>
class Graph {
public:
    struct Edge {
        Vertex from;
        Vertex to;
        Distance distance;

        bool operator==(const Edge& other) const {
            return from == other.from && to == other.to && distance == other.distance;
        }
    };

    bool has_vertex(const Vertex& v) const {
        return adj_list.find(v) != adj_list.end();
    }

    void add_vertex(const Vertex& v) {
        adj_list.emplace(v, std::vector<Edge>());
    }

    bool remove_vertex(const Vertex& v) {
        if (!has_vertex(v)) return false;
        adj_list.erase(v);
        for (auto& [key, edges] : adj_list) {
            edges.erase(std::remove_if(edges.begin(), edges.end(),
                [&v](const Edge& e) { return e.to == v; }), edges.end());
        }
        return true;
    }

    std::vector<Vertex> vertices() const {
        std::vector<Vertex> verts;
        for (const auto& [key, _] : adj_list) {
            verts.push_back(key);
        }
        return verts;
    }

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
        if (!has_vertex(from)) add_vertex(from);
        if (!has_vertex(to)) add_vertex(to);
        adj_list[from].push_back(Edge{ from, to, d });
    }

    bool remove_edge(const Vertex& from, const Vertex& to) {
        if (!has_vertex(from) || !has_vertex(to)) return false;
        auto& edges = adj_list[from];
        auto it = std::remove_if(edges.begin(), edges.end(),
            [&to](const Edge& e) { return e.to == to; });
        if (it != edges.end()) {
            edges.erase(it, edges.end());
            return true;
        }
        return false;
    }

    bool remove_edge(const Edge& e) {
        if (!has_vertex(e.from)) return false;
        auto& edges = adj_list[e.from];
        auto it = std::remove_if(edges.begin(), edges.end(),
            [&e](const Edge& edge) { return edge == e; });
        if (it != edges.end()) {
            edges.erase(it, edges.end());
            return true;
        }
        return false;
    }

    bool has_edge(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from)) return false;
        const auto& edges = adj_list.at(from);
        return std::any_of(edges.begin(), edges.end(),
            [&to](const Edge& e) { return e.to == to; });
    }

    bool has_edge(const Edge& e) const {
        if (!has_vertex(e.from)) return false;
        const auto& edges = adj_list.at(e.from);
        return std::any_of(edges.begin(), edges.end(),
            [&e](const Edge& edge) { return edge == e; });
    }

    std::vector<Edge> edges(const Vertex& vertex) {
        if (!has_vertex(vertex)) return {};
        return adj_list[vertex];
    }

    size_t order() const {
        return adj_list.size();
    }

    size_t degree(const Vertex& v) const {
        if (!has_vertex(v)) return 0;
        return adj_list.at(v).size();
    }

    std::vector<Vertex> walk(const Vertex& start_vertex) const {
        std::vector<Vertex> traversal;
        if (!has_vertex(start_vertex)) return traversal;

        std::queue<Vertex> q;
        std::unordered_set<Vertex> visited;

        q.push(start_vertex);
        visited.insert(start_vertex);

        while (!q.empty()) {
            Vertex current = q.front();
            q.pop();
            traversal.push_back(current);

            for (const auto& edge : adj_list.at(current)) {
                if (visited.find(edge.to) == visited.end()) {
                    q.push(edge.to);
                    visited.insert(edge.to);
                }
            }
        }

        return traversal;
    }

    std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {

        for (const auto& [vertex, _] : adj_list) {
            if (vertex < 0) {
                throw std::invalid_argument("Graph contains a negative vertex.");
            }
        }

        std::map<Vertex, Distance> distances;
        std::map<Vertex, Vertex> predecessors;
        for (const auto& [vertex, _] : adj_list) {
            distances[vertex] = std::numeric_limits<Distance>::max();
        }
        distances[from] = 0;

        auto compare = [&distances](const Vertex& a, const Vertex& b) {
            return distances[a] > distances[b];
            };
        std::priority_queue<Vertex, std::vector<Vertex>, decltype(compare)> pq(compare);
        pq.push(from);

        while (!pq.empty()) {
            Vertex current = pq.top();
            pq.pop();

            if (current == to) break;

            for (const auto& edge : adj_list.at(current)) {
                Distance new_dist = distances[current] + edge.distance;
                if (new_dist < distances[edge.to]) {
                    distances[edge.to] = new_dist;
                    predecessors[edge.to] = current;
                    pq.push(edge.to);
                }
            }
        }

        std::vector<Edge> path;
        if (distances[to] == std::numeric_limits<Distance>::max()) return path;

        for (Vertex at = to; at != from; at = predecessors[at]) {
            Vertex pred = predecessors[at];
            if (predecessors.find(at) != predecessors.end()) {
                auto it = std::find_if(adj_list.at(pred).begin(), adj_list.at(pred).end(),
                    [&at](const Edge& e) { return e.to == at; });
                if (it != adj_list.at(pred).end()) {
                    path.push_back(*it);
                }
            }
            else {
                break;
            }
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

private:
    std::unordered_map<Vertex, std::vector<Edge>> adj_list;
};

template<typename Vertex, typename Distance>
Vertex find_optimal_warehouse_location(const Graph<Vertex, Distance>& graph) {
    auto vertices = graph.vertices();
    std::unordered_map<Vertex, Distance> total_distances;
    Distance min_average_distance = std::numeric_limits<Distance>::max();
    Vertex optimal_vertex;

    for (const auto& v : vertices) {
        Distance total_distance = 0;
        int count = 0;

        for (const auto& other : vertices) {
            if (v != other) {
                auto path = graph.shortest_path(v, other);
                Distance distance = 0;
                for (const auto& edge : path) {
                    distance += edge.distance;
                }
                total_distance += distance;
                count++;
            }
        }

        Distance average_distance = total_distance / count;
        if (average_distance < min_average_distance) {
            min_average_distance = average_distance;
            optimal_vertex = v;
        }
    }
    return optimal_vertex;
}