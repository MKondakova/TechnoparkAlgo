#include "list_graph.h"

ListGraph::ListGraph(int vertex_number) : adjacency_lists(vertex_number) {}

ListGraph::ListGraph(const IGraph& graph) {
    int vertex_count = graph.VerticesCount();
    adjacency_lists(vertex_number);
    for (int i = 0; i < vertex_count; ++i) {
        vector next_vertices = graph.GetNextVertices(i);
        for (int vertex : next_vertices) {
            adjacency_lists[i].push_back(vertex);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjacency_lists.size());
    assert(to >= 0 && to < adjacency_lists.size());
    adjacency_lists[from].push_back(to);
}

int ListGraph::VerticesCount() const { return adjacency_lists.size(); }

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacency_lists.size());
    return adjacency_lists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacency_lists.size());
    std::vector<int> prev_vertices;
    for (int i = 0; i < adjacency_lists.size(); ++i) {
        for (int j = 0; j < adjacency_lists[i].size(); ++j) {
            if (vertex == adjacency_lists[i][j]) {
                prev_vertices.push_back(i);
            }
        }
    }
    return prev_vertices;
}
