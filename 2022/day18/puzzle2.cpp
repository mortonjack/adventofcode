#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 15 // max length of lines

void dfs_from_outside(vector<vector<vector<int>>>& cubes, int x, int y, int z) {
    if (cubes[x][y][z] != -1) return;
    cubes[x][y][z] = -2;
    for (int l = 0; l < 6; l++) {
        int i, j, k;
        if (l < 2) i = l == 0 ? 1 : -1;
        else i = 0;
        if (l >=2 && l < 4) j = l == 2 ? 1 : -1;
        else j = 0;
        if (l >= 4) k = l == 4 ? 1 : -1;
        else k = 0;
        
        if (x+i >= 0 && x+i < 25 && y+j >= 0 && y+j < 25 && z+k >= 0 && z+k < 25 && cubes[x+i][y+j][z+k] != -2) {
            dfs_from_outside(cubes, x+i, y+j, z+k);
        }
        
    }
}

bool dfs(vector<vector<vector<int>>>& cubes, int x, int y, int z) {
    if (x < 0 || x > 24 || y < 0 || y > 24 || z < 0 || z > 24) {
        cout << "Error: " << x << "," << y << "," << z << " in dfs search\n";
        return false;
    }
    if (cubes[x][y][z] > 0) {
        cout << x << "," << y << "," << z << " in dfs search\n";
        return false;
    }
    if (cubes[x][y][z] == -2) return true;
    //if (cubes[x][y][z] == -3) return false;
    // Return true if on the edge
    if (x == 0 || x == 24 || y == 0 || y == 24 || z == 0 || z == 24) {
        cubes[x][y][z] = -2;
        return true;
    }

    cubes[x][y][z] = -4;

    // Look for empty air pockets
    for (int l = 0; l < 6; l++) {
        int i, j, k;
        if (l < 2) i = l == 0 ? 1 : -1;
        else i = 0;
        if (l >=2 && l < 4) j = l == 2 ? 1 : -1;
        else j = 0;
        if (l >= 4) k = l == 4 ? 1 : -1;
        else k = 0;
            if (cubes[x+i][y+j][z+k] == -3) {
                cubes[x][y][z] = -3;
                return false;
            }
            if (cubes[x+i][y+j][z+k] == -2) {
                cubes[x][y][z] = -2; // -2 == access to air
                return true;
            } else if (cubes[x+i][y+j][z+k] < 0 && cubes[x+i][y+j][z+k] != -4) {
                if (dfs(cubes, x+i, y+j, z+k)) {
                    cubes[x][y][z] = -2;
                    return true;
                /*} else {
                    cubes[x][y][z] = -3;
                    return false;*/
                }
            }
    }
    cubes[x][y][z] = -3;
    return false;
}

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
    /*for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 25; j++) {
            for (int k = 0; k < 25; k++) {
                if (cubes[i][j][k] == -1) dfs(cubes, i, j, k);
            }
        }
    }*/
    for (int i = 0; i < 25; i++) {
        dfs_from_outside(cubes, i, 0, 0);
    }

    for (int x = 0; x < 25; x++) {
        for (int y = 0; y < 25; y++) {
            for (int z = 0; z < 25; z++) {
       /*         if (cubes[x][y][z] == -1) {
                    // Unchecked air. Could this connect out?
                    dfs(cubes, x, y, z);
                }*/
//                cout << x << "," << y << "," << z << endl;
                if (cubes[x][y][z] == 1) {
                    // Check all faces
                    if (x != 0 && cubes[x-1][y][z] == -2) {
                        count++;
                    }
                    else if (x == 0) {
                        count++;
                    }
                    if (x < 24 && cubes[x+1][y][z] == -2) {
                        count++;
                    } else if (x == 24) count++;
                    if (y != 0 && cubes[x][y-1][z] == -2) {
                        count++;
                    }
                    else if (y == 0) {
                        count++;
                    }
                    if (y < 24 && cubes[x][y+1][z] == -2) {
                        count++;
                    } else if (y == 24) count++;
                    if (z != 0 && cubes[x][y][z-1] == -2) {
                            count++;
                    }
                    else if (z == 0) {
                        count++;
                    }
                    if (z < 24 && cubes[x][y][z+1] == -2) {
                        count++;
                    } else if (z == 24) count++;
                }
            }
        }
    }
    cout << count << endl;

    file.close();
    return 0;
}
