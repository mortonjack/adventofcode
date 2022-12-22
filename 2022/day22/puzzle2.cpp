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
    vector<vector<string>> squares(6, vector<string>(50, string(50, ' ')));
    vector<string> board; // initial reading
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    while (line[0] == ' ' || line[0] == '.' || line[0] == '#') {
        board.push_back(line);
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    // Turn board into square
    int count = 0;
    for (int i = 0; i < (int)board.size(); i += 50) {
        for (int j = 0; j < (int)board[i].length(); j += 50) {
            if (board[i][j] == '.' || board[i][j] == '#') {
                // A square exists here
                for (int r = 0; r < 50; r++) {
                    for (int c = 0; c < 50; c++) {
                        squares.at(count).at(r).at(c) = board.at(r+i).at(c+j);
                    }
                }
                count++;
            }
        }
    }
    

    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    vector<vector<vector<int>>> faces(6, vector<vector<int>>(4, vector<int>(2, -1)));
    // Faces[i][0] = right
    // Faces[i][1] = down
    // Faces[i][2] = left
    // Faces[i][3] = up
    // Faces[i][j][0] = face
    // Faces[i][j][1] = rotation
    // Rotations: amount to the right
    //
    // Hard coding this cos idk math!!!
    //
    // I regret this but i gtg soon so no time to turn back lol
    //
    // 1's face (0)
    faces[0][0][0] = 1;
    faces[0][0][1] = 0;
    faces[0][1][0] = 2;
    faces[0][1][1] = 0;
    faces[0][2][0] = 3;
    faces[0][2][1] = 2;
    faces[0][3][0] = 5;
    faces[0][3][1] = 1;
    // 2's face (1)
    faces[1][0][0] = 4;
    faces[1][0][1] = 2;
    faces[1][1][0] = 2;
    faces[1][1][1] = 1;
    faces[1][2][0] = 0;
    faces[1][2][1] = 0;
    faces[1][3][0] = 5;
    faces[1][3][1] = 0;
    // 3's face (2)
    faces[2][0][0] = 1;
    faces[2][0][1] = 3;
    faces[2][1][0] = 4;
    faces[2][1][1] = 0;
    faces[2][2][0] = 3;
    faces[2][2][1] = 3;
    faces[2][3][0] = 0;
    faces[2][3][1] = 0;
    // 4's face (3)
    faces[3][0][0] = 4;
    faces[3][0][1] = 0;
    faces[3][1][0] = 5;
    faces[3][1][1] = 0;
    faces[3][2][0] = 0;
    faces[3][2][1] = 2;
    faces[3][3][0] = 2;
    faces[3][3][1] = 1;
    // 5's face (4)
    faces[4][0][0] = 1;
    faces[4][0][1] = 2;
    faces[4][1][0] = 5;
    faces[4][1][1] = 1;
    faces[4][2][0] = 3;
    faces[4][2][1] = 0;
    faces[4][3][0] = 2;
    faces[4][3][1] = 0;
    // 6's face (5)
    faces[5][0][0] = 4;
    faces[5][0][1] = 3;
    faces[5][1][0] = 1;
    faces[5][1][1] = 0;
    faces[5][2][0] = 0;
    faces[5][2][1] = 3;
    faces[5][3][0] = 3;
    faces[5][3][1] = 0;

    // Follow instructions
    /* 
     * 0 for right
     * 1 for down
     * 2 for left
     * 3 for up
     */
    
    // Faces[i][0] = right
    // Faces[i][1] = down
    // Faces[i][2] = left
    // Faces[i][3] = up
    // Faces[i][j][0] = face
    // Faces[i][j][1] = rotation
    // Rotations: amount to the right

    int row = 0, column = 0, direction = 0, square = 0;
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
            // move in said direction
            for (int j = 0; j < movecount; j++) {
                int next_r = row, next_c = column, next_square = square, new_d = direction;
                switch (direction) {
                    case 0: /* right */
                        next_c++;
                        if (next_c == 50) {
                            next_square = faces[square][0][0];
                            next_c = 0;
                            /* Apply rotation */
                            for (int k = 0; k < faces[square][0][1]; k++) {
                                int old_r = next_r, old_c = next_c;
                                next_r = old_c;
                                next_c = 49-old_r;
                                new_d++;
                                new_d %= 4;
                            }
                        }
                        break;
                    case 1: /* down */
                        next_r++;
                        if (next_r == 50) {
                            next_square = faces[square][1][0];
                            next_r = 0;
                            /* Apply rotation */
                            for (int k = 0; k < faces[square][1][1]; k++) {
                                int old_r = next_r, old_c = next_c;
                                next_r = old_c;
                                next_c = 49-old_r;
                                new_d++;
                                new_d %= 4;
                            }
                        }
                        break;
                    case 2: /* left */
                        next_c--;
                        if (next_c == -1) {
                            next_square = faces[square][2][0];
                            next_c = 49;
                            /* Apply rotation */
                            for (int k = 0; k < faces[square][2][1]; k++) {
                                int old_r = next_r, old_c = next_c;
                                next_r = old_c;
                                next_c = 49-old_r;
                                new_d++;
                                new_d %= 4;
                            }
                        }
                        break;
                    case 3: /* up */
                        next_r--;
                        if (next_r == -1) {
                            next_square = faces[square][3][0];
                            next_r = 49;
                            /* Apply rotation */
                            for (int k = 0; k < faces[square][3][1]; k++) {
                                int old_r = next_r, old_c = next_c;
                                next_r = old_c;
                                next_c = 49-old_r;
                                new_d++;
                                new_d %= 4;
                            }
                        }
                        break;
                }

                // Check for wall
                if (squares.at(next_square).at(next_r).at(next_c) == '#') break;

                // Update coords
                row = next_r;
                column = next_c;
                square = next_square;
                direction = new_d % 4;
            }
        }
    }

    square++;
    if (square == 3) row += 50;
    else if (square == 4 || square == 5) row += 100;
    else if (square == 6) row += 150;
    if (square == 1 || square == 3 || square == 5) column += 50;
    else if (square == 2) column += 100;
    cout << "Current position: " << row+1 << ", " << column+1 << " facing " << direction << endl;
    cout << "Answer: " << (1000*(1+row)) + (4*(1+column)) + direction << endl;

    file.close();
    return 0;
}
