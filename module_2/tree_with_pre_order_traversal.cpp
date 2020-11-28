/**
 * Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root,
 * если root→Key ≤ K, то узел K добавляется в правое поддерево root;
 * иначе в левое поддерево root.
 * Требования: Рекурсия запрещена. Решение должно поддерживать передачу
 * функции сравнения снаружи.
 * Выведите элементы в порядке pre-order (сверху вниз).
 */

#include <assert.h>

#include <iostream>
#include <sstream>
#include <stack>

template <class T>
class BinTree {
   public:
    explicit BinTree(bool (*compareFunc)(const T& a, const T& b));
    ~BinTree();
    BinTree(const BinTree&) = delete;
    void Add(int key);
    void PreOrderDFS(void visit(T));

   private:
    struct Node {
        Node* Left;
        Node* Right;
        T Key;

        explicit Node(T key) : Left(nullptr), Right(nullptr), Key(key) {}
    };
    Node* root;
    bool (*compare)(const T& a, const T& b);

    void add(Node*& node, T key);
    void preOrderDFS(Node* node, void visit(T)) const;
    void postOrderDFS(Node* node, void visit(Node*));
};


template <class T>
BinTree<T>::BinTree(bool (*compareFunc)(const T&, const T&))
    : root(nullptr), compare(compareFunc) {}


template <class T>
void BinTree<T>::Add(int key) {
    add(root, key);
}


template <class T>
void BinTree<T>::add(BinTree::Node*& node, T key) {
    if (node == nullptr) {
        node = new Node(key);
        return;
    }
    Node** currentNode = &node;
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
void BinTree<T>::PreOrderDFS(void (*visit)(T)) {
    preOrderDFS(root, visit);
}


template <class T>
void BinTree<T>::preOrderDFS(BinTree::Node* node, void (*visit)(T)) const {
    if (node == nullptr) {
        return;
    }
    std::stack<Node*> visitStack;
    Node* currentNode;
    visitStack.push(node);
    while (!visitStack.empty()) {
        currentNode = visitStack.top();
        visitStack.pop();
        visit(currentNode->Key);
        if (currentNode->Right != nullptr) {
            visitStack.push(currentNode->Right);
        }
        if (currentNode->Left != nullptr) {
            visitStack.push(currentNode->Left);
        }
    }
}


template <class T>
void BinTree<T>::postOrderDFS(BinTree::Node* node, void (*visit)(Node*)) {
    if (node == nullptr) {
        return;
    }
    std::stack<Node*> visitStack;
    Node* currentNode = node;
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
    postOrderDFS(root, [](Node* node) { delete node; });
}


template <class T>
bool defaultCompare(const T& a, const T& b) {
    return a < b;
}


std::stringstream mainOutput;
void stringstreamOutput(int value) { mainOutput << value << " "; }


void run(std::istream& input, void (*visit)(int) = stringstreamOutput) {
    int n = 0;
    input >> n;
    BinTree<int> tree(defaultCompare);
    for (int i = 0; i < n; ++i) {
        int key;
        input >> key;
        tree.Add(key);
    }
    tree.PreOrderDFS(visit);
}


void test() {
    {
        std::stringstream input;
        input << "3\n"
                 "2 1 3";
        run(input);
        assert(mainOutput.str() == "2 1 3 ");
        mainOutput.str(std::string());
    }
    {
        std::stringstream input;
        input << "3\n"
                 "1 2 3";
        run(input);
        assert(mainOutput.str() == "1 2 3 ");
        mainOutput.str(std::string());
    }
    {
        std::stringstream input;
        input << "3\n"
                 "3 1 2";
        run(input);
        assert(mainOutput.str() == "3 1 2 ");
        mainOutput.str(std::string());
    }
    {
        std::stringstream input;
        input << "4\n"
                 "3 1 4 2";
        run(input);
        assert(mainOutput.str() == "3 1 2 4 ");
        mainOutput.str(std::string());
    }
}

int main() {
    run(std::cin, [](int value) { std::cout << value << " "; });
    // test();
    return 0;
}