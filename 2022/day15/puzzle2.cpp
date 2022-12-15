#include <iostream>
#include <climits>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 100 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int length = line.length();

    vector<vector<int>> coords;
    // { { sensor x, sensor y, beacon x, beacon y } }

    while (!file.eof()) {
        vector<int> a(4, -1);
        int f = 0;
        for (int i = 0; i < length; i++) {
            if (line[i] == '=') {
                a[f] = stoi( line.substr(i+1, length) );
                f++;
            }
        }

        coords.push_back(a);
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }

    // I'm sure there's a much faster way to do this. I choose to abuse my computer instead.
    // also, this was originally 0 -> 4mil, i just manually narrowed it down after forgetting to
    // output 'y' the first time, then messing up the calculations once or twice more (not using longs)

    for (int y = 2500000; y < 3000000; y++) {
    vector<vector<int>> boundaries;
    for (int i = 0; i < (int)coords.size(); i++) {
        // Calculate distance from sensor to beacon
        int distance;
        if (coords[i][0] < coords[i][2]) distance = coords[i][2]-coords[i][0];
        else distance = coords[i][0]-coords[i][2];
        if (coords[i][1] < coords[i][3]) distance += coords[i][3]-coords[i][1];
        else distance += coords[i][1]-coords[i][3];

        // Calculate distance from beacon to y
        int remaining = coords[i][1] - y;
        if (remaining < 0) remaining *= -1;
        remaining = distance - remaining;

        if (remaining > 0) {
            // Calculate x values on either side
            vector<int> a;
            a.push_back(coords[i][0] - remaining);
            a.push_back(coords[i][0] + remaining);
            if (a[0] < 0) a[0] = 0;
            if (a[1] > 4000000) a[1] = 4000000;
            
            // Look for existing boundary
            bool exist = false;
            for (int j = 0; j < (int)boundaries.size(); j++) {
                if (boundaries[j][0] <= a[0] && boundaries[j][1] >= a[0]) {
                    // Left boundary exists inside this boundary
                    exist = true;
                    // Update right boundary
                    boundaries[j][1] = boundaries[j][1] > a[1] ? boundaries[j][1] : a[1];
                    // Exit loop
                    break;
                } else if (boundaries[j][1] >= a[1] && boundaries[j][0] <= a[1]) {
                    // Right boundary exists inside this boundary
                    exist = true;
                    // Update left boundary
                    boundaries[j][0] = boundaries[j][0] < a[0] ? boundaries[j][0] : a[0];
                    // Exit loop
                    break;
                } else if (a[0] <= boundaries[j][0] && a[1] >= boundaries[j][0]) {
                    // This left boundary exists inside boundary
                    exist = true;
                    boundaries[j][0] = a[0];
                    boundaries[j][1] = boundaries[j][1] > a[1] ? boundaries[j][1] : a[1];
                } else if (a[1] >= boundaries[j][1] && a[0] <= boundaries[j][1]) {
                    // This right boundary exists inside boundary
                    exist = true;
                    boundaries[j][1] = a[1];
                    boundaries[j][0] = boundaries[j][0] < a[0] ? boundaries[j][0] : a[0];
                }
            }
            
            if (!exist) {
                boundaries.push_back(a);
            }

            // Check updated boundary doesn't conflict with existing boundary
            for (int j = 0; j < (int)boundaries.size(); j++) {
                for (int k = 0; k < (int)boundaries.size(); k++) {
                    if (j != k) {
                        if (boundaries[j][0] <= boundaries[k][0] && boundaries[j][1] >= boundaries[k][0]) {
                            // Left boundary exists inside this boundary
                            boundaries[j][1] = boundaries[j][1] > boundaries[k][1] ? boundaries[j][1] : boundaries[k][1];
                            // Delete existing thingo
                            boundaries.erase(boundaries.begin()+k);
                            k--;
                        } else if (boundaries[j][1] >= boundaries[k][1] && boundaries[j][0] <= boundaries[k][1]) {
                            // Right boundary inside this boundary
                            boundaries[j][0] = boundaries[j][0] < boundaries[k][0] ? boundaries[j][0] : boundaries[k][0];
                            // Delete existing thingo
                            boundaries.erase(boundaries.begin()+k);
                            k--;
                        }
                    }
                }
            }
        }
    }
    if (boundaries.size() > 1) {
        cout << boundaries[0][0] << ", " << boundaries[0][1] << " and " ;
        cout << boundaries[1][0] << ", " << boundaries[1][1] << endl;
        cout << y << endl;
        cout << (long long)((long long)boundaries[0][1] + (long long)1)*(long long)4000000 + y << endl;
    }
    }

    file.close();
    return 0;
}
