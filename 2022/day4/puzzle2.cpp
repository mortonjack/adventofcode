#include <iostream>
#include <string>
#include <fstream>
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 15 // max length of lines

void convert(char line[MAX_LENGTH], int length, int bounds[4]) {
    int i = 0; // index of string
    int j = 0; // index of bounds
    while (i != length) {
        if (line[i] == '-' || line[i] == ',') {
            j++;
        } else {
            bounds[j] *= 10;
            bounds[j] += (int) (line[i] - '0');
        }

        i++;
    }
    return;
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int length = line.length();

    // Count contained pairs
    int count = 0;
    // Store numbers from input
    int bounds[4];

    while (!file.eof()) {
        // Initialise bounds to 0
        for (int i = 0; i < 4; i++) {
            bounds[i] = 0;
        }

        // Read numbers for this line
        convert(thisLine, length, bounds);

        // Increase count
        if ((bounds[1] >= bounds[2] && bounds[0] <= bounds[2])
         || (bounds[1] >= bounds[3] && bounds[0] <= bounds[3])
         || (bounds[1] <= bounds[3] && bounds[0] >= bounds[2])) {
            count++;
        }

        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }

    cout << "In " << count << " assignment pairs, one range contains the other" << endl;

    file.close();
    return 0;
}
