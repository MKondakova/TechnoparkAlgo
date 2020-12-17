/** Решение должно поддерживать передачу функции сравнения снаружи.
 * 4_2. Порядковые статистики. Дано число N и N строк. Каждая строка содержит
 * команду добавления или удаления натуральных чисел, а также запрос на
 * получение k-ой порядковой статистики. Команда добавления числа A задается
 * положительным числом A, команда удаления числа A задается отрицательным
 * числом “-A”.
 * Запрос на получение k-ой порядковой статистики задается числом k.
 * Требования: скорость выполнения запроса - O(log n).
 */

#include <assert.h>

#include <iostream>
#include <sstream>

template <class K>
struct AVLTreeNode {
    AVLTreeNode *left;
    AVLTreeNode *right;
    size_t childrenCount;
    size_t height;

    K key;
    explicit AVLTreeNode(const K value)
        : left(nullptr),
          right(nullptr),
          childrenCount(1),
          height(1),
          key(value){};
};

template <class K>
class AVLTree {
   public:
    explicit AVLTree(bool (*compareFuncForKeys)(const K &a, const K &b));
    ~AVLTree();
    AVLTree(const AVLTree &) = delete;
    void Add(const K &key);
    void Delete(const K &key);
    K GetKStat(size_t k);
    bool Has(const K &key);

   private:
    bool (*compareFuncForKeys_)(const K &a, const K &b);
    AVLTreeNode<K> *root;
    AVLTreeNode<K> *addNode(const K &key, AVLTreeNode<K> *node);
    AVLTreeNode<K> *deleteNode(const K &key, AVLTreeNode<K> *node);
    bool hasNode(const K &key, AVLTreeNode<K> *node);
    K getKStat(size_t k, AVLTreeNode<K> *node);

    size_t getHeight(AVLTreeNode<K> *node) { return !node ? 0 : node->height; }
    size_t getChildrenCount(AVLTreeNode<K> *node) {
        return !node ? 0 : node->childrenCount;
    }
    int balanceFactor(AVLTreeNode<K> *node) {
        return getHeight(node->right) - getHeight(node->left);
    }
    void updateHeight(AVLTreeNode<K> *node);
    void updateChildrenCount(AVLTreeNode<K> *node);
    void deleteTree(AVLTreeNode<K> *node);

    AVLTreeNode<K> *balance(AVLTreeNode<K> *node);
    AVLTreeNode<K> *rotateLeft(AVLTreeNode<K> *node);
    AVLTreeNode<K> *rotateRight(AVLTreeNode<K> *node);
    AVLTreeNode<K> *findAndRemoveMin(AVLTreeNode<K> *&node);
    AVLTreeNode<K> *findAndRemoveMax(AVLTreeNode<K> *&node);
};

template <class K>
AVLTree<K>::AVLTree(bool (*compareFuncForKeys)(const K &, const K &))
    : compareFuncForKeys_(compareFuncForKeys), root(nullptr) {}

template <class K>
AVLTree<K>::~AVLTree() {
    deleteTree(root);
}

template <class K>
void AVLTree<K>::Add(const K &key) {
    root = addNode(key, root);
}

template <class K>
void AVLTree<K>::Delete(const K &key) {
    root = deleteNode(key, root);
}

template <class K>
K AVLTree<K>::GetKStat(size_t k) {
    return getKStat(k, root);
}

template <class K>
bool AVLTree<K>::Has(const K &key) {
    return hasNode(key, root);
}

template <class K>
AVLTreeNode<K> *AVLTree<K>::addNode(const K &key, AVLTreeNode<K> *node) {
    if (!node) {
        return new AVLTreeNode<K>(key);
    }
    if (node->key < key) {
        node->right = addNode(key, node->right);
    } else {
        node->left = addNode(key, node->left);
    }

    return balance(node);
}

template <class K>
AVLTreeNode<K> *AVLTree<K>::deleteNode(const K &key, AVLTreeNode<K> *node) {
    if (!node) {
        return nullptr;
    }
    if (node->key < key) {
        node->right = deleteNode(key, node->right);
    } else if (node->key > key) {
        node->left = deleteNode(key, node->left);
    } else {
        AVLTreeNode<K> *right = node->right;
        AVLTreeNode<K> *left = node->left;
        AVLTreeNode<K> *newNode;
        if (balanceFactor(node) > 0) {
            newNode = findAndRemoveMin(right);
        } else if (balanceFactor(node) <= 0 && left) {
            newNode = findAndRemoveMax(left);
        } else {
            newNode = nullptr;
        }
        delete node;
        if (!newNode) {
            return nullptr;
        }
        newNode->right = right;
        newNode->left = left;
        return balance(newNode);
    }
    return balance(node);
}

