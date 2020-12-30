/**
 * Задача 3. «Города» (4 балла)
 * Требуется отыскать самый выгодный маршрут между городами.
 * Требования: время работы O((N+M)logN), где N-количество городов, M-известных
 * дорог между ними.
 * Формат входных данных.
 * Первая строка содержит число N – количество городов.
 * Вторая строка содержит число M - количество дорог.
 * Каждая следующая строка содержит описание дороги(откуда, куда, время в пути).
 * Последняя строка содержит маршрут (откуда и куда нужно доехать).
 * Формат выходных данных.
 * Вывести длину самого выгодного маршрута.
 */
#include <assert.h>

#include <iosfwd>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>
#define INT_MAX (2147483647)

class ListGraph {
   public:
    explicit ListGraph(int vertex_number);

    void AddEdge(int from, int to, int weight);
    int VerticesCount() const;
    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const;

   private:
    std::vector<std::vector<std::pair<int, int>>> adjacency_lists;
};

ListGraph::ListGraph(int vertex_number) : adjacency_lists(vertex_number) {}

void ListGraph::AddEdge(int from, int to, int weight) {
    assert(from >= 0 && from < adjacency_lists.size());
    assert(to >= 0 && to < adjacency_lists.size());
    adjacency_lists[from].push_back(std::make_pair(weight, to));
    adjacency_lists[to].push_back(std::make_pair(weight, from));
}

int ListGraph::VerticesCount() const { return adjacency_lists.size(); }

std::vector<std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjacency_lists.size());
    return adjacency_lists[vertex];
}

bool Relax(int u, int v, int weight, std::vector<int>& parent,
           std::vector<int>& distance) {
    if (distance[v] > distance[u] + weight) {
        distance[v] = distance[u] + weight;
        parent[v] = u;
        return true;
    }
    return false;
}

int Dijkstra(const ListGraph& graph, int start, int finish) {
    std::vector<int> distance(graph.VerticesCount(), INT_MAX);
    std::vector<int> parent(graph.VerticesCount(), -1);
    distance[start] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>
        queue;
    queue.push(std::make_pair(0, start));
    while (!queue.empty()) {
        std::pair<int, int> current_vertex = queue.top();
        queue.pop();
        if (distance[current_vertex.second] < current_vertex.first) continue;
        for (std::pair<int, int> next :
             graph.GetNextVertices(current_vertex.second)) {
            if (distance[next.second] == INT_MAX) {
                distance[next.second] =
                    distance[current_vertex.second] + next.first;
                parent[next.second] = current_vertex.second;
                queue.push(std::make_pair(next.first, next.second));
            } else if (Relax(current_vertex.second, next.second, next.first,
                             parent, distance)) {
                queue.push(std::make_pair(distance[next.second], next.second));
            }
        }
    }
    return distance[finish];
}

void run(std::istream& input, std::ostream& output) {
    int n = 0, m = 0;
    input >> n >> m;
    ListGraph graph(n);
    for (int i = 0; i < m; ++i) {
        int from, to, weight;
        input >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }
    int start = 0, end = 0;
    input >> start >> end;
    output << Dijkstra(graph, start, end);
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "6\n"
                 "9\n"
                 "0 3 1\n"
                 "0 4 2\n"
                 "1 2 7\n"
                 "1 3 2\n"
                 "1 4 3\n"
                 "1 5 3\n"
                 "2 5 3\n"
                 "3 4 4\n"
                 "3 5 6\n"
                 "0 2";
        run(input, output);
        assert(output.str() == "9");
    }
}

int main() {
    run(std::cin, std::cout);
    // test();
    return 0;
}