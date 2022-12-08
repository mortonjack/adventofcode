#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 100 // max length of lines

/* Small issue: Something isn't initialising correctly so the answer
 * can vary. */

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int length = line.length();
    vector<string> treeline;

    char tallest[MAX_LENGTH-1][MAX_LENGTH-1][4];
    // [i][j][0] = up
    // [i][j][1] = down
    // [i][j][2] = left
    // [i][j][3] = right

    while (!file.eof()) {
        treeline.push_back(line);
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }

    for (int i = 0; i < MAX_LENGTH-1; i++) {
        tallest[0][i][0]  = '0';  // up
        tallest[MAX_LENGTH-2][i][1] = '0'; // down
        tallest[i][0][2]  = '0';  // left
        tallest[i][MAX_LENGTH-2][3] = '0'; // right
        if (i != 0 && i != MAX_LENGTH-2) {
            tallest[1][i][0] = treeline[0][i];
            tallest[MAX_LENGTH-3][i][1] = treeline[MAX_LENGTH-2][i];
            tallest[i][1][2] = treeline[i][0];
            tallest[i][MAX_LENGTH-3][3] = treeline[i][MAX_LENGTH-2];
        }
    }

    for (int i = 1; i < MAX_LENGTH-2; i++) {
        for (int j = 1; j < MAX_LENGTH-2; j++) {

            // Establish free from above
            if (treeline[i][j] > tallest[i][j][0]) {
                tallest[i+1][j][0] = treeline[i][j];
            } else tallest[i+1][j][0] = tallest[i][j][0];

            // Establish open from below
            if (treeline[MAX_LENGTH-2-i][MAX_LENGTH-2-j] > tallest[MAX_LENGTH-2-i][MAX_LENGTH-2-j][1]) {
                tallest[MAX_LENGTH-3-i][MAX_LENGTH-2-j][1] = treeline[MAX_LENGTH-2-i][MAX_LENGTH-2-j];
            } else tallest[MAX_LENGTH-3-i][MAX_LENGTH-2-j][1] = tallest[MAX_LENGTH-2-i][MAX_LENGTH-2-j][1];

            // Establish open from left
            if (treeline[i][j] > tallest[i][j][2]) {
                tallest[i][j+1][2] = treeline[i][j];
            } else tallest[i][j+1][2] = tallest[i][j][2];

            // Establish open from right
            if (treeline[MAX_LENGTH-2-i][MAX_LENGTH-2-j] > tallest[MAX_LENGTH-2-i][MAX_LENGTH-2-j][3]) {
                tallest[MAX_LENGTH-2-i][MAX_LENGTH-3-j][3] = treeline[MAX_LENGTH-2-i][MAX_LENGTH-2-j];
            } else tallest[MAX_LENGTH-2-i][MAX_LENGTH-3-j][3] = tallest[MAX_LENGTH-2-i][MAX_LENGTH-2-j][3];

        }
    }

    int count = 0;

    for (int i = 0; i < MAX_LENGTH-1; i++) {
        for (int j = 0; j < MAX_LENGTH-1; j++) {
            if (treeline[i][j] > tallest[i][j][0]
             || treeline[i][j] > tallest[i][j][1]
             || treeline[i][j] > tallest[i][j][2]
             || treeline[i][j] > tallest[i][j][3]) {
                count++;
            } 
        }
    }

    cout << "There are " << count << " trees visible." << endl;

    file.close();
    return 0;
}
