#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
using std::unordered_map;
using std::vector; using std::queue;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 125 // max length of lines
/* Regular input */
#define ROWS 27
#define COLS 122
/* Sample input */
//#define ROWS 6
//#define COLS 8

void simulate(vector<vector<int>>& grid) {
    /* Simulates one turn */
    vector<vector<int>> sample_grid(ROWS, vector<int>(COLS, 0));
    for (int i = 1; i < (int)grid.size()-1; i++) {
        for (int j = 1; j < (int)grid.at(i).size()-1; j++) {
            int tile = grid.at(i).at(j);
            /* Check for up */
            if (tile % 10 == 1) {
                if (i != 1) {
                    sample_grid.at(i-1).at(j) += 1;
                } else {
                    sample_grid.at(ROWS-2).at(j) += 1;
                }
                tile -= 1;
            }
            /* Check for down */
            if (tile % 100 == 10) {
                if (i != ROWS-2) {
                    sample_grid.at(i+1).at(j) += 10;
                } else {
                    sample_grid.at(1).at(j) += 10;
                }
                tile -= 10;
            }
            /* Check for left */
            if (tile % 1000 == 100) {
                if (j != 1) {
                    sample_grid.at(i).at(j-1) += 100;
                } else {
                    sample_grid.at(i).at(COLS-2) += 100;
                }
                tile -= 100;
            }
            /* Check for right */
            if (tile % 10000 == 1000) {
                if (j != COLS-2) {
                    sample_grid.at(i).at(j+1) += 1000;
                } else {
                    sample_grid.at(i).at(1) += 1000;
                }
                tile -= 1000;
            }
            /* Update grid value */
            grid.at(i).at(j) = 0;
        }
    }

    /* Copy sample_grid values to grid */
    for (int i = 1; i < (int)grid.size()-1; i++) {
        for (int j = 1; j < (int)grid.at(i).size()-1; j++) {
            grid.at(i).at(j) = sample_grid.at(i).at(j);
        }
    }
    return;
}

void display(vector<vector<int>>& grid) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int tile = grid.at(i).at(j);
            switch (grid.at(i).at(j)) {
                case -1:
                    cout << '#';
                    break;
                case 1:
                    cout << '^';
                    break;
                case 10:
                    cout << 'v';
                    break;
                case 100:
                    cout << '<';
                    break;
                case 1000:
                    cout << '>';
                    break;
                case 0:
                    cout << '.';
                    break;
                default:
                    int count = 0;
                    if (tile % 10 == 1) {
                        count++;
                        tile -= 1;
                    }
                    if (tile % 100 == 10) {
                        count++;
                        tile -= 10;
                    }
                    if (tile % 1000 == 100) {
                        count++;
                        tile -= 100;
                    }
                    if (tile % 10000 == 1000) {
                        count++;
                        tile -= 1000;
                    }
                    cout << count;
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    vector<vector<int>> grid(ROWS, vector<int>(COLS, 0));
    /* -1 = wall
     * 0 = empty space
     * 1 = up
     * 10 = down
     * 100 = left
     * 1000 = right
     * e.g. 1011 = 3 blizzards, right, down, up */
    int row = -1;
    int col;

    while (!file.eof()) {
        row++;
        bool hash = false;
        for (int i = 0; i < (int)line.length(); i++) {
            switch (line.at(i)) {
                case '#':
                    grid.at(row).at(i) = -1;
                    hash = true;
                    break;
                case '^':
                    grid.at(row).at(i) = 1;
                    break;
                case 'v':
                    grid.at(row).at(i) = 10;
                    break;
                case '<':
                    grid.at(row).at(i) = 100;
                    break;
                case '>':
                    grid.at(row).at(i) = 1000;
                    break;
                case '.':
                    if (hash) col = i;
            }
        }
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    int end_row = row;
    int end_col = col;
    int start_row = 0;
    int start_col = 1;
    int turns = 0;

    for (int go = 0; go < 3; go++) {
        cout << "Starting at " << start_row << ", " << start_col << endl;
        cout << "Going to " << end_row << ", " << end_col << endl;
        vector<vector<bool>> possible(ROWS, vector<bool>(COLS, false));
        bool end = false;
        possible.at(start_row).at(start_col) = true;
        while (!end) {
            simulate(grid);
            vector<vector<bool>> new_possible(ROWS, vector<bool>(COLS, false));
            for (int i = 0; i < ROWS; i++) {
                for (int j = 1; j < COLS-1; j++) {
                    if (possible.at(i).at(j)) {
                        // Check surrounding tiles
                        if (grid.at(i).at(j) == 0) new_possible.at(i).at(j) = true;
                        if (i == 0) {
                            if (grid.at(1).at(j) == 0) new_possible.at(1).at(j) = true;
                        } else if (i == ROWS-1) {
                            if (grid.at(ROWS-2).at(j) == 0) new_possible.at(ROWS-2).at(j) = true;
                        } else {
                            if (grid.at(i+1).at(j) == 0) new_possible.at(i+1).at(j) = true;
                            if (grid.at(i-1).at(j) == 0) new_possible.at(i-1).at(j) = true;
                            if (grid.at(i).at(j+1) == 0) new_possible.at(i).at(j+1) = true;
                            if (grid.at(i).at(j-1) == 0) new_possible.at(i).at(j-1) = true;
                        }
                    }
                }
            }
            /* Update possible grid */
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    possible.at(i).at(j) = new_possible.at(i).at(j);
                }
            }
            /* Check end */
            if (possible.at(end_row).at(end_col)) end = true;
            turns++;
        }
        // Update start/end
        cout << "Took " << turns << " minutes." << endl << endl;
        int temp = start_row;
        start_row = end_row;
        end_row = temp;
        temp = start_col;
        start_col = end_col;
        end_col = temp;
    }

    cout << "It takes " << turns << " minutes to leave." << endl;

    file.close();
    return 0;
}
