/**
 * Задача 4. «Пятнашки» (8 баллов)
 * Написать алгоритм для решения игры в “пятнашки”.
 * Решением задачи является приведение к виду:
 * [ 1  2  3  4 ]
 * [ 5  6  7  8 ]
 * [ 9  10 11 12]
 * [ 13 14 15 0 ]
 * где 0 задает пустую ячейку.
 * Достаточно найти хотя бы какое-то решение.
 * Число перемещений костяшек не обязано быть минимальным.
 * Формат входных данных
 * Начальная расстановка.
 * Формат выходных данных
 * Если решение существует, то в первой строке выходного файла
 * выведите минимальное число перемещений костяшек, которое нужно сделать,
 * чтобы достичь выигрышной конфигурации, а во второй строке выведите
 * соответствующую последовательность ходов:
 * L означает, что костяшка сдвинулась влево,
 * R – вправо, U – вверх, D – вниз.
 * Если таких последовательностей несколько, то выведите любую из них.
 * Если же выигрышная конфигурация недостижима,
 * то выведите в выходной файл одно число −1.
 */
#include <assert.h>

#include <array>
#include <cstring>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>

using std::array;
using std::cout;
using std::ostream;
using std::string;
using std::swap;
using std::set;
using std::unordered_set;

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;

class GameState {
   public:
    explicit GameState(const array<char, FieldSize>& _field);

    bool isSolved() const;

    bool CanMoveLeft() const;
    bool CanMoveUp() const;
    bool CanMoveRight() const;
    bool CanMoveDown() const;

    GameState MoveLeft() const;
    GameState MoveUp() const;
    GameState MoveRight() const;
    GameState MoveDown() const;

    int getHeuristicValue() const { return heuristicValue; }
    void updateHeuristicValue();
    int getManhattanDistance() const;
    int lastMove() const;
    string getHistory() const { return history; }

    bool isSolvable()const;

    bool operator<(const GameState& state) const {
        return heuristicValue < state.getHeuristicValue();
    }
    bool operator==(const GameState& state) const {
        return field == state.field;
    }
    bool operator!=(const GameState& state) const { return !(*this == state); }

   private:
    int heuristicValue;
    array<char, FieldSize> field;
    char zeroPos;
    string history;

    friend struct StateHasher;
};

GameState::GameState(const array<char, FieldSize>& _field)
    : field(_field), zeroPos(-1), history("") {
    for (unsigned int i = 0; i < field.size(); ++i)
        if (field[i] == 0) {
            zeroPos = i;
            break;
        }
    updateHeuristicValue();
    assert(zeroPos != -1);
}

bool GameState::CanMoveLeft() const { return zeroPos % SideSize != 0; }

bool GameState::CanMoveUp() const { return zeroPos >= SideSize; }

