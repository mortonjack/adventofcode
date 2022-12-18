#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 15 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    vector<vector<vector<int>>> cubes(25, vector<vector<int>>(25, vector<int>(25, -1)));

    while (!file.eof()) {
        int x, y, z;
        x = stoi(line.substr(0, 2));
        int i =0;
        while (line[i] != ',') i++;
        i++;
        y = stoi(line.substr(i, 2));
        while (line[i] != ',') i++;
        i++;
        z = stoi(line.substr(i, 2));
//        cout << x << "," << y << "," << z << endl;
        cubes[x][y][z] = 1;
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    int count = 0;

    for (int x = 0; x < 25; x++) {
        for (int y = 0; y < 25; y++) {
            for (int z = 0; z < 25; z++) {
//                cout << x << "," << y << "," << z << endl;
                if (cubes[x][y][z] != -1) {
                    // Check all faces
                    if (x != 0 && cubes[x-1][y][z] == -1) count++;
                    else if (x == 0) count++;
                    if (x < 24 && cubes[x+1][y][z] == -1) count++;
                    else if (x == 24) count++;
                    if (y != 0 && cubes[x][y-1][z] == -1) count++;
                    else if (y == 0) count++;
                    if (y < 24 && cubes[x][y+1][z] == -1) count++;
                    else if (y == 24) count++;
                    if (z != 0 && cubes[x][y][z-1] == -1) count++;
                    else if (z == 0) count++;
                    if (z < 24 && cubes[x][y][z+1] == -1) count++;
                    else if (z == 24) count++;
                }
            }
        }
    }
    cout << count << endl;

    file.close();
    return 0;
}
