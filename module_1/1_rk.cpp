/*Дан отсортированный по возрастанию массив попарно различных целых чисел
   A[0..n-1]. На вход также подается некоторое целое число k. Необходимо вернуть
   индекс элемента k в массиве A, если он там присутствует, либо вернуть
   позицию, куда его следовало бы вставить, чтобы массив остался
   отсортированным.
*/

#include <cassert>
#include <iostream>
#include <sstream>

int binary_search(int start, int end, int elem, int *A) {

    while (end - start > 1) {
        int middle = start + (end - start) / 2;
        if (A[middle] <= elem) {
            start = middle;
        } else {
            end = middle;
        }
    }
    return start;
}

int find_position(int *A, int elem, int n) {
    int pos = binary_search(0, n, elem, A);
    if (elem > A[pos]) {
        return pos + 1;
    }
    return pos;
}
void run(std::istream &input, std::ostream &output) {
    int n = 0;
    input >> n;
    int *A = new int[n];
    for (int i = 0; i < n; ++i) {
        input >> A[i];
    }
    int k = 0;
    input >> k;
    output << find_position(A, k, n);
    delete[] A;
}
void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n"
                 "1 3 5 6\n"
                 "5";
        run(input, output);
        assert(output.str() == "2");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n"
                 "1 3 5 6\n"
                 "2";
        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n"
                 "1 3 5 6\n"
                 "0";
        run(input, output);
        assert(output.str() == "0");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n"
                 "1 3 5 6\n"
                 "8";
        run(input, output);
        assert(output.str() == "4");
    }
}

int main() {
    run(std::cin, std::cout);
    // test();
    return 0;
}