template <class K>
bool AVLTree<K>::hasNode(const K &key, AVLTreeNode<K> *node) {
    AVLTreeNode<K> *currentNode = node;
    while (currentNode) {
        if (currentNode->key == key) {
            return true;
        }
        if (currentNode->key < key) {
            currentNode = currentNode->right;
        } else {
            currentNode = currentNode->left;
        }
    }
    return false;
}

template <class K>
K AVLTree<K>::getKStat(size_t k, AVLTreeNode<K> *node) {
    size_t leftSubtreeSize = getChildrenCount(node->left);
    if (leftSubtreeSize > k) {
        return getKStat(k, node->left);
    }
    if (leftSubtreeSize < k) {
        return getKStat(k - (leftSubtreeSize + 1), node->right);
    }
    return node->key;
}

template <class K>
void AVLTree<K>::updateHeight(AVLTreeNode<K> *node) {
    size_t leftHeight = getHeight(node->left);
    size_t rightHeight = getHeight(node->right);
    node->height = (rightHeight > leftHeight ? rightHeight : leftHeight) + 1;
}

template <class K>
void AVLTree<K>::updateChildrenCount(AVLTreeNode<K> *node) {
    size_t leftChildrenCount = getChildrenCount(node->left);
    size_t rightChildrenCount = getChildrenCount(node->right);
    node->childrenCount = leftChildrenCount + rightChildrenCount + 1;
}

template <class K>
void AVLTree<K>::deleteTree(AVLTreeNode<K> *node) {
    if (node) {
        deleteTree(node->right);
        deleteTree(node->left);
        delete node;
    }
}
template <class K>
AVLTreeNode<K> *AVLTree<K>::balance(AVLTreeNode<K> *node) {
    updateHeight(node);
    updateChildrenCount(node);
    if (balanceFactor(node) == 2) {
        if (balanceFactor(node->right) < 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    if (balanceFactor(node) == -2) {
        if (balanceFactor(node->left) > 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    return node;
}
template <class K>
AVLTreeNode<K> *AVLTree<K>::rotateLeft(AVLTreeNode<K> *node) {
    AVLTreeNode<K> *right = node->right;
    node->right = right->left;
    right->left = node;
    updateHeight(node);
    updateHeight(right);
    updateChildrenCount(node);
    updateChildrenCount(right);
    return right;
}

template <class K>
AVLTreeNode<K> *AVLTree<K>::rotateRight(AVLTreeNode<K> *node) {
    AVLTreeNode<K> *left = node->left;
    node->left = left->right;
    left->right = node;
    updateHeight(node);
    updateHeight(left);
    updateChildrenCount(node);
    updateChildrenCount(left);
    return left;
}

template <class K>
AVLTreeNode<K> *AVLTree<K>::findAndRemoveMin(AVLTreeNode<K> *&node) {
    AVLTreeNode<K> *minNode;
    if (!node->left) {
        minNode = node;
        node = node->right;
        return minNode;
    }
    minNode = findAndRemoveMin(node->left);
    node = balance(node);
    return minNode;
}

template <class K>
AVLTreeNode<K> *AVLTree<K>::findAndRemoveMax(AVLTreeNode<K> *&node) {
    AVLTreeNode<K> *maxNode;
    if (!node->right) {
        maxNode = node;
        node = node->left;
        return maxNode;
    }
    maxNode = findAndRemoveMax(node->right);
    node = balance(node);
    return maxNode;
}

bool defaultCompare(const int &a, const int &b) { return a < b; }

void run(std::istream &input, std::ostream &output) {
    AVLTree<int> tree(defaultCompare);
    int N = 0;
    input >> N;

    for (int i = 0; i < N; i++) {
        int value, k;
        input >> value >> k;
        if (value >= 0) {
            tree.Add(value);
        } else {
            tree.Delete(-1 * value);
        }
        output << tree.GetKStat(k) << std::endl;
    }
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 "
                 "40 0\n"
                 "10 1\n"
                 "4 1 \n"
                 "-10 0\n"
                 "50 2";
        run(input, output);
        assert(output.str() ==
               "40\n"
               "40\n"
               "10\n"
               "4\n"
               "50\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n"
                 "1 0\n"
                 "2 0\n"
                 "-1 0";
        run(input, output);
        assert(output.str() == "1\n1\n2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 "
                 "2 0\n"
                 "3 1\n"
                 "1 2 \n"
                 "-1 0\n"
                 "1 0\n"
                 "-3 0";
        run(input, output);
        assert(output.str() ==
               "2\n"
               "3\n"
               "3\n"
               "2\n"
               "1\n"
               "1\n");
    }
}

int main() {
    run(std::cin, std::cout);

    // test();
    return 0;
}
