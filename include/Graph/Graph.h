#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <limits>
#include <memory>
#include <numeric>

template<typename Vertex, typename Distance = double>
class Graph {
public:
    struct Edge {
        Vertex from;
        Vertex to;
        Distance distance;
    };

    bool has_vertex(const Vertex& v) const {
        return adj_list.find(v) != adj_list.end();
    }

    void add_vertex(const Vertex& v) {
        adj_list[v];
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
        adj_list[from].emplace_back(from, to, d);
    }

    bool remove_edge(const Vertex& from, const Vertex& to) {
        if (!has_vertex(from)) return false;
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
        return remove_edge(e.from, e.to);
    }

    bool has_edge(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from)) return false;
        const auto& edges = adj_list.at(from);
        return std::any_of(edges.begin(), edges.end(),
            [&to](const Edge& e) { return e.to == to; });
    }

    bool has_edge(const Edge& e) const {
        return has_edge(e.from, e.to);
    }

    std::vector<Edge> edges(const Vertex& vertex) const {
        if (!has_vertex(vertex)) return {};
        return adj_list.at(vertex);
    }

    size_t order() const {
        return adj_list.size();
    }

    size_t degree(const Vertex& v) const {
        if (!has_vertex(v)) return 0;
        return adj_list.at(v).size();
    }

private:
    std::unordered_map<Vertex, std::vector<Edge>> adj_list;
};