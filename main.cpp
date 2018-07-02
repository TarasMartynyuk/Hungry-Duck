#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <cassert>
#include <queue>
#include <unordered_set>
#include <sstream>
#include <iterator>

using namespace std;

template <typename T>
T** create2DArray(int width, int height) {
    T ** res = new T* [width];

    for (int i = 0; i < width; ++i) {
        res[i] = new T[height];
    }
    return res;
}

struct State {
    int row;
    int column;
    int food;
};

class World {
public:
    const int rows_;
    const int columns_;

    World(int rows, int columns) : rows_(rows), columns_(columns),
        food_(create2DArray<int>(rows, columns)) {}

    int foodAt(int row, int col) {
        return food_[row][col];
    }

    int setFoodAt(int row, int col, int food) {
        food_[row][col] = food;
    }

    void getNeighbours(const State& center, State* neighbours, int& count) {
        count = 0;

        if(center.row < rows_ - 1) {
            assignBottomNode(neighbours[count++], center);
        }
        if (center.column < columns_ - 1) {
            assignRightNode(neighbours[count++], center);
        }
    }

    bool isEndNode(const State& node) {
        return node.row == columns_ - 1 &&
            node.column == rows_ - 1;
    }

private:
    int** food_;

    void assignBottomNode(State& bottom_node, const State& node) {
        assert(node.row + 1 < rows_);

        bottom_node.row = node.row + 1;
        bottom_node.column = node.column;
        bottom_node.food = node.food + foodAt(bottom_node.row, bottom_node.column);
    }

    void assignRightNode(State& right_node, const State& node) {
        assert(node.column + 1 < columns_);

        right_node.row = node.row;
        right_node.column = node.column + 1;
        right_node.food = node.food + foodAt(right_node.row, right_node.column);
    }
};

int main()
{
    //region input
    int columns, rows;
    cin >> columns >> rows; cin.ignore();

    World world(rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int food; cin >> food; cin.ignore();
            world.setFoodAt(i, j, food);
        }
    }

    // for each coordinate , holds the max amount of food we had when we encountered previously,
    // or -1 if we haven't been at that coordinates yet
    vector<vector<int>> prev_states_food(rows, vector<int>(columns, -1));
    queue<State> states;

    auto* adjacent = new State[2];
    for (int j = 0; j < 2; ++j) {
        adjacent[j].row = adjacent[j].column = adjacent[j].food = -1;
    }
    int adj_count = -1;

    states.push({0, 0, world.foodAt(0, 0)});

    int additions = 1;
    int poppings = 0;

    while (! states.empty()) {
        State next = states.front();
        states.pop();
        poppings++;

        world.getNeighbours(next, adjacent, adj_count);

        for (int i = 0; i < adj_count; ++i) {
            auto& alt = adjacent[i];
            int prev_state_food =
                prev_states_food.at(alt.row).at(alt.column);
            // add only if there isn't a state we discovered previously with better score
            if (alt.food > prev_state_food) {
                prev_states_food.at(alt.row).at(alt.column) = alt.food;
                states.push(alt);
                additions++;
            }
        }
    }

    cerr << "additions: " << additions << "\n";
    cerr << "poppings: " << poppings << "\n";
    cout << prev_states_food.at(rows - 1).at(columns - 1) << endl;
}