#include <iostream>
#include <string>
#include <fstream>
#include <array>
using std::array;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 15 // max length of lines

void convert(string& line, array<int, 4>& bounds) {
    int j = 0; // index of bounds
    bounds[0] = 0;
    for (auto& c: line) {
        if (c == '-' || c == ',') bounds[++j] = 0;
        else {
            bounds[j] *= 10;
            bounds[j] += (int) (c - '0');
        }
    }
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    // Count contained pairs
    int count = 0;
    // Store numbers from input
    array<int, 4> bounds = {0};

    while (!file.eof()) {
        // Read numbers for this line
        convert(line, bounds);

        // Increase count
        if ((bounds[1] >= bounds[2] && bounds[0] <= bounds[2])
         || (bounds[1] >= bounds[3] && bounds[0] <= bounds[3])
         || (bounds[1] <= bounds[3] && bounds[0] >= bounds[2])) {
            count++;
        }

        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    cout << "In " << count << " assignment pairs, one range contains the other" << endl;

    file.close();
    return 0;
}
