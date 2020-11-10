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

int countdown(int N, int k) {
    int aliveCount = N;
    int counted = 0;
    int *array = new int[N];
    const int DEAD = -1;
    const int ALIVE = 1;
    for (int i = 0; i < N; ++i) {
        array[i] = ALIVE;
    }
    for (int i = 0; aliveCount > 1; i = (i + 1) % N) {
        if (array[i] == ALIVE) {
            if (counted == k - 1) {
                array[i] = DEAD;
                counted = 0;
                --aliveCount;
            } else {
                ++counted;
            }
        }
    }
    int result = 0;
    for (; array[result] != ALIVE; ++result)
        ;
    delete[] array;
    return result + 1;
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