// Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых
// чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс
// элемента массива A[k], ближайшего по значению к B[i].

#include <cassert>
#include <iostream>
#include <sstream>

int find_min_index_of_closer_for_elem(int *A, int elem, int n) {
  int index = 1;
  // ищем диапазон, в котором находится ближайшие по значению к elem числа
  while (index < n && elem > A[index]) {
    index = index * 2;
  }
  int start, end;
  start = index / 2;
  if (index >= n) {
    end = n - 1;
  } else {
    end = index;
  }
  // ищем сам индекс
  while (end - start > 1) {
    int middle = start + (end - start) / 2;
    if (A[middle] <= elem) {
      start = middle;
    } else {
      end = middle;
    }
  }
  return abs(A[start] - elem) > abs(elem - A[end]) ? end : start;
}
void run(std::istream &input, std::ostream &output) {
  int n = 0;
  input >> n;
  int *A = new int[n];
  for (int i = 0; i < n; ++i) {
    input >> A[i];
  }
  int m = 0;
  input >> m;
  int *B = new int[m];
  for (int i = 0; i < m; ++i) {
    input >> B[i];
  }
  for (int i = 0; i < m; ++i) {
    output << find_min_index_of_closer_for_elem(A, B[i], n) << " ";
  }
  delete[] A;
  delete[] B;
}
void test() {
  // один элемент меньше минимального, другой больше максимального
  {
    std::stringstream input;
    std::stringstream output;
    input << "3\n"
             "10 20 30\n"
             "3\n"
             "9 15 35";
    run(input, output);
    assert(output.str() == "0 0 2 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "3\n"
             "10 20 30\n"
             "4\n"
             "8 9 20 32";
    run(input, output);
    assert(output.str() == "0 0 1 2 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "2\n"
             "20 22\n"
             "1\n"
             "21";
    run(input, output);
    assert(output.str() == "0 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "5\n"
             "13 15 27 34 54\n"
             "6\n"
             "2 72 27 30 90 53";
    run(input, output);
    assert(output.str() == "0 4 2 2 4 4 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "6\n"
             "1 15 26 33 54 78\n"
             "7\n"
             "2 72 27 30 90 53 5";
    run(input, output);
    assert(output.str() == "0 5 2 3 5 4 0 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "9\n"
             "33 39 57 60 67 77 78 79 88\n"
             "10\n"
             "79 33 57 88 99 67 60 20 77 79";
    run(input, output);
    assert(output.str() == "7 0 2 8 8 4 3 0 5 7 ");
  }
}

int main() {
  //run(std::cin, std::cout);
  test();
  return 0;
}
