#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>
using std::set; using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 5 // max length of lines

bool out_of_pos(vector<int> tail, vector<int> head) {
    if (tail[0] == head[0] || tail[0] == head[0]+1 || tail[0] == head[0]-1) {
        if (tail[1] == head[1] || tail[1] == head[1]+1 || tail[1] == head[1]-1) {
            return false;
        }
    }
    return true;
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int length = line.length();
    
    vector<vector<int>> ropes(10, {0, 0});
    set<vector<int>> pos;
    pos.insert(ropes[9]);

    while (!file.eof()) {

        char direction = line[0];
        int moves = stoi(line.substr(2, line.length()));

        for (int i = 0; i < moves; i++) {
            switch (direction) {
                case 'U':
                    ropes[0][0]++;
                    break;
                case 'D':
                    ropes[0][0]--;
                    break;
                case 'L':
                    ropes[0][1]--;
                    break;
                case 'R':
                    ropes[0][1]++;
                    break;
            }

            // Update tail position
            for (int j = 1; j <= 9; j++) {
                if (out_of_pos(ropes[j], ropes[j-1])) {
                    if (ropes[j][1] == ropes[j-1][1]) {
                        // Same col
                        if (ropes[j-1][0] > ropes[j][0]) ropes[j][0]++;
                        else ropes[j][0]--;
                    } else if (ropes[j][0] == ropes[j-1][0]) {
                        // Same row
                        if (ropes[j-1][1] > ropes[j][1]) ropes[j][1]++;
                        else ropes[j][1]--;
                    } else {
                        // Diagonal move
                        if (ropes[j-1][0] > ropes[j][0]) ropes[j][0]++;
                        else ropes[j][0]--;
                        if (ropes[j-1][1] > ropes[j][1]) ropes[j][1]++;
                        else ropes[j][1]--;
                    }
                }
            }

            /*
            cout << head[0] << ", " << head[1] << " - ";
            cout << tail[0] << ", " << tail[1] << endl;
            */

            // Add tail position to set
            pos.insert(ropes[9]);
        }
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }

    cout << "unique positions: " << pos.size() << endl;

    file.close();
    return 0;
}
