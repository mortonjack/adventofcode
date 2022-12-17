#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 10100 // max length of lines
#define SAVE_COUNT 1000

int main() {
    ifstream file;
    file.open("input.txt");

    vector<vector<string>> forms;
    forms.push_back({"####"});
    forms.push_back({".#.", "###", ".#."});
    forms.push_back({"###", "..#", "..#"});
    forms.push_back({"#","#","#","#"});
    forms.push_back({"##","##"});
    /*for (int i = 0; i < (int)forms.size(); i++) {
        for (int j = forms[i].size()-1; j >= 0; j--) {
            cout << forms[i][j];
            cout << endl;
        }
        cout << endl;
    }*/

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
    long long height_per_cycle = 0;
    bool falling = false;
    int resets = 0;
    int last_time = 0;
    long long last_time_rocks = 0;
    int counted = 0;
    /* Initialise cave to 2022*4 +1 tall, empty space */
//    long long height_after_remainder = 0;
    vector<vector<char>> cave(100000 + SAVE_COUNT, vector<char>(7, '.'));
    //int diffcount = 0;
    //int last_thing = 0;

    while (rest < 1000000000000) {
        if (!falling) {
            // Spawn in new rock
            rock++;
            x = 2;
            y = highest+4;

            falling = true;

            rock %= 5;
        }
        if (i == 2767) {
            /*if (rest % 1745 != last_thing) {
                last_thing = rest % 1745;
                cout << last_thing << ", continued for " << diffcount << " cycles\n";
                diffcount = 0;
            } else diffcount++;*/
            counted++;
            cout << "ROCK " << rock << endl;
            cout << "Current highest: " << highest << " with " << resets << " resets.\n";
            height_per_cycle = (highest +resets*100000) -last_time;
/*            if (height_per_cycle != 2767) {
                cout << "DIFFERENT HEIGHT PER CYCLE!\n";
                cout << "Current highest: " << highest << " with " << resets << " resets.\n";
                cout << "Difference since last time: " << height_per_cycle << endl;
                cout << "Rest % 1745 = " << rest % 1745 << endl;
                cout << rest - last_time_rocks << " new rocks and " << 1000000000000 - rest << " to go\n";
            }*/
            cout << "Difference since last time: " << height_per_cycle << endl;
            cout << "With " << rest - last_time_rocks << " new rocks and " << 1000000000000-rest << " to go\n";
            cout << "Expected height: " << (long long)(highest + 100000*resets + ((double)(1000000000000-rest) * (((double)highest + (double)100000*(double)resets) / (double)rest))) << endl;
            last_time = highest + (resets*100000);
            last_time_rocks = rest;
//            if (counted > 1000) break;
        }
//        if (counted > 2 && i == 2767+573) {
//            height_after_remainder = highest-last_time;
//            cout << "Height after remainder " << height_after_remainder << endl;
//        }

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

        // RESET
        if (highest > 100000) {
            resets++;
            vector<vector<char>> save(SAVE_COUNT, vector<char>(7, '.'));
            for (int j = 0; j < SAVE_COUNT; j++) {
                for (int k = 0; k < 7; k++) {
                    save[j][k] = cave[100000+j][k];
                }
            }
            cave = vector<vector<char>>(100000+SAVE_COUNT, vector<char>(7, '.'));
            for (int j = 0; j < SAVE_COUNT; j++) {
                for (int k = 0; k < 7; k++) {
                    cave[j][k] = save[j][k];
                }
            }
            highest -= 100000;
        }

    }
    cout << "The highest is " << highest + (resets * (100000)) << endl;
    // 13821 + 2767 * ((999999991281 - 1016)/1745)  + 1016 * 1601
            /*cout << "ROCK " << rock << endl;
            cout << "Current highest: " << highest << " with " << resets << " resets.\n";
            cout << "Difference since last time: " << highest-last_time << endl;
            long long new_rocks = rest + resets*100000;
            cout << "With " << new_rocks - last_time_rocks << " new rocks and " << 1000000000000-new_rocks << " to go\n";
            last_time = highest;
            last_time_rocks = new_rocks;*/
    /* Calculate the remaining rocks */
/*    long long remaining = 1000000000000-rest;
    long long rocks_per_cycle = 1745;
    long long remainder = remaining % rocks_per_cycle;
    long long full_cycles_remaining = (remaining-remainder)/rocks_per_cycle;
    // Total is current max height + remainder height + height_per_cycle * full_cycles_remaining
    cout << endl << rest << " rocks at rest with " << remaining << " remaining." << endl;
    cout << "With " << rocks_per_cycle << " rocks per cycle, there are " << full_cycles_remaining << " cycles remaining." << endl;
    cout << remainder << " rocks will remain after all cycles complete." << endl;
    cout << height_per_cycle << " new height per cycle." << endl;
    cout << endl;
    long long total_height = highest + resets*100000;
    cout << total_height << " current height." << endl;
    cout << height_after_remainder << " height will be added." << endl;
    total_height += height_after_remainder;
    cout << "Aside from the cycles, there is " << total_height << " height." << endl;
    cout << height_per_cycle << " * " << full_cycles_remaining << " = " << height_per_cycle*full_cycles_remaining << endl;
    total_height += height_per_cycle * full_cycles_remaining;
    cout << "There will be " << total_height << " total height after all cycles go through." << endl;
*/
    file.close();
    return 0;
}
