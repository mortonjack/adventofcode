#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 10100 // max length of lines

int main() {
    ifstream file;
    file.open("input.txt");

    vector<vector<string>> forms;
    forms.push_back({"####"});
    forms.push_back({".#.", "###", ".#."});
    forms.push_back({"###", "..#", "..#"});
    forms.push_back({"#","#","#","#"});
    forms.push_back({"##","##"});
    for (int i = 0; i < (int)forms.size(); i++) {
        for (int j = forms[i].size()-1; j >= 0; j--) {
            cout << forms[i][j];
            cout << endl;
        }
        cout << endl;
    }

    char thisLine[MAX_LENGTH];
    string jet;
    file.getline(thisLine, MAX_LENGTH);
    jet.assign(thisLine);
    int length = jet.length();
    int i = 0; /* index of jet stream direction */
    int rock = -1;
    int rest = 0;
    int highest = -1;
    int x = 2, y = 3;
    bool falling = false;
    /* Initialise cave to 2022*4 +1 tall, empty space */
    vector<vector<char>> cave(2022*4 +1, vector<char>(7, '.'));

    while (rest < 2022) {
        if (!falling) {
            // Spawn in new rock
            rock++;
            x = 2;
            y = highest+4;

            falling = true;

            rock %= 5;
        }

        // Attempt to move rock left/right
//        cout << i << ": " << x << ", " << y << endl;
        if (jet[i] == '<') {
            // Left
            bool valid = x > 0;
            if (x > 0) {
                for (int j = 0; j < (int)forms[rock].size(); j++) {
                    for (int k = 0; k < (int)forms[rock][j].length(); k++) {
                        if (forms[rock][j][k] == '#' && cave[y+j][x-1+k] == '#') {
                            valid = false;
//                            cout << "error: " << y+j << " and " << x-1+k << " is #\n";
                        }
                    }
                }
            }
            if (valid) x--;
        } else {
            // Right
            bool valid = x+forms[rock][0].size() < 7;
            if (valid) {
                for (int j = 0; j < (int)forms[rock].size(); j++) {
                    for (int k = 0; k < (int)forms[rock][j].length(); k++) {
                        if (forms[rock][j][k] == '#' && cave[y+j][x+1+k] == '#') {
                            valid = false;
//                            cout << "error: " << y+j << " and " << x+1+k << " is #\n";
                        }
                    }
                }
            }
            if (valid) x++;
        }
        i++;
        i %= length;

        // Can rock be moved down?
        if (y == 0) falling = false;
        else {
            for (int j = 0; j < (int)forms[rock].size(); j++) {
                for (int k = 0; k < (int)forms[rock][j].length(); k++) {
                    if (forms[rock][j][k] == '#' && cave[y-1+j][x+k] == '#') falling = false;
                }
            }
        }

        // Come to rest
        if (!falling) {
            rest++;
            highest = highest > y+(int)forms[rock].size() -1 ? highest : y+forms[rock].size()-1;
            // Draw rock
            for (int j = 0; j < (int)forms[rock].size(); j++) {
                for (int k = 0; k < (int)forms[rock][j].length(); k++) {
                    // Draw at y+j, x+k
//                    cout << '[' << y+j << "][" << x+k << "] = [" << rock << "][" << j << "][" << k << "]\n";
                    if (forms[rock][j][k] == '#') cave[y+j][x+k] = '#';
                }
            }
        } else {
            y--;
        }
    }

    cout << "Max height is " << highest+1 << endl;
    for (int i = 20; i >= 0; i--) {
        for (int j = 0; j < 7; j++) {
            cout << cave[i][j];
        }
        cout << " " << i << endl;
    }

    file.close();
    return 0;
}
