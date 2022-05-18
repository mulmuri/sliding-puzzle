// modified from https://github.com/siddg97/Npuzzle

#include <iostream>
#include <ctime>
#include <queue>
#include <vector>
#include <random>
#include <chrono>

#define int int

using namespace std;


bool isSolved = false;
int Size;
int mdTable[16][16];
int movableTable[16][4];
int path[85];
int nodeCount = 0;

struct NodeInfo {
	int id;
	int parent;
	int blank;
	int move;
};

struct Node {
	int board[16];
	NodeInfo nodeInfo;
	int f, g, move;

	Node(int* bd, int blk, int move, int id, int parent, int f = 0, int g = 0){
		for (int i= 0; i<Size*Size; ++i){
			board[i]= bd[i];
		}
		nodeInfo.blank = blk;
		nodeInfo.id = id;
		nodeInfo.parent = parent;
		nodeInfo.move = move;
		this->f = f;
		this->g = g;
	}

	bool operator >(const Node& other) const{
		return f > other.f;
	}
};

void MakeMovableTable(int size){
	int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	int board[4][4];
	for(int y = 0; y<size; ++y){
		for(int x = 0; x<size; ++x){
			board[y][x] = x+y*size;
		}
	}
	int dx, dy, j;
	for(int y = 0; y<size; ++y){
		for(int x = 0; x<size; ++x){
			for(int i = 0; i<4; ++i){
				dx = moves[i][0];
				dy = moves[i][1];
				if(x+dx<0 || y+dy<0 || x+dx>=size || y+dy>=size){
					j = -1;
				}
				else{
					j = board[y+dy][x+dx];
				} 
				movableTable[x+y*size][i] = j;
			}
		}
	}
}

void MakeMDTable(int size){
	for(int y = 0, i = 1; y<size*size; ++y, (++i)%(size*size)){
		for(int x = 0; x < size * size; ++x){
			if(i == 0){
				mdTable[i][x] = 0;
			}
			else{
				mdTable[i][x] = abs((y / size) - (x / size)) + abs((y % size) - (x % size));
			}
		}
	}
}

int GetDistance(int *row, int md, int nums){
	if (nums > 1){
		if (nums == 2){
			if (row[0] > row[1])
				md += 2;
		}
		else if (nums == 3){
			if (row[0] > row[1] || row[0] > row[2])
				md += 2;
			if (row[1] > row[2])
				md += 2;
		}
		else if (nums == 4){
			if (row[0] > row[1] || row[0] > row[2] || row[0] > row[3])
				md += 2;
			if (row[1] > row[2] || row[1] > row[3])
				md += 2;
			if (row[2] > row[3])
				md += 2;
		}
	}
	return md;
}

int GetManhattan(int* puzzle){
	int i, j, x, md = 0;
	int k, n;
	int temp[4];

	for(i = 0; i < Size*Size; ++i){
		md += mdTable[puzzle[i]][i];
	}
	for (i = 0, x = 1; i<Size; ++i, ++x){
		k = 0;
		for (j = 0; j<Size; ++j){
			n = puzzle[i*Size+j];
			if (n <= Size*x && n>Size*(x-1)){
				temp[k++]= n;
			}
		}
		md = GetDistance(temp, md, k);
	}
	for (i = 0, x = 1; i < Size; ++i, ++x){
		k = 0;
		for (j = 0; j < Size; ++j){
			n = puzzle[j * Size + i];
			if (n == x || n == x + Size || n == x + Size * 2 || n == x + Size * 3){
				temp[k++] = n;
			}
		}
		md = GetDistance(temp, md, k);
	}
	return md;
}

int GetBlank(int *puzzle){
	for(int i = 0; i < Size * Size; ++i){
		if (puzzle[i] == 0)
		{
			return i;
		}
	}
}

