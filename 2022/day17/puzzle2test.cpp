#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
using std::vector; using std::to_string;
using std::cout; using std::endl;
using std::string; using std::ifstream;
using std::unordered_map;

#define MAX_LENGTH 10100 // max length of lines

string key(int i, int rock, int highest, vector<vector<char>>& cave) {
    string e = to_string(i);
    e.push_back('_');
    e.append( to_string(rock) );
    e.push_back('_');
    bool floor = false;
    for (int j = highest; j >= 0 && !floor; j--) {
        floor = true;
        for (int k = 0; k <(int) cave[j].size(); k++) {
            e.push_back(cave[j][k]);
            if (cave[j][k] == '.') floor = false;
        }
    }
    return e;
}

int main() {
    ifstream file;
    file.open("input.txt");

    vector<vector<string>> forms;
    forms.push_back({"####"});
    forms.push_back({".#.", "###", ".#."});
    forms.push_back({"###", "..#", "..#"});
    forms.push_back({"#","#","#","#"});
    forms.push_back({"##","##"});

    char thisLine[MAX_LENGTH];
    string jet;
    file.getline(thisLine, MAX_LENGTH);
    jet.assign(thisLine);
    int length = jet.length();
    int i = 0; /* index of jet stream direction */
    int rock = -1;
    long long rest = 0;
    int highest = -1;
    int x =  2, y = 3;
    bool falling = false;
    int resets = 0;
    /* Initialise cave to 2022*4 +1 tall, empty space */
    vector<vector<char>> cave(100100, vector<char>(7, '.'));
    unordered_map<string, vector<long long>> cycle;
    int last_rest = 0;
    bool found_cycle = false;

    while (rest < 1000000000000) {
        if (rest % 1000000 == 0 && rest != last_rest) {
            cout << rest/1000000 << " million complete, " << 1000000-(rest/1000000) << " to go." << endl;
            last_rest = rest;
        }
        

        if (!falling) {
            // Spawn in new rock
            rock++;
            x = 2;
            y = highest+4;

            falling = true;

            rock %= 5;
        }

        // Attempt to move rock left/right
        if (jet[i] == '<') {
            // Left
            bool valid = x > 0;
            if (x > 0) {
                for (int j = 0; j < (int)forms[rock].size(); j++) {
                    for (int k = 0; k < (int)forms[rock][j].length(); k++) {
                        if (forms[rock][j][k] == '#' && cave[y+j][x-1+k] == '#') {
                            valid = false;
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
                    if (forms[rock][j][k] == '#') cave[y+j][x+k] = '#';
                }
            }
            /* Dynamic programming approach. 
             * If there's a cycle anywhere here, it'll be extremely fast.
             * Input key: characters from highest down to the first 'floor', i.e. 7 filled tiles in a row 
             * or y = 0. Also, prefixed with i_rock_. */ 
            /* i, rock, highest, cave */
            bool open = true;
            for (int j = y; j < (int)forms[rock].size() && !found_cycle; j++) {
                open = false;
                // Look for a floor at any point
                for (int k = 0; k < 7; k++) {
                    if (cave[j][k] != '.') open = true;
                }
                if (!open) {
                    // Look for cycle or store cycle.
                    string k = key(i, rock, highest, cave);
                    cout << "Floor found at " << j+(100000*resets) << endl;
                    if (cycle.count(k)) {
                        // Cycle repeats
                        cout << "Repeated cycle from " << cycle[k][0] << endl;
                        cout << "Cycle length " << rest - cycle[k][1] << endl;
                        long long per_cycle = (highest + (100000*resets)) - cycle[k][0];
                        cout << "Height per cycle " << per_cycle << endl;
                        long long in_cycle = 1000000000000 - cycle[k][1];
                        long long remainder = in_cycle % (rest-cycle[k][1]);
                        in_cycle -= remainder;
                        cout << "In cycle " << in_cycle << " rocks, making " << in_cycle*per_cycle << " height" << endl;
                        cout << "Cycles remaining " << remainder << endl;
                        rest = 1000000000000-remainder;
                        found_cycle = true;
                    } else {
                        // {highest + 100k thing, rest}
                        cycle[k] = {highest + 100000*resets, rest};
                    }
                    break;
                }
            }
        } else {
            y--;
        }

        // RESET
        if (highest > 100000) {
            resets++;
            vector<vector<char>> save(100, vector<char>(7, '.'));
            for (int j = 0; j < 100; j++) {
                for (int k = 0; k < 7; k++) {
                    save[j][k] = cave[100000+j][k];
                }
            }
            cave = vector<vector<char>>(100100, vector<char>(7, '.'));
            for (int j = 0; j < 100; j++) {
                for (int k = 0; k < 7; k++) {
                    cave[j][k] = save[j][k];
                }
            }
            highest -= 100000;
        }

    }

    file.close();
    return 0;
}
