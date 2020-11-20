/**
 * Реализуйте структуру данных типа “множество строк” на основе динамической
 * хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
 * строчных латинских букв. Хеш-функция строки должна быть реализована с помощью
 * вычисления значения многочлена методом Горнера. Начальный размер таблицы
 * должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов
 * в случае, когда коэффициент заполнения таблицы достигает 3/4. Структура
 * данных должна поддерживать операции добавления строки в множество, удаления
 * строки из множества и проверки принадлежности данной строки множеству. Для
 * разрешения коллизий используйте двойное хеширование. Требования: В таблице
 * запрещено хранение указателей на описатель элемента. Формат входных данных
 * Каждая строка входных данных задает одну операцию над множеством. Запись
 * операции состоит из типа операции и следующей за ним через пробел строки, над
 * которой проводится операция. Тип операции  – один из трех символов:
 *   +  означает добавление данной строки в множество;
 *   -  означает удаление  строки из множества;
 *   ?  означает проверку принадлежности данной строки множеству.
 * При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в
 * этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он
 * присутствует в этом множестве.
 */

#include <assert.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

template <class T, class H>
class HashTable {
   public:
    HashTable(H& _hasher, const T& EmptyValue, const T& DelValue);
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    ~HashTable() = default;
    bool Add(const T& data);
    bool Has(const T& data) const;
    bool Delete(const T& data);

   private:
    const T DELETE_VALUE;
    const T EMPTY_VALUE;
    H hasher;
    vector<T> table;
    size_t keysCount;
    void growTable();
    void reduceTable();
};
template <class T, class H>
HashTable<T, H>::HashTable(H& _hasher, const T& EmptyValue, const T& DelValue)
    : DELETE_VALUE(DelValue),
      EMPTY_VALUE(EmptyValue),
      hasher(_hasher),
      table(8, EMPTY_VALUE),
      keysCount(0) {}


template <class T, class H>
bool HashTable<T, H>::Add(const T& data) {
    if (keysCount * 4 > table.size() * 3) {
        growTable();
    }
    long int firstDelete = -1;
    size_t i = 0;
    unsigned int hash = hasher(data, i) % table.size();
    while (i < table.size() && table[hash] != EMPTY_VALUE &&
           table[hash] != data) {
        if (firstDelete == -1 && table[hash] == DELETE_VALUE) {
            firstDelete = hash;
        }
        ++i;
        hash = hasher(data, i) % table.size();
    }
    if (table[hash] == data) {
        return false;
    }
    if (table[hash] == EMPTY_VALUE) {
        if (firstDelete != -1) {
            table[firstDelete] = data;
        } else {
            table[hash] = data;
        }
        ++keysCount;
        return true;
    }
    assert(firstDelete != -1);
    table[firstDelete] = data;
    return true;
}


template <class T, class H>
bool HashTable<T, H>::Has(const T& data) const {
    size_t i = 0;
    unsigned int hash = hasher(data, i) % table.size();
    while (i < table.size() && table[hash] != EMPTY_VALUE &&
           table[hash] != data) {
        ++i;
        hash = hasher(data, i) % table.size();
    }
    if (table[hash] == data) {
        return true;
    }
    return false;
}


template <class T, class H>
bool HashTable<T, H>::Delete(const T& data) {
    size_t i = 0;
    unsigned int hash = hasher(data, i) % table.size();
    while (i < table.size() && table[hash] != EMPTY_VALUE &&
           table[hash] != data) {
        ++i;
        hash = hasher(data, i) % table.size();
    }
    if (table[hash] == data) {
        table[hash] = DELETE_VALUE;
        --keysCount;
        if (keysCount * 9 < table.size()) {
            reduceTable();
        }
        return true;
    }
    return false;
}
template <class T, class H>
void HashTable<T, H>::growTable() {
    vector<T> newTable(table.size() * 2, EMPTY_VALUE);
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i] != DELETE_VALUE && table[i] != EMPTY_VALUE) {
            unsigned int newHash = hasher(table[i], 0) % newTable.size();
            for (size_t j = 1; newTable[newHash] != EMPTY_VALUE;
                 newHash = hasher(table[i], j++) % newTable.size())
                ;
            newTable[newHash] = table[i];
        }
    }
    table = std::move(newTable);
}
template <class T, class H>
void HashTable<T, H>::reduceTable() {
    vector<T> newTable(table.size() / 2, EMPTY_VALUE);
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i] != DELETE_VALUE && table[i] != EMPTY_VALUE) {
            unsigned int newHash = hasher(table[i], 0) % newTable.size();
            for (size_t j = 1; newTable[newHash] != EMPTY_VALUE;
                 newHash = hasher(table[i], j++) % newTable.size())
                ;
            newTable[newHash] = table[i];
        }
    }
    table = std::move(newTable);
}

struct Hasher {
    unsigned int operator()(const string& data, const unsigned int& i) const {
        return (hash(data, 7) + i * (hash(data, 3) * 2 + 1));
    }
    unsigned int hash(const string& data, const unsigned int& alpha) const {
        int hash = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            hash = hash * alpha + data[i];
        }
        return hash;
    }
};

void run(std::istream& input, std::ostream& output) {
    Hasher hasher;
    HashTable<string, Hasher> table(hasher, "EMPTY", "DELETE");
    char operation;
    string word;
    while (input >> operation >> word) {
        if (operation == '+') {
            output << (table.Add(word) ? "OK" : "FAIL") << std::endl;
        } else if (operation == '-') {
            output << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
        } else if (operation == '?') {
            output << (table.Has(word) ? "OK" : "FAIL") << std::endl;
        }
    }
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "+ hello\n"
                 "+ bye\n"
                 "? bye\n"
                 "+ bye\n"
                 "- bye\n"
                 "? bye\n"
                 "? hello";
        run(input, output);
        assert(output.str() ==
               "OK\n"
               "OK\n"
               "OK\n"
               "FAIL\n"
               "OK\n"
               "FAIL\n"
               "OK\n");
    }
}

int main() {
    run(std::cin, std::cout);
    // test();
    return 0;
}
