/**
 * Задача 1. «Представление графа».(5 баллов)
 * Обязательная задача
 * Дан базовый интерфейс для представления ориентированного графа:
 * struct IGraph2 {
 *  virtual ~IGraph2() {}
 *  virtual void AddEdge(int from, int to) = 0;
 *  virtual int VerticesCount() const  = 0;
 *  virtual std::vector<int> GetNextVertices(int vertex) const = 0;
 *  virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
 * };
 * Необходимо написать несколько реализаций интерфейса:
 *  ListGraph, хранящий граф в виде массива списков смежности,
 *  MatrixGraph, хранящий граф в виде матрицы смежности,
 *  SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев
 * поиска, ArcGraph, хранящий граф в виде одного массива пар {from, to}. Также
 * необходимо реализовать конструктор, принимающий const IGraph&. Такой
 * конструктор должен скопировать переданный граф в создаваемый объект.
 * Для каждого класса создавайте отдельные h и cpp файлы.
 * Число вершин графа задается в конструкторе каждой реализации.
 */

#include <iostream>
#include <queue>

#include "graph_interface/arc_graph.h"
#include "graph_interface/list_graph.h"
#include "graph_interface/matrix_graph.h"
#include "graph_interface/set_graph.h"

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
    ListGraph list_graph(7);
    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 3);
    list_graph.AddEdge(0, 4);
    list_graph.AddEdge(1, 4);
    list_graph.AddEdge(4, 2);
    list_graph.AddEdge(3, 5);
    list_graph.AddEdge(5, 6);
    ArcGraph arc_graph(list_graph);
    SetGraph set_graph(arc_graph);
    MatrixGraph matrix_graph(set_graph);
    ListGraph another_list_graph(matrix_graph);

    BFS(list_graph, 0, [](int vertex) { std::cout << vertex << ", "; });
    std::cout << std::endl;
    BFS(arc_graph, 0, [](int vertex) { std::cout << vertex << ", "; });
    std::cout << std::endl;
    BFS(set_graph, 0, [](int vertex) { std::cout << vertex << ", "; });
    std::cout << std::endl;
    BFS(matrix_graph, 0, [](int vertex) { std::cout << vertex << ", "; });
    std::cout << std::endl;
    BFS(another_list_graph, 0, [](int vertex) { std::cout << vertex << ", "; });
    return 0;
}