bool GameState::CanMoveRight() const {
    return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveDown() const { return zeroPos < FieldSize - SideSize; }

GameState GameState::MoveLeft() const {
    assert(CanMoveLeft());
    GameState newState(*this);
    swap(newState.field[newState.zeroPos],
         newState.field[newState.zeroPos - 1]);
    --newState.zeroPos;
    newState.history += 'R';
    newState.updateHeuristicValue();

    return newState;
}

GameState GameState::MoveUp() const {
    assert(CanMoveUp());
    GameState newState(*this);
    swap(newState.field[newState.zeroPos],
         newState.field[newState.zeroPos - SideSize]);
    newState.zeroPos -= SideSize;
    newState.history += 'D';
    newState.updateHeuristicValue();
    return newState;
}

GameState GameState::MoveRight() const {
    assert(CanMoveRight());
    GameState newState(*this);
    swap(newState.field[newState.zeroPos],
         newState.field[newState.zeroPos + 1]);
    ++newState.zeroPos;
    newState.history += 'L';
    newState.updateHeuristicValue();
    return newState;
}

GameState GameState::MoveDown() const {
    assert(CanMoveDown());
    GameState newState(*this);
    swap(newState.field[newState.zeroPos],
         newState.field[newState.zeroPos + SideSize]);
    newState.zeroPos += SideSize;
    newState.history += 'U';
    newState.updateHeuristicValue();
    return newState;
}

struct StateHasher {
    size_t operator()(const GameState& state) const {
        size_t hash = 0;
        memcpy(&hash, &state.field[0], sizeof(hash));
        return hash;
    }
};

void GameState::updateHeuristicValue() {
    heuristicValue = getManhattanDistance() + lastMove();
}
int GameState::getManhattanDistance() const {
    int distance = 0;
    for (int i = 0; i < FieldSize; ++i) {
        int value = (field[i] + FieldSize - 1) % FieldSize;
        distance += abs((value - i) % SideSize) +
                    abs((value / SideSize - i / SideSize));
    }
    return distance;
}
bool GameState::isSolved() const {
    for (int i = 0; i < FieldSize; ++i) {
        if (field[i] != (i + 1) % FieldSize) return false;
    }
    return true;
}
int GameState::lastMove() const {
    return (field[FieldSize - 1] == FieldSize - 1 ||
            field[FieldSize - 1] == FieldSize - SideSize)
               ? 0
               : 2;
}
bool GameState::isSolvable() const{
    int sum = 0;
    for (int i = FieldSize - 1; i >= 0; --i) {
        int lessThenCurrentAfter = 0;
        if (field[i] != 0) {
            for (int j = i + 1; j < FieldSize;++j){
                if (field[i] > field[j] && field[j] != 0) {
                    ++lessThenCurrentAfter;
                }
            }
        }
        sum +=lessThenCurrentAfter;
    }
    sum += zeroPos/SideSize + 1;
    return sum % 2 == 0;
}

string Get15thSolution(const GameState& state) {
    set<GameState> set;
    unordered_set<GameState, StateHasher> visited;
    set.insert(state);
    while (!set.empty()) {
        GameState tempState = *set.begin();
        set.erase(set.begin());
        if (tempState.isSolved()) {
            return tempState.getHistory();
        }
        string history = tempState.getHistory();
        if (tempState.CanMoveLeft() && (history.empty() || history.back() != 'L')) {
            GameState newState = tempState.MoveLeft();
            if (visited.find(newState) == visited.end()) {
                visited.insert(newState);
                set.insert(newState);
            }
        }
        if (tempState.CanMoveUp()&& (history.empty() || history.back() != 'U')) {
            GameState newState = tempState.MoveUp();
            if (visited.find(newState) == visited.end()) {
                visited.insert(newState);
                set.insert(newState);
            }
        }
        if (tempState.CanMoveRight()&& (history.empty() || history.back() != 'R')) {
            GameState newState = tempState.MoveRight();
            if (visited.find(newState) == visited.end()) {
                visited.insert(newState);
                set.insert(newState);
            }
        }
        if (tempState.CanMoveDown()&& (history.empty() || history.back() != 'D')) {
            GameState newState = tempState.MoveDown();
            if (visited.find(newState) == visited.end()) {
                visited.insert(newState);
                set.insert(newState);
            }
        }
    }
    return string();


}

void run(std::istream &input, std::ostream &output) {

    array<char, FieldSize> field;
    for (int i = 0; i < FieldSize;++i) {
        int a;
        input >> a;
        field[i] = a;
    }
    GameState state(field);
    if (!state.isSolvable()) {
        output << "-1";
    } else {
        string solution = Get15thSolution(state);
        output << solution.length() << std::endl;
        for (char i : solution) {
            output << i << " ";
        }
    }
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "1 2 3 4\n"
                 "5 6 7 8\n"
                 "9 10 11 0\n"
                 "13 14 15 12";
        run(input, output);
        assert(output.str() == "1\nU ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1 2 3 4\n"
                 "5 6 7 8\n"
                 "9 10 11 12\n"
                 "13 15 14 0";
        run(input, output);
        assert(output.str() == "-1");
    }

}

int main() {
    //run(std::cin, std::cout);

    test();
    return 0;
}
