#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <cassert>
#include <queue>
#include <unordered_set>
#include <sstream>
using namespace std;

template <typename T>
T** create2DArray(int width, int height) {
    T ** res = new T* [width];

    for (int i = 0; i < width; ++i) {
        res[i] = new T[height];
    }
    return res;
}

struct Node {
    int x;
    int y;
    int food;
};
class FoodComparator {
public:
    bool operator()(const Node& left, const Node& right) {
        return left.food < right.food;
    }
};
namespace std {
    template <>
    struct hash<Node*> {
        size_t operator() (Node* const& node) const {
            return 13 + std::hash<int>()(node->x) + 71 +
                   std::hash<int>()(node->y) + 131 +
                   std::hash<int>()(node->food);
        }
    };
}

class World {
public:
    const int width_;
    const int height_;

    World(int width, int height) : width_(width), height_(height),
        food_(create2DArray<int>(width, height)) {}

    int foodAt(int x, int y) {
        return food_[x][y];
    }

    int setFoodAt(int x, int y, int food) {
        food_[x][y] = food;
    }

    void getNeighbours(const Node& center, Node* neighbours, int& count) {
        assert(! isEndNode(center));
        count = 0;

        if (center.x < width_ - 1) {
            assignRightNode(neighbours[count++], center);
        }
        if(center.y < height_ - 1) {
            assignBottomNode(neighbours[count++], center);
        }
    }

    void assignRightNode(Node& right_node, const Node& node) {
        assert(node.x + 1 < width_);

        right_node.x = node.x + 1;
        right_node.y = node.y;
        right_node.food = node.food + foodAt(node.x + 1, node.y);
    }

    void assignBottomNode(Node& bottom_node, const Node& node) {
        assert(node.y + 1 < height_);

        bottom_node.x = node.x;
        bottom_node.y = node.y + 1;
        bottom_node.food = node.food + foodAt(node.x, node.y + 1);
    }

    bool isEndNode(const Node& node) {
        return node.x == width_ - 1 &&
            node.y == height_ - 1;
    }

private:
    int** food_;
};
class Visitor {
public:
//    const width_;
//    const height_;

//    Visitor(const int width, const int height)
//        : width_(width), height_(height),
//        visited_(create2DArray<bool>(width, height)) {
//        for (int i = 0; i < width; ++i) {
//            for (int j = 0; j < height; ++j) {
//                visited_[i][j] = false;
//            }
//        }
//    }

    bool isVisited(Node& node) {
//        return visited_[node.x][node.y];
        return visited_.count(&node) != 0;
    }

    void visit(Node& node) {
//        visited_[node.x][node.y] = true;
        visited_.insert(&node);
    }

private:
//    bool** visited_;
    unordered_set<Node*> visited_;
};

int main()
{
//    stringstream cin;
//    cin << "3 3\n"
//           "1 2 3\n"
//           "4 5 6\n"
//           "7 8 9\n";

    int W, H;
    cin >> W >> H; cin.ignore();
    cerr << "W: " << W << "\nH: " << H;
    
    World world(W, H);
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            int food; cin >> food; cin.ignore();
            world.setFoodAt(x, y, food);
        }
    }

//    for (int y = 0; y < H; ++y) {
//        for (int x = 0; x < W; ++x) {
//            cerr << world.foodAt(x, y) << " ";
//        }
//        cerr << "\n";
//    }

    Visitor visitor;
    priority_queue<Node, vector<Node>, FoodComparator> moves;

    auto* adjacent = new Node[2];
    for (int j = 0; j < 2; ++j) {
        adjacent[j].x = adjacent[j].y = adjacent[j].food = -1;
    }
    int adj_count = -1;

    moves.push({0, 0, world.foodAt(0, 0)});

    int max_food_eaten = 0;
    while (! moves.empty()) {
        Node next = moves.top();
        moves.pop();

        if(world.isEndNode(next)) {
             if(max_food_eaten < next.food) {
                 max_food_eaten = next.food;
             }
             visitor.visit(next);
            continue;
        }

        world.getNeighbours(next, adjacent, adj_count);

        for (int i = 0; i < adj_count; ++i) {
            auto& adj = adjacent[i];
            if (! visitor.isVisited(adj) ) {
                moves.push(adj);
            }
        }

        visitor.visit(next);
    }

    cout << max_food_eaten << endl;
}