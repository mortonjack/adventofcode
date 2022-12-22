#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 5700 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    vector<string> board;
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    while (line[0] == ' ' || line[0] == '.' || line[0] == '#') {
        board.push_back(line);
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int row = 0, column = 0, direction = 0;
    for (int i = 0; i < (int)board[0].length(); i++) {
        if (board[0][i] == '.') {
            column = i;
            break;
        }
    }

    cout << "Opening at " << row+1 << ", " << column+1 << endl;

    // Follow instructions
    /* 
     * 0 for right
     * 1 for down
     * 2 for left
     * 3 for up
     */

    bool getdir = false;

    for (int i = 0; i < (int)line.length(); i++) {
        if (line[i] == 'R') {
            direction++;
            direction %= 4;
            getdir = false;
        } else if (line[i] == 'L') {
            direction--;
            getdir = false;
            if (direction == -1) direction = 3;
        } else if (!getdir) {
            int movecount = stoi(line.substr(i, 12));
            getdir = true;
            // Move in said direction
            for (int j = 0; j < movecount; j++) {
                int next_r = row;
                int next_c = column;
                switch (direction) {
                    case 0:
                        next_c++;
                        if (next_c > (int)board[next_r].size() || board[next_r][next_c] == ' ' || board[next_r][next_c] == '\0') {
                            int k = next_c-1;
                            while (k >= 0 && board[next_r][k] != ' ' && board[next_r][k] != '\0') k--;
                            next_c = k+1;
                        }
                        break;
                    case 1:
                        next_r++;
                        if (next_r > (int)board.size() || next_c >= (int)board[next_r].size() || board[next_r][next_c] == ' ' || board[next_r][next_c] == '\0') {
                            int k = next_r-1;
                            while (k >= 0 && (int)board[k].size() > next_c && board[k][next_c] != ' ' && board[k][next_c] != '\0') k--;
                            next_r = k+1;
                        }
                        break;
                    case 2:
                        next_c--;
                        if (next_c < 0 || board[next_r][next_c] == ' ' || board[next_r][next_c] == '\0') {
                            int k = next_c+1;
                            while (k <= (int)board[next_r].size() && board[next_r][k] != ' ' && board[next_r][k] != '\0') k++;
                            next_c = k-1;
                        }
                        break;
                    case 3:
                        next_r--;
                        if (next_r < 0 || next_c >= (int)board[next_r].size() || board[next_r][next_c] == ' ' || board[next_r][next_c] == '\0') {
                            int k = next_r+1;
                            while (k <= (int)board.size() && (int)board[k].size() > next_c && board[k][next_c] != ' ' && board[k][next_c] != '\0') k++;
                            next_r = k-1;
                        }
                        break;
                }

                // Check for wall
                if (board[next_r][next_c] == '#') {
                    break;
                }

                // Update coords
                row = next_r;
                column = next_c;
            }
        }
    }

    cout << "Current position: " << row+1 << ", " << column+1 << " facing " << direction << endl;
    cout << "Answer: " << (1000*(1+row)) + (4*(1+column)) + direction << endl;

    file.close();
    return 0;
}
