#include <iostream>
#include <string>
#include <fstream>
using std::cout; using std::endl;
using std::string; using std::ifstream;

int value(char c) {
    if (c <= 'z' && c >= 'a') return c - 'a' + 1;
    else return c - 'A' + 27;
}

int main() {
    ifstream file;
    file.open("input.txt");
    
    char thisLine[50];
    file.getline(thisLine, 49);
    string line;
    line.assign(thisLine);
    int length = line.length(); // length of line

    int counter = 0;
    int lastSeen[26+26+1]; // last seen in first segment
    for (int i = 0; i < 26*2 + 1; i++) {
        lastSeen[i] = -1;
    }
    int sum = 0;

    while (!file.eof()) {
        // Look for duplicate
        for (int i = 0; i < length/2; i++) {
            lastSeen[value(line[i])] = counter;
        }
        for (int i = length/2; i < length; i++) {
            if (lastSeen[value(line[i])] == counter) {
                sum += value(line[i]);
                i = length; // don't double-count
            }
        }

        // Prepare for next loop
        file.getline(thisLine, 49);
        line.assign(thisLine);
        length = line.length();
        counter++;
    }

    cout << "Total sum: " << sum << endl;

    file.close();
}
