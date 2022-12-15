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
    int sum = 0;

    int y = 2000000;
    //y = 10;

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
        cout << "Line " << i << " distance is " << distance << " with " << remaining << " remaining" << endl;

        if (remaining > 0) {
            // Calculate x values on either side
            vector<int> a;
            a.push_back(coords[i][0] - remaining);
            a.push_back(coords[i][0] + remaining);
            
            // Look for existing boundary
            bool exist = false;
            for (int j = 0; j < (int)boundaries.size(); j++) {
                if (boundaries[j][0] <= a[0] && boundaries[j][1] >= a[0]) {
                    cout << boundaries[j][0] << " < " << a[0] << " < " << boundaries[j][1] << ", right boundary " << a[1] << endl;
                    // Left boundary exists inside this boundary
                    exist = true;
                    // Update right boundary
                    boundaries[j][1] = boundaries[j][1] > a[1] ? boundaries[j][1] : a[1];
                    // Exit loop
                    break;
                } else if (boundaries[j][1] >= a[1] && boundaries[j][0] <= a[1]) {
                    cout << boundaries[j][0] << " < " << a[1] << " < " << boundaries[j][1] << ", left boundary " << a[0] << endl;
                    // Right boundary exists inside this boundary
                    exist = true;
                    // Update left boundary
                    boundaries[j][0] = boundaries[j][0] < a[0] ? boundaries[j][0] : a[0];
                    // Exit loop
                    break;
                } else if (a[0] <= boundaries[j][0] && a[1] >= boundaries[j][0]) {
                    // This left boundary exists inside boundary
                    cout << a[0] << " < " << boundaries[j][0] << " < " << a[1] << ", right boundary " << boundaries[j][1] << endl;
                    exist = true;
                    boundaries[j][0] = a[0];
                    boundaries[j][1] = boundaries[j][1] > a[1] ? boundaries[j][1] : a[1];
                } else if (a[1] >= boundaries[j][1] && a[0] <= boundaries[j][1]) {
                    // This right boundary exists inside boundary
                    cout << a[0] << " < " << boundaries[j][1] << " < " << a[1] << ", left boundary " << boundaries[j][0] << endl;
                    exist = true;
                    boundaries[j][1] = a[1];
                    boundaries[j][0] = boundaries[j][0] < a[0] ? boundaries[j][0] : a[0];
                }
            }
            
            if (!exist) {
                cout << "New boundary " << a[0] << " -> " << a[1] << endl;
                boundaries.push_back(a);
            }

            // Check updated boundary doesn't conflict with existing boundary
            for (int j = 0; j < (int)boundaries.size(); j++) {
                for (int k = 0; k < (int)boundaries.size(); k++) {
                    if (j != k) {
                        if (boundaries[j][0] <= boundaries[k][0] && boundaries[j][1] >= boundaries[k][0]) {
                    cout << boundaries[j][0] << " < " << boundaries[k][0] << " < " << boundaries[j][1] << ", right boundary " << boundaries[k][1] << endl;
                            // Left boundary exists inside this boundary
                            boundaries[j][1] = boundaries[j][1] > boundaries[k][1] ? boundaries[j][1] : boundaries[k][1];
                            // Delete existing thingo
                            boundaries.erase(boundaries.begin()+k);
                            k--;
                        } else if (boundaries[j][1] >= boundaries[k][1] && boundaries[j][0] <= boundaries[k][1]) {
                    cout << boundaries[j][0] << " < " << boundaries[k][1] << " < " << boundaries[j][1] << ", left boundary " << boundaries[k][0] << endl;
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

    for (int i = 0; i < (int)boundaries.size(); i++) {
        cout << boundaries[i][0] << " -> " << boundaries[i][1] << endl;
        sum += boundaries[i][1]-boundaries[i][0];
    }
    cout << sum << endl;

    file.close();
    return 0;
}
