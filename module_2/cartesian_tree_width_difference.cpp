/** Дано число N < 106 и последовательность пар целых чисел из [-231..231]
 * длиной N. Построить декартово дерево из N узлов, характеризующихся парами
 * чисел {Xi, Yi}. Каждая пара чисел {Xi, Yi} определяет ключ Xi и приоритет Yi
 * в декартовом дереве.
 *
 *Вычислить количество узлов в самом широком слое декартового дерева и
 *количество узлов в самом широком слое наивного дерева поиска. Вывести их
 *разницу. Разница может быть отрицательна.
 *
 */
#include <assert.h>

#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
template <class P, class K>
struct CTreeNode {
    CTreeNode *Left;
    CTreeNode *Right;
    P Priority;
    K Key;
    CTreeNode(P priority, K key)
        : Left(nullptr), Right(nullptr), Priority(priority), Key(key){};
};

template <class P, class K>
class CTree {
   public:
    CTree(bool (*compareFuncForKeys)(const K &a, const K &b),
          bool (*compareFuncForPriorities)(const P &a, const P &b));
    ~CTree();
    CTree(const CTree &) = delete;
    void Add(P priority, K key);
    int MaxWidth() const;

   private:
    bool (*compareFuncForKeys_)(const K &a, const K &b);
    bool (*compareFuncForPriorities_)(const P &a, const P &b);

    CTreeNode<P, K> *root;
    void postOrderDFS(CTreeNode<P, K> *node, void visit(CTreeNode<P, K> *));
    void add(P priority, K key, CTreeNode<P, K> *&node);
    void Split(CTreeNode<P, K> *currentNode, K key, CTreeNode<P, K> *&left,
               CTreeNode<P, K> *&right);
    CTreeNode<P, K> Merge(CTreeNode<P, K> *left, CTreeNode<P, K> *right);
};

template <class P, class K>
void CTree<P, K>::postOrderDFS(CTreeNode<P, K> *node,
                               void (*visit)(CTreeNode<P, K> *)) {
    if (node == nullptr) {
        return;
    }
    std::stack<CTreeNode<P, K>*> visitStack;
    CTreeNode<P, K> *currentNode = node;
    do {
        while (currentNode != nullptr) {
            if (currentNode->Right != nullptr) {
                visitStack.push(currentNode->Right);
            }
            visitStack.push(currentNode);
            currentNode = currentNode->Left;
        }
        currentNode = visitStack.top();
        visitStack.pop();
        if (!visitStack.empty() && currentNode->Right == visitStack.top()) {
            visitStack.pop();
            visitStack.push(currentNode);
            currentNode = currentNode->Right;
        } else {
            visit(currentNode);
            currentNode = nullptr;
        }
    } while (!visitStack.empty());
}

template <class P, class K>
void CTree<P, K>::Split(CTreeNode<P, K> *currentNode, K key,
                        CTreeNode<P, K> *&left, CTreeNode<P, K> *&right) {
    if (!currentNode) {
        left = nullptr;
        right = nullptr;
    } else if (!compareFuncForKeys_(key, currentNode->Key)) {
        Split(currentNode->Right, key, currentNode->Right, right);
        left = currentNode;
    } else {
        Split(currentNode->Left, key, left, currentNode->Left);
        right = currentNode;
    }
}

template <class P, class K>
CTreeNode<P, K> CTree<P, K>::Merge(CTreeNode<P, K> *left,
                                   CTreeNode<P, K> *right) {
    if (left == nullptr) return right;
    if (right == nullptr) return left;
    if (compareFuncForPriorities_(right->Priority, left->Priority)) {
        left->Right = Merge(left->Right, right);
        return left;
    } else {
        right->Left = Merge(left, right->Left);
        return right;
    }
}

template <class P, class K>
CTree<P, K>::~CTree() {
    postOrderDFS(root, [](CTreeNode<P, K> *node) { delete node; });
}

template <class P, class K>
void CTree<P, K>::Add(P priority, K key) {
    add(priority, key, root);
}

template <class P, class K>
void CTree<P, K>::add(P priority, K key, CTreeNode<P, K> *&node) {
    if (node == nullptr) {
        node = new CTreeNode<P, K>(priority, key);
        return;
    }
    if (compareFuncForPriorities_(node->Priority, priority)) {
        CTreeNode<P, K> *newNode = new CTreeNode<P, K>(priority, key);
        Split(node, key, newNode->Left, newNode->Right);
        node = newNode;
    } else {
        if (compareFuncForKeys_(key, node->Key)) {
            add(priority, key, node->Left);
        } else {
            add(priority, key, node->Right);
        }
    }
}

