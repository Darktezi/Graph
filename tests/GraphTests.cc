#include <gtest/gtest.h>
#include "Graph/Graph.h"

TEST(GraphTests, AddVertexTest) {
    Graph<int, double> graph;
    graph.add_vertex(1);
    ASSERT_TRUE(graph.has_vertex(1));
}

TEST(GraphTests, RemoveVertexTest) {
    Graph<int, double> graph;

    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.remove_vertex(1);

    ASSERT_FALSE(graph.has_vertex(1));
    ASSERT_TRUE(graph.has_vertex(2));
}

TEST(GraphTests, AddEdgeTest) {
    Graph<char, int> graph;

    graph.add_vertex('A');
    graph.add_vertex('B');
    graph.add_edge('A', 'B', 5);

    ASSERT_TRUE(graph.has_edge('A', 'B'));
}

TEST(GraphTests, RemoveEdgeTest) {
    Graph<char, int> graph;

    graph.add_vertex('A');
    graph.add_vertex('B');
    graph.add_edge('A', 'B', 5);
    graph.remove_edge('A', 'B');

    ASSERT_FALSE(graph.has_edge('A', 'B'));
}

TEST(GraphTests, ShortestPathTest) {
    Graph<std::string, double> graph;

    graph.add_vertex("A");
    graph.add_vertex("B");
    graph.add_vertex("C");
    graph.add_edge("A", "B", 1.0);
    graph.add_edge("B", "C", 2.0);

    auto path = graph.shortest_path("A", "C");

    ASSERT_EQ(path.size(), 2);
    ASSERT_EQ(path[0].from, "A");
    ASSERT_EQ(path[0].to, "B");
    ASSERT_EQ(path[0].distance, 1.0);
    ASSERT_EQ(path[1].from, "B");
    ASSERT_EQ(path[1].to, "C");
    ASSERT_EQ(path[1].distance, 2.0);
}

TEST(GraphTests, OptimalWarehouseLocationTest) {
    Graph<std::string, int> graph;
    graph.add_vertex("A");
    graph.add_vertex("B");
    graph.add_vertex("C");
    graph.add_vertex("D");

    graph.add_edge("A", "B", 1.0);
    graph.add_edge("A", "C", 4.0);
    graph.add_edge("B", "C", 2.0);
    graph.add_edge("C", "D", 1.0);

    std::string warehouse = find_optimal_warehouse_location(graph);
    ASSERT_EQ(warehouse, "C");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}