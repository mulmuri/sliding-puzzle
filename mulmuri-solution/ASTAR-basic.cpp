#include <iostream>
#include <queue>
#include <chrono>
#include <set>
using namespace std;

const int R = 4;
const int C = 4;



struct Node {
	Node* parent;
	int mat[R][C];
	int x, y;
	int cost, level;
};

Node* newNode(int mat[R][C], int x, int y, int newX, int newY, int level, Node* parent) {
	Node* node = new Node;

	node->parent = parent;
	memcpy(node->mat, mat, sizeof node->mat);
	swap(node->mat[x][y], node->mat[newX][newY]);
	node->cost = INT_MAX;
	node->level = level;
	node->x = newX;
	node->y = newY;

	return node;
}

struct comp {
	bool operator() (const Node* lhs, const Node* rhs) const {
		return (lhs->cost + lhs->level) > (rhs->cost + rhs->level);
	}
};



void printMatrix(int mat[R][C]) {
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++)
			printf("%d ", mat[i][j]);
		printf("\n");
	}
}

int printPath(Node* root) {
	if (root == NULL)
		return 0;
	int cnt = printPath(root->parent);
	printMatrix(root->mat);
	printf("\n");
    return cnt+1;
}

void printAns(Node* root) {
    printf("length: %d, \n", printPath(root));
}



long long getHash(int mat[R][C]) {
    long long hash = 0;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            hash += mat[i][j];
            hash <<= 4;
        }
    }
    //cout << hash << endl;
    return hash;
}

set<long long> vst;

int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };
bool isSafe(int x, int y) {return x >= 0 && x < R && y >= 0 && y < C; }


/*
int Cost(int initial[R][C], int final[R][C]) {
	int count = 0;

	for (int i = 0; i < R; i++)
	    for (int j = 0; j < C; j++)
		    if (initial[i][j] && initial[i][j] != final[i][j])
		        count++;

	return count;
}
*/

typedef pair<int,int> co;
co findCo(int val, int arr[R][C]) {
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (arr[i][j] == val) {
                return {i,j};
            }
        }
    }
}

int Cost(int initial[R][C], int final[R][C]) {
    int count = 0;

    for (int i = 0; i < R*C; i++ ) {
        co st = findCo(i, initial);
        co ed = findCo(i, final);
        int point = abs(st.first - ed.first) + abs(st.second - ed.second);
        count += point;
    }
    return count;
}





void solve(int initial[R][C], int x, int y,	int final[R][C]) {
	priority_queue<Node*, vector<Node*>, comp> pq;

	Node* root = newNode(initial, x, y, x, y, 0, NULL);
	root->cost = Cost(initial, final);

	pq.push(root);
    vst.insert(getHash(root->mat));

    int max_lv = 0;

	while (!pq.empty()) {
		Node* min = pq.top();

        vst.insert(getHash(min->mat));

		pq.pop();

		if (min->cost == 0) {
			printAns(min);
			return;
		}

		for (int i = 0; i < 4; i++) {
			if (isSafe(min->x + row[i], min->y + col[i])) {
				Node* child = newNode(
                            min->mat, min->x,
							min->y, min->x + row[i],
							min->y + col[i],
							min->level + 1, min);
				child->cost = Cost(child->mat, final);

                if (vst.find(getHash(child->mat)) != vst.end())
                    continue;

                if (max_lv < child->level) {
                    max_lv = child->level;
                    cout << max_lv << endl;
                }

				pq.push(child);
			}
		}
	}
}



int main() {
    std::chrono::system_clock::time_point startTime, endTime;
    std::chrono::milliseconds millisecond;



	int initial[R][C] = {
        {12, 15, 4, 1},
        {2, 11, 9, 3},
        {7, 14, 13, 5},
        {6, 8, 10, 0}
    };
    //for (int i = 0; i < R; i++) {
    //    for (int j = 0; j < C; j++) {
    //        cin >> initial[i][j];
    //    }
    //}
    int x = 3, y = 3;

	int final[R][C] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12},
        {13,14,15}
    };

/*
             0 15  8  3     6  5  9 13    11  5  9 13     0 15  8 13     0 15  8 13     0 11  9 13
            12 11  7  4     2  1 10 14     2  6 10 14    12 11  3  7    12 11  9 10    12 15 10 14
            14 10  6  5     3  7  0 15     3  7  0 15    14  9  6  2    14  3  6  2     3  7  6  2
             9 13  2  1     4  8 12 11     4  8 12  1 	  4 10  5  1     4  7  5  1     4  8  5  1
Estimate:            48             58             58             58             62             66
Moves:               70             72             74             76             78             80
Time:            111.4s          56.6s          13.7s           5.8s           5.7s           3.5s
Nodes:        492357819      231367077       46383751       20187376       18363209       12715201
*/


/*
	int initial[R][C] = {
		{7, 2, 3},
		{5, 1, 4},
		{6, 8, 0}
	};

	int final[R][C] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
	};
*/
/*
    int initial[R][C] = {
        {4, 2, 3, 1},
        {9, 5, 11, 8},
        {6, 7, 0, 10}
    };

    int final[R][C] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,0}
    };
*/

    startTime = std::chrono::system_clock::now();
    	solve(initial, x, y, final);
    endTime = std::chrono::system_clock::now();
    millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Time Cost: " << millisecond.count() << " ms\n";

	return 0;
}