void PrintPath(int depth){
	for(int i = 0; i < depth; ++i){
		printf("%2d  ",path[i]);
		if((i + 1) % 10 == 0) 
			printf("\n");
	}
	printf("\n\n");
}

void PrintPuzzle(int* puzzle){
	for(int i = 0; i < Size; ++i){
		for(int j = 0; j < Size; ++j){
			printf("%2d ", puzzle[i * Size + j]);
		}
		printf("\n");
	}
	printf("\n");
}

int IdaStar(int depth, int maxDepth, int* puzzle, int lastMove, int blank){
	int h, f, min;
	nodeCount += 1;
	h = GetManhattan(puzzle);
	f = depth + h;
	if (f > maxDepth ){
		return f;
	}

	if (h == 0){
		isSolved = true;
		return f;
	}

	min = 127;
	for (int move = 0; move < 4; ++move){
		if(lastMove == -1 || (move + lastMove) % 4 != 1){
			int newBlank = movableTable[blank][move];
			if(newBlank == -1) 
				continue;
			puzzle[blank] = puzzle[newBlank];
			puzzle[newBlank] = 0;
			f = IdaStar(depth + 1, maxDepth, puzzle, move, newBlank);
			puzzle[newBlank] = puzzle[blank];
			puzzle[blank] = 0;
			if (f < min){
				min = f;
			}
			if (isSolved){
				path[depth] = puzzle[newBlank];
				return min;
			}
		}
	}
	return min;
}

void IDAStar(int* puzzle){
	time_t start, end;
	int h, depth;
	int lastMove = -1;
	int blank = GetBlank(puzzle);
	
	isSolved = false;
	int totalCount = 0;
	
	h = GetManhattan(puzzle);
	depth = h;
	start = clock();
	while (true){
		nodeCount = 0;
		depth = IdaStar(0, depth, puzzle, lastMove, blank);
		end = clock();
		totalCount += nodeCount;
		printf("Depth = %d, Node Count = %10u, Total Count = %10u, time = %6.2f msecs\n",h, nodeCount, totalCount, (end-start)/1000.);
		if (isSolved){
			printf("Total Node Count = %u, shortest path length = %d, time = %.2f msecs\n",totalCount, depth, (end-start)/1000.);
			return;
		}
		h = depth;
	}
}

void ShuffleArray(int arr[], int n) { //from : https://www.geeksforgeeks.org/shuffle-an-array-using-stl-in-c/
    // To obtain a time-based seed 
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    // Shuffling our array 
    shuffle(arr, arr + n, default_random_engine(seed));
}

int* ToIntArr(int * c , int s){
 	int *board=new int[s];
	for(int i=0;i<s;i++){
		int a=(int) c[i];
		board[i]=a;
	}
	return board;
} 

bool solvable(int * c){
	int *board=ToIntArr(c,16);
	// for(int i=0;i<16;i++)
	// 	cout<<board[i]<<" ";
	// cout<<endl;
	int count=0;
	for(int i=0;i<15;i++){
        for (int j=i+1;j<16;j++)
             if(board[i]>board[j] && board[i]!=0 &&board[j]!=0){ 
                 count++;
             }
    }
    return (count%2+1)%2;
}

int main(){
	int  size=4;
	int puzzle[16] = {
        12, 15, 4, 1,
        2, 11, 9, 3,
        7, 14, 13, 5,
        6, 8, 10, 0
    };
	int p[16];
	MakeMDTable(size);
	MakeMovableTable(size);
	Size = size;
	ShuffleArray(puzzle,16);
	while(!solvable(puzzle)){
		ShuffleArray(puzzle,16);
	}
    
	cout << "  PUZZLE TO SOLVE  \n--------------------" << endl << endl;
	PrintPuzzle(puzzle);
	cout << "--------------------" << endl << endl;
	// cout<<solvable(puzzle)<<endl;
	printf("\n===== IDA Star Algiorithm =====\n");
	IDAStar(puzzle);

	return 0;
}
