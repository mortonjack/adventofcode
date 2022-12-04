#include <iostream>
#include <string>
#include <fstream>
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
    int length = line.length();

    while (!file.eof()) {
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }

    file.close();
    return 0;
}
