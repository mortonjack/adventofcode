#include <iostream>
#include <string>
#include <fstream>
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 12 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    char screen[6][40];

    int cycle = 0; // cycle
    int X = 1; // signal strength
    int i = 0; // level

    while (!file.eof()) {
        if (cycle == 40) {
            cycle = 0;
            i++;
        }
        if (line[0] == 'a') {
            if (X == cycle-1 || X == cycle || X == cycle+1) screen[i][cycle] = '#';
            else screen[i][cycle] = '.';
            cycle++;
            if (cycle == 40) {
                cycle = 0;
                i++;
            }
            if (X == cycle-1 || X == cycle || X == cycle+1) screen[i][cycle] = '#';
            else screen[i][cycle] = '.';
            cycle++;
            X += stoi( line.substr(5, line.length()));
        } else {
            if (X == cycle || X == cycle-1 || X == cycle+1) screen[i][cycle] = '#';
            else screen[i][cycle] = '.';
            cycle++;
        }

        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 40; j++) {
            cout << screen[i][j];
        }
        cout << endl;
    }

    file.close();
    return 0;
}
