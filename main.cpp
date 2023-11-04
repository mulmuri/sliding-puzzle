#include <iostream>
#include <vector>
#include <random>
#include <chrono>

const int MAX_R = 6;
const int MAX_C = 6;
const int MAX_SIZE = MAX_R * MAX_C;
const int MAX_DEPTH = 300;



namespace puzzleSolver {

    int puzzle2d[MAX_R][MAX_C] = {
        {7,1,14,10,11,6,},
        {2,15,8,16,4,5,},
        {13,0,22,9,18,3,},
        {19,20,21,28,17,12,},
        {25,26,23,27,29,24}
    };


    int puzzle[36] = {};

    int size_r = 5;
    int size_c = 6;
}

namespace IDAstar {

    void setManhattanChange(int, int);
    int manhattanChange[MAX_SIZE][MAX_SIZE][MAX_SIZE+1];
}



namespace optimizer {

    int dc[] = {-1,0,1,0};
    int dr[] = {0,-1,0,1};
    bool isOk(int r, int c, int size_r, int size_c) {
        return 0 <= r && r < size_r && 0 <= c && c < size_c;
    }


    std::vector<int> nextPos[MAX_SIZE];
    int flag = 0;

    void setNextPos(int size_r, int size_c) {
        if (flag == size_r*size_c) return;
        flag = size_r*size_c;

        for (int r=0; r<size_r; r++) {
            for (int c=0; c<size_c; c++) {
                int curPos = r*size_c+c;

                for (int k=0; k<4; k++) {
                    int nr = r+dr[k];
                    int nc = c+dc[k];
                    int newPos = nr*size_c+nc;

                    if (isOk(nr, nc, size_r, size_c)) {
                        nextPos[curPos].push_back(newPos);
                    }
                }
            }
        }
    }

}



namespace puzzleSolver {

    struct answerType {
        bool solved = false;
        int time;
        int depth = -1;
        char path[MAX_DEPTH];
        int noodeCount = 0;
    };

    void print_puzzle(int* puzzle, int size_r, int size_c) {
        for (int r=0; r<size_r; r++) {
            for (int c=0; c<size_c; c++) {
                int num = r*size_c+c;
                if (puzzle[num] < 10) std::cout <<' ';
                std::cout << puzzle[num] <<' ';
            }
            std::cout << std::endl;
        }
    }

    void print_answer(answerType& answer) {
        if (answer.solved) {
            std::cout << "spent time: " << answer.time << "ms" << std::endl;
            std::cout << "depth: " << answer.depth << std::endl;
            std::cout << "path: ";
            for (int i=0; i<answer.depth; i++) std::cout << answer.path[i];
            std::cout << std::endl;
        } else {
            std::cout << "spent time: " << answer.time << "ms" << std::endl;
            std::cout << "an unsolvable puzzle" << std::endl;
        }
        std::cout << std::endl;
    }



    int getBlank(int* puzzle, int size_r, int size_c) {
        for (int r=0; r<size_r; r++) {
            for (int c=0; c<size_c; c++) {
                if (puzzle[r*size_c+c] == 0) {
                    return r*size_c+c;
                }
            }
        }
    }

    bool isSolvable(int* puzzle, int size_r, int size_c) {
        int inverse = 0;
        return 1;

        for (int i=0; i<size_r*size_c-1; i++) {
            for (int j=i+1; j<size_r*size_c; j++) {
                if (puzzle[i] > puzzle[j] && puzzle[i] != 0 && puzzle[j] != 0) {
                    inverse++;
                }
            }
        }

        int blank = getBlank(puzzle, size_r, size_c);

        return (inverse + blank/size_c + 1) % 2;
    }



