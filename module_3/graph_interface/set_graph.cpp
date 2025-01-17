#include "set_graph.h"
SetGraph::SetGraph(int vertex_number) : adjacency_lists(vertex_number) {}

SetGraph::SetGraph(const IGraph& graph)
    : adjacency_lists(graph.VerticesCount()) {
    int vertex_count = graph.VerticesCount();
    for (int i = 0; i < vertex_count; ++i) {
        std::vector<int> next_vertices = graph.GetNextVertices(i);
        for (int vertex : next_vertices) {
            adjacency_lists[i].insert(vertex);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjacency_lists.size());
    assert(to >= 0 && to < adjacency_lists.size());
    adjacency_lists[from].insert(to);
}

int SetGraph::VerticesCount() const { return adjacency_lists.size(); }

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacency_lists.size());
    return std::vector<int>(adjacency_lists[vertex].begin(),
                            adjacency_lists[vertex].end());
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacency_lists.size());
    std::vector<int> prev_vertices;
    for (int i = 0; i < adjacency_lists.size(); ++i) {
        if (adjacency_lists[i].find(vertex) != adjacency_lists[i].end())
            prev_vertices.push_back(i);
    }
    return prev_vertices;
}
