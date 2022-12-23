#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 75 /* max length of lines */
#define GRIDSIZE 500 /* length of grid */
#define START 150 /* What to add to row, col to get to (x, y) in input */

class Elf {
    private:
        int _r;
        int _c;
        int next_r;
        int next_c;
    public:
        Elf(int r, int c): _r(r), _c(c) {}

        int row() { return _r; }
        int col() { return _c; }

        int nextr() { return next_r; }
        int nextc() { return next_c; }

        void setr(int r) {_r = r;}
        void setc(int c) {_c = c;}

        bool get_pos(vector<vector<int>>& grid, int& r, int& c, int m) {
            next_r = r;
            next_c = c;
            /* Check elf exists */
            bool exist = false;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if ((i != 0 || j != 0) && grid[r+i][c+j] == -1) exist = true;
                }
            }
            if (!exist) return false;

            /* Check north */
            if (m <= 0 && grid[r-1][c-1] != -1 && grid[r-1][c] != -1 && grid[r-1][c+1] != -1) {
                r--;
            /* Check south */
            } else if (m <= 1 && grid[r+1][c-1] != -1 && grid[r+1][c] != -1 && grid[r+1][c+1] != -1) {
                r++;
            /* Check west */
            } else if (m <= 2 && grid[r-1][c-1] != -1 && grid[r][c-1] != -1 && grid[r+1][c-1] != -1) {
                c--;
            /* Check east */
            } else if (m <= 3 && grid[r-1][c+1] != -1 && grid[r][c+1] != -1 && grid[r+1][c+1] != -1) {
                c++;
            }

            if (m != 0 && next_r == r && next_c == c) {
                /* Check north */
                if (grid[r-1][c-1] != -1 && grid[r-1][c] != -1 && grid[r-1][c+1] != -1) {
                    r--;
                /* Check south */
                } else if (grid[r+1][c-1] != -1 && grid[r+1][c] != -1 && grid[r+1][c+1] != -1) {
                    r++;
                /* Check west */
                } else if (grid[r-1][c-1] != -1 && grid[r][c-1] != -1 && grid[r+1][c-1] != -1) {
                    c--;
                /* Check east */
                } else if (grid[r-1][c+1] != -1 && grid[r][c+1] != -1 && grid[r+1][c+1] != -1) {
                    c++;
                }
            }
                
            if (next_r == r && next_c == c) return false;

            next_r = r;
            next_c = c;
            return true;
        }
};

bool move(vector<vector<int>>& grid, vector<int>& rowcount, vector<int>& colcount, vector<Elf*> elves, int m) {
    /* Get each elves proposed position */
    int moved = false;
    Elf* elf = nullptr;
    for (int i = 0; i < (int)elves.size(); i++) {
        elf = elves.at(i);
        int r = elf->row(), c = elf->col();
        if (elf->get_pos(grid, r, c, m%4)) {
            grid[r][c]++;
        }
    }

    /* Move elves who have no competitors */
    for (int i = 0; i < (int)elves.size(); i++) {
        elf = elves.at(i);
        int r = elf->row(), c = elf->col();
        int new_r = elf->nextr(), new_c = elf->nextc();
        if (grid.at(new_r).at(new_c) == 1) {
            moved = true;
            grid.at(new_r).at(new_c) = -1;
            grid.at(r).at(c) = 0;
            elf->setr(new_r);
            elf->setc(new_c);
            rowcount.at(r)--;
            colcount.at(c)--;
            rowcount.at(new_r)++;
            colcount.at(new_c)++;
        } else if (grid.at(new_r).at(new_c) > 1) {
            /* Reset proposed positions */
            grid.at(new_r).at(new_c) = 0;
        }
    }

    return moved;
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    vector<vector<int>> grid(GRIDSIZE, vector<int>(GRIDSIZE, 0));
    /* 0 represents empty space */
    /* -1 represents an elf */
    /* Positive numbers represent # elves who want to move there */
    int length = line.length();
    int rows = 0;
    vector<int> rowcount(GRIDSIZE, 0);
    vector<int> colcount(GRIDSIZE, 0);
    vector<Elf*> elves;

    while (!file.eof()) {
        for (int i = 0; i < length; i++) {
            if (line[i] == '#') {
                elves.push_back(new Elf(rows+START, i+START));
                grid[rows+START][i+START] = -1;
                rowcount[rows+START]++;
                colcount[i+START]++;
            }
        }
        rows++;
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }
    
    bool moved = true;
    int moves = 0;
    while (moved) {
        moved = move(grid, rowcount, colcount, elves, moves);
        moves++;
    }
    cout << moves << endl;

    file.close();
    return 0;
}
