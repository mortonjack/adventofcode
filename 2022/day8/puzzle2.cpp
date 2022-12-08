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
    vector<string> treeline;

    char tallest[MAX_LENGTH-1][MAX_LENGTH-1][4];
    int view_dist[MAX_LENGTH-1][MAX_LENGTH-1][4];
    // [i][j][0] = up
    // [i][j][1] = down
    // [i][j][2] = left
    // [i][j][3] = right

    while (!file.eof()) {
        treeline.push_back(line);
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    for (int i = 0; i < MAX_LENGTH-1; i++) {
        view_dist[0][i][0] = 0;
        view_dist[MAX_LENGTH-2][i][1] = 0;
        view_dist[i][0][2] = 0;
        view_dist[i][MAX_LENGTH-2][3] = 0;

        tallest[0][i][0]  = '0';  // up
        tallest[MAX_LENGTH-2][i][1] = '0'; // down
        tallest[i][0][2]  = '0';  // left
        tallest[i][MAX_LENGTH-2][3] = '0'; // right
         
        // Initialise 2nd most outer ring
        if (i != 0 && i != MAX_LENGTH-2) {
            tallest[1][i][0] = treeline[0][i];
            tallest[MAX_LENGTH-3][i][1] = treeline[MAX_LENGTH-2][i];
            tallest[i][1][2] = treeline[i][0];
            tallest[i][MAX_LENGTH-3][3] = treeline[i][MAX_LENGTH-2];
        }
    }

    for (int i = 1; i < MAX_LENGTH-2; i++) {
        for (int j = 1; j < MAX_LENGTH-2; j++) {
            int r, c;

            // Establish free from above
            r = i;
            c = j;
            if (treeline[r][c] > tallest[r][c][0]) {
                tallest[r+1][c][0] = treeline[r][c];
            } else {
                tallest[r+1][c][0] = tallest[r][c][0];
            }

            view_dist[r][c][0] = treeline[r][c] > treeline[r-1][c] ? view_dist[r-1][c][0] + 1 : 1;

            // Establish open from below
            r = MAX_LENGTH-2-i;
            c = MAX_LENGTH-2-j;
            if (treeline[r][c] > tallest[r][c][1]) {
                tallest[r-1][c][1] = treeline[r][c];
            } else {
                tallest[r-1][c][1] = tallest[r][c][1];
            }

            view_dist[r][c][1] = treeline[r][c] > treeline[r+1][c] ? view_dist[r+1][c][1] + 1 : 1;

            // Establish open from left
            r = i;
            c = j;
            if (treeline[r][c] > tallest[r][c][2]) {
                tallest[r][c+1][2] = treeline[r][c];
            } else {
                tallest[r][c+1][2] = tallest[r][c][2];
            }

            view_dist[r][c][2] = treeline[r][c] > treeline[r][c-1] ? view_dist[r][c-1][2] + 1 : 1;

            // Establish open from right
            r = MAX_LENGTH-2-i;
            c = MAX_LENGTH-2-j;
            if (treeline[r][c] > tallest[r][c][3]) {
                tallest[r][c-1][3] = treeline[r][c];
            } else {
                tallest[r][c-1][3] = tallest[r][c][3];
            }

            view_dist[r][c][3] = treeline[r][c] > treeline[r][c+1] ? view_dist[r][c+1][3] + 1 : 1;
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

    // Since I think I've misunderstood the question, I'm going to attempt a brute-force
    // approach for how I think it was meant to be interpreted, just to check.
    for (int i = 1; i < MAX_LENGTH-2; i++) {
        for (int j = 1; j < MAX_LENGTH-2; j++) {
            // Calculate up
            int up = 1;
            for (int k = i-1; k > 0; k--) {
                if (treeline[k][j] >= treeline[i][j]) break;
                up++;
            }
            view_dist[i][j][0] = up;

            int down = 1;
            for (int k = i+1; k < MAX_LENGTH-2; k++) {
                if (treeline[k][j] >= treeline[i][j]) break;
                down++;
            }
            view_dist[i][j][1] = down;

            int left = 1;
            for (int k = j-1; k > 0; k--) {
                if (treeline[i][k] >= treeline[i][j]) break;
                left++;
            }
            view_dist[i][j][2] = left;

            int right = 1;
            for (int k = j+1; k < MAX_LENGTH-2; k++) {
                if (treeline[i][k] >= treeline[i][j]) break;
                right++;
            }
            view_dist[i][j][3] = right;
        }
    }

    count = 0;
    int r, c;

    for (int i = 1; i < MAX_LENGTH-2; i++) {
        for (int j = 1; j < MAX_LENGTH-2; j++) {
            int max = 1;
            cout << "(" << view_dist[i][j][0] << " ";
            cout << view_dist[i][j][1] << " ";
            cout << view_dist[i][j][2] << " ";
            cout << view_dist[i][j][3] << ") "; 
            if (view_dist[i][j][0] != 0) max *= view_dist[i][j][0];
            if (view_dist[i][j][1] != 0) max *= view_dist[i][j][1];
            if (view_dist[i][j][2] != 0) max *= view_dist[i][j][2];
            if (view_dist[i][j][3] != 0) max *= view_dist[i][j][3];
            if (max > count) {
                r = i;
                c = j;
                count = max;
            }
        }
        cout << endl;
    }

    cout << "The highest viewing distance is " << count << " at " << r << ',' << c << endl;

    file.close();
    return 0;
}
