#include <iostream>
#include <string>
using namespace std;



const int R = 5;
const int C = 6;

int input[R][C] = {};



class PuzzleSolver {

private:
    int answer_time;
    int answer_count;
    string answer_route;

    int puzzle[R][C];

public:
    bool predict_solvability(int board[R][C]) {
        // 중복이 없는지 체크
        // 규칙을 만족하는지 체크
    };

    void set_puzzle() {
        // 랜덤으로 valid한 board를 생성
    };

    void set_puzzle(int arr[R][C]) {
        // 입력받은대로 board 생성
    };

    void print_answer() {

    };

    void simulate_answer() {

    };

    void solve();
};



class Astar : private PuzzleSolver {
public:
    void solve();

};


class IDAstar : private PuzzleSolver {
public:
    void solve();

};


class Constructive_DnC : private PuzzleSolver {
public:
    void solve();
};


class Astar_DnC : private PuzzleSolver {
public:
    void solve();
};




int main() {

}