    void simulate_puzzle(int* puzzle, int size_r, int size_c, char* path, int depth) {

        std::cout << "initial: " << std::endl;
        print_puzzle(puzzle, size_r, size_c);
        std::cout << std::endl;

        int cur = getBlank(puzzle, size_r, size_c);
        int nxt;

        for (int i=0; i<depth; i++) {
            if (path[i] == 'U') nxt = cur - size_c;
            else if (path[i] == 'D') nxt = cur + size_c;
            else if (path[i] == 'R') nxt = cur + 1;
            else if (path[i] == 'L') nxt = cur - 1;

            puzzle[cur] = puzzle[nxt];
            puzzle[nxt] = 0;

            cur = nxt;

            std::cout << "step " << i+1 <<": "<< std::endl;
            print_puzzle(puzzle, size_r, size_c);
            std::cout << std::endl;
        }
    }



    void setPuzzleInit(int* puzzle, int size_r, int size_c) {
        for (int i=0; i<size_r*size_c; i++) {
            puzzle[i] = i+1;
        }
        puzzle[size_r*size_c-1] = 0;
    }

    void setPuzzle(int* board, int r, int c) {
        std::copy(board, board+size_r*size_c, puzzle);
        size_r = r;
        size_c = c;
    }

    void changePuzzleDim(int twoDarr[MAX_R][MAX_C], int* puzzle, int size_r, int size_c) {
        for (int r=0; r<size_r; r++) {
            for (int c=0; c<size_c; c++) {
                puzzle[r*size_c+c] = twoDarr[r][c];
            }
        }       
    }


    void setRandomByShuffle(int* puzzle, int size_r, int size_c) {
        setPuzzleInit(puzzle, size_r, size_c);

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        std::shuffle(puzzle, puzzle + size_r*size_c, std::default_random_engine(seed));
        while (!isSolvable(puzzle, size_r, size_c)) {
            std::shuffle(puzzle, puzzle + size_r*size_c, std::default_random_engine(seed));
        }
    }

    void setUnsolvable(int* puzzle, int size_r, int size_c) {
        setPuzzleInit(puzzle, size_r, size_c);

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        std::shuffle(puzzle, puzzle + size_r*size_c, std::default_random_engine(seed));
        while (isSolvable(puzzle, size_r, size_c)) {
            std::shuffle(puzzle, puzzle + size_r*size_c, std::default_random_engine(seed));
        }
    }

    void setRandomByTime(int* puzzle, int size_r, int size_c, int time) {
        setPuzzleInit(puzzle, size_r, size_c);

        optimizer::setNextPos(size_r, size_c);

        int cur;
        int nxt = getBlank(puzzle, size_r, size_c);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 12);

        while (time--) {
            cur = nxt;

            int num = dis(gen) % optimizer::nextPos[cur].size();
            nxt = optimizer::nextPos[cur][num];

            puzzle[cur] = puzzle[nxt];
            puzzle[nxt] = 0;
        }
    }

    void setRandomByHeuristic(int* puzzle, int size_r, int size_c, int target_h) {
        setPuzzleInit(puzzle, size_r, size_c);

        optimizer::setNextPos(size_r, size_c);
        IDAstar::setManhattanChange(size_r, size_c);

        int cur;
        int nxt = getBlank(puzzle, size_r, size_c);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 12);

        int h = 0;
        while (h != target_h) {
            cur = nxt;

            int num = dis(gen) % optimizer::nextPos[cur].size();
            nxt = optimizer::nextPos[cur][num];

            puzzle[cur] = puzzle[nxt];
            puzzle[nxt] = 0;

            h += IDAstar::manhattanChange[cur][nxt][puzzle[cur]];
        }        
    }

}




namespace IDAstar {

    void setManhattanChange(int size_r, int size_c) {
        for (int prev=0; prev<size_r*size_c; prev++) {
            int prev_r = prev/size_c;
            int prev_c = prev%size_c;

            for (int k=0; k<4; k++) {
                int new_r = prev_r + optimizer::dr[k];
                int new_c = prev_c + optimizer::dc[k];

                if (!optimizer::isOk(new_r, new_c, size_r, size_c)) continue;
                int next = new_r*size_c + new_c;

                for (int num=1; num<size_r*size_c; num++) {
                    int trg_r = (num-1)/size_c;
                    int trg_c = (num-1)%size_c;

                    int prev_dist = abs(trg_r - prev_r) + abs(trg_c - prev_c);
                    int new_dist = abs(trg_r - new_r) + abs(trg_c - new_c);

                    manhattanChange[prev][next][num] = prev_dist - new_dist;
                }
            }
        }
    }

