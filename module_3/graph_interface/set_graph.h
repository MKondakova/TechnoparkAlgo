#include <unordered_set>

#include "assert.h"
#include "graph_interface.h"
class SetGraph : public IGraph {
   public:
    explicit SetGraph(int vertex_number);
    explicit SetGraph(const IGraph& graph);
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

   private:
    std::vector<std::unordered_set<int>> adjacency_lists;
};