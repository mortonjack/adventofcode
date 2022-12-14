#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 300 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");

    /* Cave layout: 
     * # = rock
     * . = air
     * + = source
     * o = sand (rest)
     */
    vector<vector<char>> cave(200, vector<char>(1000, '.'));
    cave[0][500] = '+';

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int length = line.length();

    while (!file.eof()) {
        int prevx = -1, prevy = -1;
        int x = -1, y = -1;
        for (int i = 0; i < length; i++) {
            if (x == -1) {
                // Read x coord
                x = stoi(line.substr(i, i+3));
                i += 3;
            } else if (y == -1) {
                // Read y coord
                y = stoi(line.substr(i, i+3));
                i += 2;

                // Add to graph
                if (prevx != -1) {
                    if (prevx == x) {
                        int inc = y < prevy ? -1 : 1;
                        for (int k = prevy; k != y + inc; k += inc) {
                            cave[k][x] = '#';
                        }
                    } else {
                        int inc = x < prevx ? -1 : 1;
                        for (int k = prevx; k != x + inc; k += inc) {
                            cave[y][k] = '#';
                        }
                    }
                }

                // Update prev
                prevx = x;
                prevy = y;
            } else if (line[i] == '>') {
                // Prepare for next coord
                x = -1;
                y = -1;
                i++;
            }
        }
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }

    // Come to rest!
    int x = 500, y = 0;
    int rested = 0;
    while (y < 199) {
        cout << x << "," << y << endl;
        if (cave.at(y+1).at(x) == '.') y++;
        else if (cave.at(y+1).at(x-1) == '.') {
            y++;
            x--;
        } else if (cave.at(y+1).at(x+1) == '.') {
            y++;
            x++;
        } else {
            rested++;
            cave[y][x] = 'o';
            x = 500;
            y = 0;
        }
    }

    // Display output
    for (int i = 0; i < 200; i++) {
        for (int j = 450; j < 600; j++) {
            cout << cave[i][j];
        }
        cout << endl;
    }
    cout << endl << "There are " << rested << " rested sand drops\n" << endl;

    file.close();
    return 0;
}
