#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 50 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    while (!file.eof()) {
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    file.close();
    return 0;
}
