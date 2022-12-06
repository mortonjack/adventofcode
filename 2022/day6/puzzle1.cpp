#include <iostream>
#include <string>
#include <fstream>
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 4097 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");

    char line[MAX_LENGTH];
    int conflict[MAX_LENGTH];
    file.getline(line, MAX_LENGTH);
    int ind[4] = {0, 1, 2, 3};
    int i = 4;

    while (line[i] != '\0') {
        conflict[i] = 0;
        bool c = false;
        for (int j = 0; j < 4; j++) {
            if (conflict[ind[j]] > i-4) c = true;
            if (line[ind[j]] == line[i]) {
                if (conflict[i] < ind[j]) conflict[i] = ind[j];
                c = true;
            }
        }
        if (!c) break;

        ind[i % 4] = i;
        i++;
    }
    
    cout << "The " << i+1 << "th character starts the signal." << endl;

    file.close();
    return 0;
}
