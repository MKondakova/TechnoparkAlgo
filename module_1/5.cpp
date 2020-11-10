/**
 * Во всех задачах данного раздела необходимо реализовать и использовать
 * сортировку слиянием. Решение должно поддерживать передачу функции сравнения
 * снаружи. Общее время работы алгоритма O(n log n). На числовой прямой окрасили
 * N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и
 * Ri). Найти длину окрашенной части числовой прямой.
 *
 *
 */

#include <assert.h>
#include <memory.h>

#include <iostream>
#include <sstream>

template <class T>
bool defaultCompare(const T &a, const T &b) {
    return a < b;
}

template <class T>
void merge(T *firstArray, int firstLen, T *secondArray, int secondLen,
           T *destination, bool (*compareFunc)(const T &a, const T &b)) {
    int firstPos = 0;
    int secondPos = 0;
    for (int j = 0; j < firstLen + secondLen; ++j) {
        if (firstPos < firstLen && secondPos < secondLen) {
            if (compareFunc(firstArray[firstPos], secondArray[secondPos])) {
                destination[j] = firstArray[firstPos];
                ++firstPos;
            } else {
                destination[j] = secondArray[secondPos];
                ++secondPos;
            }
        } else if (firstPos < firstLen) {
            destination[j] = firstArray[firstPos];
            ++firstPos;
        } else if (secondPos < secondLen) {
            destination[j] = secondArray[secondPos];
            ++secondPos;
        }
    }
}

template <class T>
void mergeSort(T *array, int aLen,
               bool (*compareFunc)(const T &a, const T &b) = defaultCompare) {
    if (aLen <= 1) {
        return;
    }
    int firstLen = aLen / 2;
    int secondLen = aLen - firstLen;
    mergeSort(array, firstLen, compareFunc);
    mergeSort(array + firstLen, secondLen, compareFunc);
    T *temp = new T[aLen];
    merge(array, firstLen, array + firstLen, secondLen, temp, compareFunc);
    memcpy(array, temp, sizeof(T) * aLen);
    delete[] temp;
}

struct thicknessChangePoint {
    int value;
    int position;
    thicknessChangePoint() {
        value = -1;
        position = -1;
    }
    thicknessChangePoint(int _value, int _position) {
        value = _value;
        position = _position;
    }
};

bool pointCompare(const thicknessChangePoint &a,
                  const thicknessChangePoint &b) {
    return a.position < b.position;
}

void run(std::istream &input, std::ostream &output) {
    int n = 0;
    input >> n;
    thicknessChangePoint *A = new thicknessChangePoint[2 * n];
    for (int i = 0; i < n; ++i) {
        int position;
        input >> position;
        A[2 * i] = thicknessChangePoint(1, position);
        input >> position;
        A[2 * i + 1] = thicknessChangePoint(-1, position);
    }
    mergeSort(A, 2 * n, pointCompare);
    int lenOfPaintedPart = 0;
    int state = A[0].value;
    for (int i = 1; i < 2 * n; ++i) {
        if (state != 0) {
            lenOfPaintedPart += A[i].position - A[i - 1].position;
        }
        state += A[i].value;
    }
    output << lenOfPaintedPart;
    delete[] A;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n"
                 "1 4\n"
                 "7 8\n"
                 "2 5";
        run(input, output);
        assert(output.str() == "5");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n"
                 "1 1\n"
                 "2 3\n"
                 "2 5";
        run(input, output);
        assert(output.str() == "3");
    }
}

int main() {
    run(std::cin, std::cout);
    // test();
    return 0;
}