template <class P, class K>
CTree<P, K>::CTree(bool (*compareFuncForKeys)(const K &, const K &),
                   bool (*compareFuncForPriorities)(const P &, const P &))
    : root(nullptr),
      compareFuncForKeys_(compareFuncForKeys),
      compareFuncForPriorities_(compareFuncForPriorities) {}

template <class T>
int BFSWithMaxWidthCalculation(T *node) {
    if (node == nullptr) return 0;
    std::queue<T *> queue;
    int currentWidth = 1;
    int maxWidth = currentWidth;
    queue.push(node);
    while (!queue.empty()) {
        T *currentNode = queue.front();
        queue.pop();
        --currentWidth;
        if (currentNode->Left != nullptr) {
            queue.push(currentNode->Left);
        }
        if (currentNode->Right != nullptr) {
            queue.push(currentNode->Right);
        }
        if (currentWidth == 0) {
            currentWidth = queue.size();
            if (currentWidth > maxWidth) maxWidth = currentWidth;
        }
    }
    return maxWidth;
}

template <class P, class K>
int CTree<P, K>::MaxWidth() const {
    return BFSWithMaxWidthCalculation(root);
}

template <class T>
class BinTree {
   public:
    explicit BinTree(bool (*compareFunc)(const T &a, const T &b));
    ~BinTree();
    BinTree(const BinTree &) = delete;
    void Add(T key);
    int MaxWidth() const;

   private:
    struct Node {
        Node *Left;
        Node *Right;
        T Key;

        explicit Node(T key) : Left(nullptr), Right(nullptr), Key(key) {}
    };
    Node *root;
    bool (*compare)(const T &a, const T &b);

    void addNode(Node *&node, T key);
    void postOrderDFS(Node *node, void visit(Node *));
    bool deleteNode(Node *&node, T key);
};

void test();
template <class T>
BinTree<T>::BinTree(bool (*compareFunc)(const T &, const T &))
    : root(nullptr), compare(compareFunc) {}

template <class T>
void BinTree<T>::Add(T key) {
    addNode(root, key);
}

template <class T>
void BinTree<T>::addNode(BinTree::Node *&node, T key) {
    if (node == nullptr) {
        node = new Node(key);
        return;
    }
    Node **currentNode = &node;
    do {
        if (compare(key, (*currentNode)->Key)) {
            currentNode = &((*currentNode)->Left);
        } else {
            currentNode = &((*currentNode)->Right);
        }
    } while (*currentNode != nullptr);
    *currentNode = new Node(key);
}

template <class T>
void BinTree<T>::postOrderDFS(BinTree::Node *node, void (*visit)(Node *)) {
    if (node == nullptr) {
        return;
    }
    std::stack<Node *> visitStack;
    Node *currentNode = node;
    do {
        while (currentNode != nullptr) {
            if (currentNode->Right != nullptr) {
                visitStack.push(currentNode->Right);
            }
            visitStack.push(currentNode);
            currentNode = currentNode->Left;
        }
        currentNode = visitStack.top();
        visitStack.pop();
        if (!visitStack.empty() && currentNode->Right == visitStack.top()) {
            visitStack.pop();
            visitStack.push(currentNode);
            currentNode = currentNode->Right;
        } else {
            visit(currentNode);
            currentNode = nullptr;
        }
    } while (!visitStack.empty());
}

template <class T>
BinTree<T>::~BinTree() {
    postOrderDFS(root, [](Node *node) { delete node; });
}
template <class T>
int BinTree<T>::MaxWidth() const {
    return BFSWithMaxWidthCalculation(root);
}

bool defaultCompare(const int &a, const int &b) { return a < b; }

void run(std::istream &input, std::ostream &output) {
    CTree<int, int> Ctree(defaultCompare, defaultCompare);
    BinTree<int> Btree(defaultCompare);
    int n = 0;
    input >> n;
    for (int i = 0; i < n; ++i) {
        int key, priority;
        input >> key >> priority;
        Ctree.Add(priority, key);
        Btree.Add(key);
    }
    output << Ctree.MaxWidth() - Btree.MaxWidth();
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n"
                 "5 11\n"
                 "18 8\n"
                 "25 7\n"
                 "50 12\n"
                 "30 30\n"
                 "15 15\n"
                 "20 10\n"
                 "22 5\n"
                 "40 20\n"
                 "45 9";
        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n"
                 "38 19\n"
                 "37 5\n"
                 "47 15\n"
                 "35 0\n"
                 "12 3\n"
                 "0 42\n"
                 "31 37\n"
                 "21 45\n"
                 "30 26\n"
                 "41 6";
        run(input, output);
        assert(output.str() == "1");
    }
}

int main() {
    run(std::cin, std::cout);
    //test();
    return 0;
}
