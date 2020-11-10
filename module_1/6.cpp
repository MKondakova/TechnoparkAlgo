/**
 * Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9]
 * размера n. Требуется найти k-ю порядковую статистику. т.е. напечатать число,
 * которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
 *
 * Требования: к дополнительной памяти: O(n). Среднее время работы: O(n). Должна
 * быть отдельно выделенная функция partition. Рекурсия запрещена. Решение
 * должно поддерживать передачу функции сравнения снаружи. Реализуйте стратегию
 * выбора опорного элемента “медиана трёх”.
 * Функцию Partition реализуйте методом
 * прохода двумя итераторами от конца массива к началу.
 */

#include <assert.h>

#include <iostream>
#include <sstream>

template <class T>
void swap(T *a, int firstIndex, int secondIndex) {
    T temp = a[firstIndex];
    a[firstIndex] = a[secondIndex];
    a[secondIndex] = temp;
}

template <class T>
int findPivot(T *a, int left, int right,
              bool (*compareFunc)(const T &a, const T &b)) {
    int middle = (left + right) / 2;
    // swap`ы усложняют манипулирование скоростью
    if (compareFunc(a[right], a[left])) {
        swap(a, right, left);
    }
    if (compareFunc(a[right], a[middle])) {
        swap(a, middle, right);
    } else if (compareFunc(a[middle], a[left])) {
        swap(a, middle, left);
    }
    return middle;
}

template <class T>
int partition(T *a, int left, int right,
              bool (*compareFunc)(const T &a, const T &b)) {
    if (right - left < 1) {
        return left;
    }
    int pivotPosition = findPivot(a, left, right, compareFunc);
    swap(a, left, pivotPosition);
    int i = right;
    int j = i;
    while (j > 0) {
        if (compareFunc(a[left], a[j])) {
            swap(a, i, j);
            i--;
        }
        j--;
    }
    swap(a, left, i);
    return i;
}
template <class T>
bool defaultCompare(const T &a, const T &b) {
    return a < b;
}

template <class T>
T findKStat(T *a, int n, int k,
            bool (*compareFunc)(const T &a, const T &b) = defaultCompare) {
    int left = 0;
    int right = n - 1;
    int part = partition(a, left, right, compareFunc);
    while (part != k) {
        if (k < part) {
            right = part - 1;
        } else {
            left = part + 1;
        }
        part = partition(a, left, right, compareFunc);
    }
    return a[part];
}

void run(std::istream &input, std::ostream &output) {
    int n = 0;
    input >> n;
    int k = 0;
    input >> k;
    int *A = new int[n];
    for (int i = 0; i < n; ++i) {
        input >> A[i];
    }
    output << findKStat(A, n, k);
    delete[] A;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 4\n"
                 "1 2 3 4 5 6 7 8 9 10";
        run(input, output);
        assert(output.str() == "5");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 4\n"
                 "1 2 3 4 5 6 7 8 9 10";
        run(input, output);
        assert(output.str() == "5");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 0\n"
                 "3 6 5 7 2 9 8 10 4 1";
        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 9\n"
                 "0 0 0 0 0 0 0 0 0 1";
        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 1\n"
                 "1 0";
        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1\n"
                 "1 0 5";
        run(input, output);
        assert(output.str() == "1");
    }
}

int main() {
    run(std::cin, std::cout);
    // test();
    return 0;
}