// Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых
// чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс
// элемента массива A[k], ближайшего по значению к B[i].

#include <cassert>
#include <iostream>
#include <sstream>

int binary_search(int start, int end, int elem, int *A) {
  while (start - end > 1) {
    int delta = (end - start) / 2;
    int m1 = start + delta;
    int m2 = end - delta;
    if (A[m1] <= elem) {
      start = m1;
    } else {
      end = m2;
    }
  }
  //случай, если серединой отсекся правильный ответ
  return abs(A[start] - elem) > abs(elem - A[end]) ? end : start;
}
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
  return binary_search(start, end, elem, A);
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
  int *result = new int[m];
  for (int i = 0; i < m; ++i) {
    result[i] = find_min_index_of_closer_for_elem(A, B[i], n);
  }
  for (int i = 0; i < m; ++i) {
    output << result[i] << " ";
  }
  delete[] A;
  delete[] B;
  delete[] result;
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

}

int main() {
   // run(std::cin, std::cout);
  test();
  return 0;
}