    int cost(int* board, int size_r, int size_c) {
        int cnt = 0;

        for (int r=0; r<size_r; r++) {
            for (int c=0; c<size_c; c++) {
                int num = board[r*size_c+c];
                if (num == 0) continue;

                int trg_c = (num-1)%size_c;
                int trg_r = (num-1)/size_c;
                int dist = abs(r - trg_r) + abs(c - trg_c);

                cnt += dist;
            }
        }

        return cnt;
    }


    inline char getDir(int from, int to) {
        int diff = to - from;
        if (diff == 1) return 'R';
        if (diff == -1) return 'L';
        if (diff > 1) return 'D';
        if (diff < -1) return 'U';
    }



namespace byNaive {

    int search(int*, int, int, int, int, int);
    void solve_by_naive();

    int size_r, size_c, size;
    int src[MAX_SIZE];
    int blank;

    puzzleSolver::answerType answer;



    void solve(int* board, int r, int c) {
        size_r = r;
        size_c = c;
        size = r*c;

        answer.solved = false;

        if (puzzleSolver::isSolvable(board, r, c)) {
            blank = puzzleSolver::getBlank(board, r, c);
            std::copy(board, board+size, src);

            optimizer::setNextPos(r, c);
            setManhattanChange(r, c);

            solve_by_naive();
        }
    }


    void solve_by_naive() {
        int h = cost(src, size_r, size_c);
        int bound = h;

        int puzzle[MAX_SIZE];
        std::copy(src, src+size, puzzle);

        while (true) {
            std::cout << "depth's upper bound: " << bound << std::endl;
            bound = search(puzzle, blank, -1, bound, 0, h);
            std::cout << "nodes searched: " << answer.noodeCount<< std::endl;

            if (answer.solved == true) {
                std::cout << std::endl;
                break;
            }
        }
    }


    int search(int* puzzle, int cur, int prev, int max_depth, int depth, int h) {
        answer.noodeCount++;

        if (h == 0) {
            answer.solved = true;
            answer.depth = depth;
            return 0;
        }

        int new_bound = MAX_DEPTH;

        for (int nxt: optimizer::nextPos[cur]) {
            if (nxt == prev) continue;

            int new_h = h + manhattanChange[cur][nxt][puzzle[nxt]];
            int f = depth+1 + new_h;
            if (f > max_depth) {
                new_bound = f;
                continue;
            }

            puzzle[cur] = puzzle[nxt];
            puzzle[nxt] = 0;

            int ret_bound = search(
                puzzle,
                nxt, cur,
                max_depth,
                depth+1, new_h
            );

            if (ret_bound < new_bound)
                new_bound = ret_bound;

            puzzle[nxt] = puzzle[cur];
            puzzle[cur] = 0;
            
            if (answer.solved == true) {
                answer.path[depth] = getDir(cur, nxt);
                return 0;
            }
        }

        return new_bound;
    }

}

}



int main() {

    //puzzleSolver::setRandomByHeuristic(puzzleSolver::puzzle, puzzleSolver::size_r, puzzleSolver::size_c, 70);
    puzzleSolver::changePuzzleDim(puzzleSolver::puzzle2d, puzzleSolver::puzzle, puzzleSolver::size_r, puzzleSolver::size_c);

    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
    IDAstar::byNaive::solve(puzzleSolver::puzzle, puzzleSolver::size_r, puzzleSolver::size_c);
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();

    std::chrono::milliseconds millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    IDAstar::byNaive::answer.time = millisecond.count();

    puzzleSolver::print_answer(IDAstar::byNaive::answer);

    puzzleSolver::simulate_puzzle(
        puzzleSolver::puzzle,
        puzzleSolver::size_r,
        puzzleSolver::size_c,
        IDAstar::byNaive::answer.path,
        IDAstar::byNaive::answer.depth
    );

}