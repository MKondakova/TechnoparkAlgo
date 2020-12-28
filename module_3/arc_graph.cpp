#include "arc_graph.h"
ArcGraph::ArcGraph(const IGraph& graph) {
    int vertex_count = graph.VerticesCount();
    for (int i = 0; i < vertex_count; ++i) {
        vector next_vertices = graph.GetNextVertices(i);
        for (int vertex : next_vertices) {
            edges.push_back(std::pair<int, int>(i, vertex));
        }
    }
}
void ArcGraph::AddEdge(int from, int to) {
    edges.push_back(std::pair<int, int>(from, to));
}
int ArcGraph::VerticesCount() const {
    std::set<int> vertices;
    for (std::pair<int, int> edge:edges){
        vertices.insert(edge.first);
        vertices.insert(edge.second);
    }
    return vertices.size();
}
std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> next_vertices;
    for( std::pair<int, int> edge: edges){
        if (edge.first == vertex){
            next_vertices.push_back(edge.second);
        }
    }
}
std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> next_vertices;
    for( std::pair<int, int> edge: edges){
        if (edge.second == vertex){
            next_vertices.push_back(edge.first);
        }
    }
}
