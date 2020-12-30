/**
 * Задача 2. Количество различных путей (3 балла)
 * Дан невзвешенный неориентированный граф. В графе может быть несколько
 * кратчайших путей между какими-то вершинами. Найдите количество различных
 * кратчайших путей между заданными вершинами. Требования: сложность O(V+E).
 * Формат ввода.
 *  v: кол-во вершин (макс. 50000),
 *  n: кол-во ребер (макс. 200000),
 *  n пар реберных вершин,
 *  пара вершин u, w для запроса.
 * Формат вывода.
 *  Количество кратчайших путей от u к w.
 */
#include <assert.h>

#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

class ListGraph {
   public:
    explicit ListGraph(int vertex_number);

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;

   private:
    std::vector<std::vector<int>> adjacency_lists;
};

ListGraph::ListGraph(int vertex_number) : adjacency_lists(vertex_number) {}

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

int shortestPathsCounter(const ListGraph& graph, int start, int destination) {
    std::vector<int> visits_counter(graph.VerticesCount(), 0);
    std::vector<int> distance(graph.VerticesCount(), 0);
    std::queue<int> queue;
    queue.push(start);
    visits_counter[start] = 1;
    distance[start] = 1;
    int result = 0;
    while (!queue.empty() && result == 0) {
        int current_vertex = queue.front();
        queue.pop();
        if (current_vertex == destination) {
            result = visits_counter[current_vertex];
        } else {
            std::vector<int> next_vertices =
                graph.GetNextVertices(current_vertex);
            for (int next_vertex : next_vertices) {
                if (distance[next_vertex] == 0) {
                    queue.push(next_vertex);
                    distance[next_vertex] = distance[current_vertex] + 1;
                }
                if (distance[next_vertex] > distance[current_vertex]) {
                    visits_counter[next_vertex] +=
                        visits_counter[current_vertex];
                }
            }
        }
    }
    return result;
}

void run(std::istream& input, std::ostream& output) {
    int v = 0, n = 0;
    input >> v >> n;
    ListGraph graph(v);
    int u = 0, w = 0;
    for (int i = 0; i < n; ++i) {
        input >> u >> w;
        graph.AddEdge(u, w);
        graph.AddEdge(w, u);
    }
    input >> u >> w;
    output << shortestPathsCounter(graph, u, w);
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n"
                 "5\n"
                 "0 1\n"
                 "0 2\n"
                 "1 2\n"
                 "1 3\n"
                 "2 3\n"
                 "0 3";
        run(input, output);
        assert(output.str() == "2");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2\n"
                 "1\n"
                 "0 1\n"
                 "0 1";
        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5\n"
                 "5\n"
                 "0 1\n"
                 "0 2\n"
                 "1 3\n"
                 "2 3\n"
                 "3 4\n"
                 "0 4";
        run(input, output);
        assert(output.str() == "2");
    }
}

int main() {
    run(std::cin, std::cout);
    //test();
    return 0;
}