/*
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
Реализовать дек с динамическим зацикленным буфером.
 */

#include <cassert>
#include <iostream>
#include <sstream>

template <class T> class Deque {
public:
  Deque();
  ~Deque();
  void pushBack(const T &value);
  T popBack();
  void pushFront(const T &value);
  T popFront();
  bool isEmpty() const { return size == 0; }
private:
  int capacity;
  int size;
  T *buffer;
  int head;
  int tail;
  void expand();
};

template <class T>
Deque<T>::Deque() : capacity(0), size(0), head(0), tail(0), buffer(nullptr) {}

template <class T> Deque<T>::~Deque() {
    delete[] buffer;
}

template <class T> void Deque<T>::pushBack(const T &value) {
  if (size == capacity) {
    expand();
  }
  buffer[tail] = value;
  tail = (tail + 1) % capacity;
  ++size;
}
template <class T> void Deque<T>::pushFront(const T &value) {
  if (size == capacity) {
    expand();
  }
  head = (capacity + head - 1) % capacity;
  buffer[head] = value;
  ++size;
}
template <class T> T Deque<T>::popBack() {
  assert(!isEmpty());
  tail = (capacity + tail - 1) % capacity;
  --size;
  return buffer[tail];
}
template <class T> T Deque<T>::popFront() {
  assert(!isEmpty());
  T value = buffer[head];
  head = (head + 1) % capacity;
  --size;
  return value;
}
template <class T> void Deque<T>::expand() {
  const int MIN_CAPACITY = 8;
  int newCapacity = capacity == 0 ? MIN_CAPACITY : capacity * 2;
  T *newBuffer = new T[newCapacity];
  for (int i = 0; i < size; ++i) {
    newBuffer[i] = buffer[(head + i) % capacity];
  }
  delete[] buffer;
  capacity = newCapacity;
  buffer = newBuffer;
  head = 0;
  tail = size;
}

bool getResult(int command, int value, Deque<int> &deque) {
  switch (command) {
  case 1:
    deque.pushFront(value);
    return true;
  case 2:
    if (deque.isEmpty()) {
      return value == -1;
    }
    return deque.popFront() == value;
  case 3:
    deque.pushBack(value);
    return true;
  case 4:
    if (deque.isEmpty()) {
      return value == -1;
    }
    return deque.popBack() == value;
  default:
    assert(false);
  }
}

void run(std::istream &input, std::ostream &output) {
  Deque<int> deque;

int n = 0;
  input >> n;
  bool result = true;
  for (int i = 0; i < n && result; ++i) {
    int command = 0;
    int value = 0;
    input >> command >> value;
    result = getResult(command, value, deque);
  }
  output << (result ? "YES" : "NO");
}

void test() {

  {
    std::stringstream input;
    std::stringstream output;
    input << "3\n"
             "1 44\n"
             "3 50\n"
             "2 44";
    run(input, output);
    assert(output.str() == "YES");
  }
  //пустой дек
  {
    std::stringstream input;
    std::stringstream output;
    input << "2\n"
             "2 -1\n"
             "1 10";
    run(input, output);
    assert(output.str() == "YES");
  }
  // pushBack and popFront
  {
    std::stringstream input;
    std::stringstream output;
    input << "2\n"
             "3 2\n"
             "2 2";
    run(input, output);
    assert(output.str() == "YES");
  }

  // неверный результат popBack
  {
    std::stringstream input;
    std::stringstream output;
    input << "2\n"
             "3 44\n"
             "4 66";
    run(input, output);
    assert(output.str() == "NO");
  }
  // правильный pushFront и popFront с расширением буфера
  {
    std::stringstream input;
    std::stringstream output;
    input << "9\n"
             "1 1\n"
             "1 2\n"
             "1 3\n"
             "1 4\n"
             "1 5\n"
             "1 6\n"
             "1 7\n"
             "2 7\n"
             "4 1\n";
    run(input, output);
    assert(output.str() == "YES");
  }
}

int main() {
  run(std::cin, std::cout);
  //test();
  return 0;
}