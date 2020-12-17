#include <iostream>
struct Node {
    Node *a;
    int key;
};
Node *func(Node *& a1) {
        Node *minNode = a1;
        a1 = a1->a;
        return  minNode;
}
int main() {
    Node a2 = {nullptr, 2};
    Node a1 = {&a2, 1};
    Node *s = &a1;
    Node *res = func(s);
    std::cout << res->key << " " << s->key << a1.key;
}