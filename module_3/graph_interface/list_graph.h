#include <assert.h>

#include <vector>

#include "graph_interface.h"

class ListGraph : public IGraph {
   public:
    explicit ListGraph(int vertex_number);
    explicit ListGraph(const IGraph& graph);

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

   private:
    std::vector<std::vector<int>> adjacency_lists;
};