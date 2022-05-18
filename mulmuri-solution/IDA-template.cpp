#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

// variable
const int R = 3;
const int C = 3;
const int SZ = R*C;
const int INF = 100;

#define h(s,e) cost(s,e)



// graph traversal
char dm[] = {'U', 'R', 'D', 'L'};
int dr[] = {-1,0,1,0};
int dc[] = {0,1,0,-1};
bool isOk(int r, int c) {return 0 <= r && r < R && 0 <= c && c < C;}



// struct
struct Board {
    int board[R][C];    
    int x,y;

    bool operator == (Board ref) {
        for (int r=0; r<R; r++) {
            for (int c=0; c<C; c++) {
                if (board[r][c] != ref.board[r][c]) {
                    return false;
                }
            }
        }
        return true;
    }

    Board& operator = (Board& ref) {
        for (int r=0; r<R; r++) {
            for (int c=0; c<C; c++) {
                board[r][c] = ref.board[r][c];
            }
        }
        y = ref.y;
        x = ref.x;
        return *this;
    }

    friend ostream& operator << (ostream&, const Board&);
};

ostream& operator << (ostream& os, const Board& ref) {
    for (int r=0; r<R; r++) {
        for (int c=0; c<C; c++) {
            os << ref.board[r][c] <<' ';
        }
        os << '\n';
    }
    return os;
}


struct Node : Board {
    int level;
    int f, g;
    char move;
};



// class
class PuzzleSolver {
protected:

    int answer_time;
    int answer_count;
    string answer_route;

    Node puzzle;
    Node dest;

    virtual void solve() {};


public:
    PuzzleSolver() {
        for (int i=0; i<SZ; i++) dest.board[0][i] = i+1;
        dest.board[R-1][C-1] = 0;

        dest.y = R-1;
        dest.x = C-1;
    }

    void set_puzzle() {
        for (int i=0; i<SZ; i++) puzzle.board[0][i] = i;
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(puzzle.board[0], puzzle.board[R], default_random_engine(seed));

        for (int r=0; r<R; r++) {
            for (int c=0; c<C; c++) {
                if (puzzle.board[r][c] == 0) {
                    puzzle.y = r;
                    puzzle.x = c;
                    
                    return;
                }
            }
        }
    };

    void set_puzzle(vector<vector<int>> board) {
        for (int r=0; r<R; r++) {
            for (int c=0; c<C; c++) {
                puzzle.board[r][c] = board[r][c];
            }
        }

        for (int r=0; r<R; r++) {
            for (int c=0; c<C; c++) {
                if (puzzle.board[r][c] == 0) {
                    puzzle.y = r;
                    puzzle.x = c;
                    
                    return;
                }
            }
        }
    };

    void run() {
        std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
    	    solve();
        std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
        std::chrono::milliseconds millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        answer_time = millisecond.count();
        answer_count = answer_route.size() - 1;
    }

    void print_answer() {
        cout << "spent_time: " << answer_time << endl;
        cout << "count: " << answer_count << endl;
        cout << "route: " << answer_route << endl;
    };
};



// ida
class IDAstar : public PuzzleSolver {
private:
    vector<Node> path;
    bool found = false;

public:
    vector<Node> nxtNodes(Node &cur) {
        vector<Node> found;

        for (int i=0; i<4; i++) {
            Node nxt = cur;

            nxt.y = cur.y + dr[i];
            nxt.x = cur.x + dc[i];
            

            if (isOk(nxt.y, nxt.x)) {
                swap(nxt.board[nxt.y][nxt.x], nxt.board[cur.y][cur.x]);
                nxt.move = dm[i];
                nxt.level = cur.level + 1;
                found.push_back(nxt);
            }
        }

        return found;
    }


    int cost(Board &a, Board &b) {
        cout << "cost: ";
        vector<pair<int,int>> pos_a(SZ);
        vector<pair<int,int>> pos_b(SZ);
        int num;

        for (int r=0; r<R; r++) {
            for (int c=0; c<C; c++) {
                num = a.board[r][c];
                pos_a[num] = {r,c};

                num = b.board[r][c];
                pos_b[num] = {r,c};
            }
        }

        int ret = 0;
        for (int i=0; i<SZ; i++) {
            auto[ay, ax] = pos_a[i];
            auto[by, bx] = pos_b[i];

            ret += (abs(ay-by) + abs(ax-bx));
        }
        cout << ret << endl;

        return ret;
    }


    int search(int bound) {

        Node cur = path.back();
        if (cur == dest) {
            for (auto it=path.begin() + 1; it!=path.end(); it++)
                answer_route += it->move;
            return 0;
        }
        //cout << cur << endl;

        int new_bound = INF;
        for (Node nxt: nxtNodes(cur)) {
            if (find(path.begin(), path.end(), nxt) != path.end()) continue;

            nxt.g = cur.g + cost(cur, nxt);
            nxt.f = nxt.g + h(nxt, dest);
            if (nxt.f > new_bound) continue;

            path.push_back(nxt);
            new_bound = min(new_bound, search(bound));
            path.pop_back();

            if (found == true) return 0;
        }

        return new_bound;
    }

    void solve() {
        solve(PuzzleSolver::puzzle, PuzzleSolver::dest);
    }

    void solve(Node& src, Board& dest) {
        int bound = cost(src, dest);
        Node root = src;
        path.push_back(root);
        search(bound);
        path.pop_back();
    }

};




int main() {

    IDAstar solution;
    solution.set_puzzle({
        {3,4,6},
        {8,0,7},
        {2,5,1}
    });
    solution.run();
    solution.print_answer();

}