/*Решение всех задач данного раздела предполагает использование кучи,
реализованной в виде класса.
Решение должно поддерживать передачу функции
сравнения снаружи. Куча должна быть динамической.
Дан массив натуральных чисел A[0..n), n не превосходит 10^8.
Так же задан размер некоторого окна
(последовательно расположенных элементов массива) в этом массиве k, k<=n.
Требуется для каждого положения окна (от 0 и до n-k) вывести значение максимума
в окне. Требования: Скорость работы O(n log n), память O(n). Формат входных
данных. Вначале вводится n - количество элементов массива. Затем вводится n
строк со значением каждого элемента. Затем вводится k  - размер окна. Формат
выходных данных. Разделенные пробелом значения максимумов для каждого положения
окна.*/
#include <assert.h>

#include <iostream>

template <class T>
class Heap {
   public:
    explicit Heap(bool (*compareFunc)(const T &a, const T &b));
    Heap(const T *array, size_t size,
         bool (*compareFunc)(const T &a, const T &b));
    Heap(const Heap &heap) = delete;
    ~Heap();
    void insert(const T &value);
    T extractMax();
    T peekMax() const {
        assert(isEmpty());
        return buffer[0];
    };

   private:
    size_t size;
    size_t capacity;
    T *buffer;
    bool (*compare)(const T &a, const T &b);
    void buildHeap();
    void shiftDown(size_t index);
    void shiftUp(size_t index);
    void swap(size_t a, size_t b);
    void expand();
    bool isEmpty() const { return size == 0; }
};

template <class T>
void Heap<T>::shiftDown(size_t index) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t largest = index;
    if (right < size && compare(buffer[largest], buffer[left])) {
        largest = left;
    }
    if (left < size && compare(buffer[largest], buffer[right])) {
        largest = right;
    }
    if (largest != index) {
        swap(index, largest);
        shiftDown(largest);
    }
}

template <class T>
void Heap<T>::buildHeap() {
    for (size_t i = size / 2 - 1; i >= 0; --i) {
        shiftDown(i);
    }
}

template <class T>
void Heap<T>::shiftUp(size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (!compare(buffer[parent], buffer[index])) {
            return;
        }
        swap(index, parent);
        index = parent;
    }
}
template <class T>
void Heap<T>::swap(size_t a, size_t b) {
    assert(a < size && b < size);
    T *temp = buffer[a];
    buffer[a] = buffer[b];
    buffer[b] = temp;
}

template <class T>
void Heap<T>::insert(const T &value) {
    if (size >= capacity) {
        expand();
    }
    buffer[size] = value;
    ++size;
    shiftUp(size - 1);
}

template <class T>
void Heap<T>::expand() {
    const int MIN_CAPACITY = 8;
    int newCapacity = capacity == 0 ? MIN_CAPACITY : capacity * 2;
    T *newBuffer = new T[newCapacity];
    for (int i = 0; i < size; ++i) {
        newBuffer[i] = buffer[i];
    }
    delete[] buffer;
    capacity = newCapacity;
    buffer = newBuffer;
}

template <class T>
T Heap<T>::extractMax() {
    assert(isEmpty());
    T result = buffer[0];
    buffer[0] = buffer[size - 1];
    --size;
    if (!isEmpty()) {
        shiftDown(0);
    }
    return result;
}
template <class T>
Heap<T>::~Heap() {
    delete[] buffer
}
template <class T>
Heap<T>::Heap(bool (*compareFunc)(const T &, const T &))
    : size(0), capacity(0), buffer(nullptr), compare(compareFunc) {}
template <class T>
Heap<T>::Heap(const T *array, size_t size,
              bool (*compareFunc)(const T &, const T &))
    : size(size), capacity(size), buffer(nullptr), compare(compareFunc) {
    buffer = new T[size];
    for (size_t i = 0; i < size; ++i) {
        buffer[i] = array[i];
    }
    buildHeap();
}

struct elemInWindows {
    elemInWindows() {}
    elemInWindows(int _value, int _position) {
        value = _value;
        position = _position;
    }
    int value;
    int position;
};

int findMaxInWindow(Heap<elemInWindows> heap, int startPosition);
bool elemInWindowsCompare(const elemInWindows &a, const elemInWindows &b) {
    return a.value < b.value;
}
