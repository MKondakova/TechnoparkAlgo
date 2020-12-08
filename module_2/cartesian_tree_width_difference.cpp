#include <assert.h>

#include <iostream>
#include <queue>
#include <sstream>
#include <stack>

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
    struct CTreeNode {
        CTreeNode *Left;
        CTreeNode *Right;
        P Priority;
        K Key;
        CTreeNode(P priority, K key)
            : Left(nullptr), Right(nullptr), Priority(priority), Key(key){};
    };
    CTreeNode *root;
    void postOrderDFS(CTreeNode *node, void visit(CTreeNode *));
    void add(P priority, K key, CTreeNode *&node);
    void Split(CTreeNode *currentNode, K key, CTreeNode *&left,
               CTreeNode *&right);
    void Merge(CTree::CTreeNode *left, CTree::CTreeNode *right,
               CTree::CTreeNode *&result);
};

template <class P, class K>
void CTree<P, K>::postOrderDFS(CTree::CTreeNode *node,
                               void (*visit)(CTreeNode *)) {
    if (node == nullptr) {
        return;
    }
    std::stack<CTreeNode *> visitStack;
    CTreeNode *currentNode = node;
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

template <class T, class K>
void CTree<T, K>::Split(CTree::CTreeNode *currentNode, K key,  // todo
                        CTree::CTreeNode *&left, CTree::CTreeNode *&right) {
    if (currentNode == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
}

template <class P, class K>
void CTree<P, K>::Merge(CTree::CTreeNode *left, CTree::CTreeNode *right,
                        CTree::CTreeNode *&result) {
    CTreeNode **current_root;
    current_root = result;  //вот тут не нравится момент с передачей по ссылке
    while (right != nullptr && left != nullptr) {
        if (compareFuncForPriorities_(right->Priority, left->Priority)) {
            *current_root = left;
            current_root = &(left->Right);
            left = left->Right;
        } else {
            *current_root = right;
            current_root = &(right->Left);
            right = right->Left;
        }
    }
    if (left == nullptr) *current_root = right;
    if (right == nullptr) *current_root = left;
}

template <class P, class K>
CTree<P, K>::~CTree() {
    postOrderDFS(root, [](CTreeNode *node) { delete node; });
}

template <class P, class K>
void CTree<P, K>::Add(P priority, K key) {
    add(priority, key, root);
}

template <class P, class K>
void CTree<P, K>::add(P priority, K key, CTree::CTreeNode *&node) {
    if (node == nullptr) {
        node = new CTreeNode(priority, key);
        return;
    }
    CTreeNode *parent = nullptr;
    CTreeNode *currentNode = node;
    while (currentNode != nullptr &&
           !compareFuncForPriorities_(currentNode->Priority, priority)) {
        parent = currentNode;
        if (compareFuncForKeys_(currentNode->Key, key)) {
            currentNode = currentNode->Right;
        } else {
            currentNode = currentNode->Left;
        }
    }
    CTreeNode *left, *right;
    Split(currentNode, key, left, right);
    CTreeNode *newNode = new CTreeNode(priority, key);
    newNode->Left = left;
    newNode->Right = right;
    if (compareFuncForKeys_(parent->Key, key)) {
        parent->Right = newNode;
    } else {
        parent->Left = newNode;
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
        input << "2\n"
                 "5 11\n"
                 "18 8";
        run(input, output);
        assert(output.str() == "0");
    }
    /*{
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
    }*/
}

int main() {
    test();
    return 0;
}
