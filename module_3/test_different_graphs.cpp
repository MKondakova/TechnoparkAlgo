#include <iostream>
#include <queue>

#include "arc_graph.h"
#include "list_graph.h"
#include "matrix_graph.h"
#include "set_graph.h"
void BFS(const IGraph& graph, int vertex, void (*visit)(int)) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> queue;
    queue.push(vertex);
    visited[vertex] = true;

    while (!queue.empty()) {
        int current_vertex = queue.front();
        queue.pop();
        visit(current_vertex);
        std::vector<int> next_vertices = graph.GetNextVertices(current_vertex);
        for (int next_vertex : next_vertices) {
            if (!visited[next_vertex]) {
                queue.push(next_vertex);
                visited[next_vertex] = true;
            }
        }
    }
}

int main() {
    ListGraph list_graph(5);
    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 3);
    list_graph.AddEdge(0, 4);
    list_graph.AddEdge(1, 4);
    list_graph.AddEdge(4, 2);

    BFS(list_graph, 0, [](int vertex) { std::cout << vertex << ", "; });
    return 0;
}