#include "arc_graph.h"

ArcGraph::ArcGraph(const IGraph& graph) : vertex_count(graph.VerticesCount()) {
    for (int i = 0; i < vertex_count; ++i) {
        std::vector<int> next_vertices = graph.GetNextVertices(i);
        for (int vertex : next_vertices) {
            edges.emplace_back(i, vertex);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) { edges.emplace_back(from, to); }

int ArcGraph::VerticesCount() const { return vertex_count; }

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> next_vertices;
    for (std::pair<int, int> edge : edges) {
        if (edge.first == vertex) {
            next_vertices.push_back(edge.second);
        }
    }
    return next_vertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prev_vertices;
    for (std::pair<int, int> edge : edges) {
        if (edge.second == vertex) {
            prev_vertices.push_back(edge.first);
        }
    }
    return prev_vertices;
}
