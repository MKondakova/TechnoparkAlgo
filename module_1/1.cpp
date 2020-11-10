/**“Считалочка”. В круг выстроено N человек, пронумерованных числами от 1 до N.
 * Будем исключать каждого k-ого до тех пор, пока не уцелеет только один
 * человек. (Например, если N=10, k=3, то сначала умрет 3-й, потом 6-й, затем
 * 9-й, затем 2-й, затем 7-й, потом 1-й, потом 8-й, за ним - 5-й, и потом 10-й.
 * Таким образом, уцелеет 4-й.) Необходимо определить номер уцелевшего. N, k ≤
 * 10000.
 */

#include <assert.h>

#include <iostream>
#include <sstream>

struct Node {
    explicit Node(int i) {
        position = i;
        next = nullptr;
    }
    int position;
    Node *next;
};
int countdown(int N, int k) {
    Node *list = new Node(1);
    Node *tail = list;
    for (int i = 2; i <= N; ++i) {
        Node *temp = new Node(i);
        tail->next = temp;
        tail = temp;
    }
    tail->next = list;

    int aliveCount = N;
    Node *current = list;
    while (aliveCount > 1) {
        for (int i = 1; i < k - 1; ++i) {
            current = current->next;
        }
        Node *temp = current->next;
        current->next = current->next->next;
        current = current->next;
        delete temp;
        --aliveCount;
    }
    int result = current->position;
    delete current;
    return result;
}

void run(std::istream &input, std::ostream &output) {
    int N = 0;
    input >> N;
    int k = 0;
    input >> k;
    output << countdown(N, k);
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 3";
        run(input, output);
        assert(output.str() == "4");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 5";
        run(input, output);
        assert(output.str() == "2");
    }
}

int main() {
    run(std::cin, std::cout);
    // test();
    return 0;
}