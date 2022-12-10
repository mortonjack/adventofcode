#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 12 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int length = line.length();

    int cycle = 0; // cycle
    int X = 1; // signal strength
    vector<int> cycles;

    while (!file.eof()) {
        cout << "Cycle " << cycle;
        cout << " X = " << X << endl;
        if (line[0] == 'a') {
            cycle++;
            cycles.push_back(X*cycle);
            cycle++;
            cycles.push_back(X*cycle);
            X += stoi( line.substr(5, line.length()));
        } else {
            cycle++;
            cycles.push_back(X*cycle);
        }

        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }

    cout << cycles[19] + cycles[59] + cycles[99] + cycles[139] + cycles[179] + cycles[219] << endl;


    file.close();
    return 0;
}
