#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
using std::vector; using std::queue;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 165 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");
    
    vector<string> line;
    int height = 0;

    vector<int> node = {-1, -1, 0};

    char thisLine[MAX_LENGTH];
    file.getline(thisLine, MAX_LENGTH);
    line.push_back("");
    line[height].assign(thisLine);
    int length = line[height].length();

    while (!file.eof()) {
        for (int i = 0; i < length; i++) {
            if (line[height][i] == 'S') {
                node[0] = height;
                node[1] = i;
                line[height][i] = 'a';
            }
        }
        
        // Prepare for next line
        height++;
        char thisLine[MAX_LENGTH];
        file.getline(thisLine, MAX_LENGTH);
        line.push_back("");
        line[height].assign(thisLine);
    }

    bool ** visited = new bool*[height];
    for (int i = 0; i < height; i++) visited[i] = new bool[length];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < length; j++) {
            visited[i][j] = false;
        }
    }
    visited[node[0]][node[1]] = true;

    cout << "start: " << node[0] << ", " << node[1] << endl;

    queue<vector<int>> next;
    next.push(node);
    bool found = false;

    while (!next.empty() && !found) {
        node = next.front();
        next.pop();
        char letter = line[node[0]][node[1]];
        char compare;
        int h = node[2];

        // Add adjacent tiles to queue
        for (int i = 0; i < 4; i++) {
            int r = node[0];
            int c = node[1];
            switch (i) {
                case 0:
                    r--;
                    break;
                case 1:
                    r++;
                    break;
                case 2:
                    c--;
                    break;
                case 3:
                    c++;
                    break;
            }
            
            // Check valid tile
            if ((r < height && r >= 0) && (c >= 0 && c < length)) {
                compare = line[r][c];

                //if (compare == letter-1 || compare == letter || compare == letter+1) {
                if (compare <= letter+1 && compare != 'E') {
                    if (!visited[r][c]) {
                        next.push({r, c, h+1});
                        visited[r][c] = true;
                    }
                } else if (compare == 'E' && (letter == 'y' || letter == 'z')) {
                    cout << "end: " << r << ", " << c;
                    cout << endl << "length: " << h+1 << endl;
                    found = true;
                }
            }
        }
    }


    for (int i = 0; i < height; i++) delete [] visited[i];
    delete [] visited;

    file.close();
    return 0;
}
