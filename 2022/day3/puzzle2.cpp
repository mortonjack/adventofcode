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
    int common[26*2 + 1]; // last seen in common
    int sum = 0;

    while (!file.eof()) {
        if (counter % 3 == 0) {
            for (int i = 1; i < 26*2 + 1; i++) {
                common[i] = 0;
            }
        }
        
        // Look for duplicate
        for (int i = 0; i < length; i++) {
            if (common[value(line[i])] == (counter % 3)) {
                common[value(line[i])]++;
                if (common[value(line[i])] == 3) {
                    sum += value(line[i]);
                    i = length;
                }
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
