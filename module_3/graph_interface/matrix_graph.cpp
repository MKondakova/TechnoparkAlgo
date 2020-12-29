#include "matrix_graph.h"

MatrixGraph::MatrixGraph(int vertex_number)
    : matrix(vertex_number, std::vector<bool>(vertex_number, false)) {}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < matrix.size());
    assert(to >= 0 && to < matrix.size());
    matrix[from][to] = true;
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < matrix.size());
    std::vector<int> next_vertices;
    for (int i = 0; i < matrix.size(); ++i) {
        if (matrix[vertex][i]) next_vertices.push_back(i);
    }
    return next_vertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < matrix.size());
    std::vector<int> prev_vertices;
    for (int i = 0; i < matrix.size(); ++i) {
        if (matrix[i][vertex]) prev_vertices.push_back(i);
    }
    return prev_vertices;
}

MatrixGraph::MatrixGraph(const IGraph& graph)
    : matrix(graph.VerticesCount(),
             std::vector<bool>(graph.VerticesCount(), false)) {
    int vertex_count = graph.VerticesCount();
    for (int i = 0; i < vertex_count; ++i) {
        std::vector<int> next_vertices = graph.GetNextVertices(i);
        for (int vertex : next_vertices) {
            matrix[i][vertex] = true;
        }
    }
}

int MatrixGraph::VerticesCount() const { return matrix.size(); }
