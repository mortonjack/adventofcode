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
    unordered_map<string, long long> height_since_floor;
    int last_rest = 0;

    while (rest < 1000000000000) {
        if (rest % 1000000 == 0 && rest != last_rest) cout << rest/1000000 << " million complete, " << 1000000-(rest/1000000) << " to go." << endl;
        

        if (!falling) {
            // Spawn in new rock
            rock++;
            x = 2;
            y = highest+4;

            falling = true;

            rock %= 5;
            if (rock == 0 && i == 0) cout << "IT DOES RESET." << endl;
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
            /* Dynamic programming approach. Getting the key will slow me down quite a bit. 
             * but if there's a cycle anywhere here, then it'll be extremely fast.
             * Input key: characters from highest down to the first 'floor', i.e. 7 filled tiles in a row 
             * or y = 0. Also, prefixed with i_rock_. */ 
            /* i, rock, highest, cave */
            string my_key = key(i, rock, highest, cave);
            if (height_since_floor.count(my_key)) {
                cout << "Cycle begin at " << rest << " rocks, " << i << "i\n";
                cout << "Difference: " << highest + (resets* 100000) - height_since_floor[my_key] << endl;
                cout << "Updating...\n";
                height_since_floor[my_key] = resets * 100000;
                break;
            } else {
                height_since_floor[my_key] = highest + (resets * 100000);
            }
        } else {
            y--;
        }
        if (i == 1058) {
            cout << "Height at 1058 " << highest << endl;
        }
        if (i == 2767+833) {
            cout << "Height 833 after 2767 " << highest << endl;
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
