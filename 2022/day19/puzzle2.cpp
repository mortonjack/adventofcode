#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using std::vector; using std::max;
using std::cout; using std::endl; using std::unordered_map;
using std::string; using std::ifstream; using std::to_string;

#define MAX_LENGTH 165 // max length of lines

string get_key(int ore, int ore_robot, int clay, int clay_robot, int ob, int ob_robot, int geode_robot, int time) {
    string k = "";
    k.append(to_string(ore));
    k.push_back('_');
    k.append(to_string(ore_robot));
    k.push_back('_');
    k.append(to_string(clay));
    k.push_back('_');
    k.append(to_string(clay_robot));
    k.push_back('_');
    k.append(to_string(ob));
    k.push_back('_');
    k.append(to_string(ob_robot));
    k.push_back('_');
    k.append(to_string(geode_robot));
    k.push_back('_');
    k.append(to_string(time));
    return k;
}

int max_geodes(unordered_map<string, int>& prev, vector<int>& cost, int ore, int ore_robot, int clay, int clay_robot, int ob, int ob_robot, int geodes, int geode_robot, int time, int global_max) {
    string key = get_key(ore, ore_robot, clay, clay_robot, ob, ob_robot, geode_robot, time);
    if (prev.count(key)) return prev.at(key);
//    cout << ore_robot << " " << clay_robot << " " << ob_robot << " " << geode_robot << " " << time << endl;
    if (time == 0) return 0;

    // Backtrack early if making 1 geode robot/min won't reach the global max
    // Say time is 5 and we have 2 robots. We'd get +2 now, +3 on 4, +4 on 3, +5 on 2, and +6 on 1.
    // This makes r+(r+1)+(r+2)+(r+3)+(r+4) which is 5r + (4+3+2+1) which is time*r + (time-1)(time)/2 right?
    if (global_max > geodes + (geode_robot*time) + ((time-1)*(time)/2)) return 0;

    ore += ore_robot;
    clay += clay_robot;
    ob += ob_robot;
    geodes += geode_robot;
    
    int maximum = geodes;
    bool unaffordable = false;

    // Decision tree - make ore robot
    if (ore >= cost[0]) {
        maximum = max(maximum, max_geodes(prev, cost, ore-cost[0]-1, ore_robot+1, clay, clay_robot, ob, ob_robot, geodes, geode_robot, time-1, global_max));
    } else {
        unaffordable = true;
    }

    global_max = max(maximum, global_max);

    // Decision tree - make clay robot
    if (ore >= cost[1]) {
        maximum = max(maximum, max_geodes(prev, cost, ore-cost[1], ore_robot, clay-1, clay_robot+1, ob, ob_robot, geodes, geode_robot, time-1, global_max));
    } else {
        unaffordable = true;
    }

    global_max = max(maximum, global_max);

    // Decision tree - make obby robot
    if (ore >= cost[2] && clay >= cost[3]) {
        maximum = max(maximum, max_geodes(prev, cost, ore-cost[2], ore_robot, clay-cost[3], clay_robot, ob-1, ob_robot+1, geodes, geode_robot, time-1, global_max));
    } else {
        unaffordable = true;
    }

    global_max = max(maximum, global_max);

    // Decision tree - make geode robot
    if (ore >= cost[4] && ob >= cost[5]) {
        maximum = max(maximum, max_geodes(prev, cost, ore-cost[4], ore_robot, clay, clay_robot, ob-cost[5], ob_robot, geodes-1, geode_robot+1, time-1, global_max));
    } else {
        unaffordable = true;
    }

    global_max = max(maximum, global_max);

    // Decision tree - save up
    if (unaffordable) maximum = max(maximum, max_geodes(prev, cost, ore, ore_robot, clay, clay_robot, ob, ob_robot, geodes, geode_robot, time-1, global_max));

    global_max = max(maximum, global_max);

    prev[key] = maximum;
    return prev.at(key);
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    vector<vector<int>> blueprints = {{-1}};
    int count = 0;
    // Each blueprint vector contains a cost vector of the form
    // {ore_cost, clay_cost, obsidian_ore_cost, obsidian_clay_cost, geode_ore_cost, geode_obsidian_cost};

    // Read input
    while (!file.eof() && count < 3) {
        count++;
        vector<int> cost(6, -1);
        int c = 0; /* cost index */
        for (int i = 25; c != 6; i++) {
            while (line.at(i) != 'c' || line.at(i+1) != 'o') i++;
            cost.at(c) = stoi(line.substr(i+5, 3));
            c++;
            if (c == 3 || c == 5) {
                while (line.at(i) != 'n') i++;
                cost.at(c) = stoi(line.substr(i+2, 3));
                c++;
            }
        }
        cout << "Ore: " << cost[0];
        cout << ", Clay: " << cost[1];
        cout << ", Obsidian: " << cost[2] << ", " << cost[3];
        cout << ", Geode: " << cost[4] << ", " << cost[5] << endl;

        blueprints.push_back(cost);
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    // Calculate quality levels
    // Cost: Ore, Ore, Ore/Clay, Ore/Obsidian
    vector<int> quality_levels(blueprints.size(), 0);
    int mult = 1;
    for (int i = 1; i < (int)quality_levels.size(); i++) {
        unordered_map<string, int> time;
        quality_levels.at(i) = max_geodes(time, blueprints[i], 0, 1, 0, 0, 0, 0, 0, 0, 32, 0);
        int q = quality_levels.at(i);
        cout << "Geodes gathered from blueprint " << i << " is " << q << endl;
        mult *= q;
    }
    cout << "The multiple of possible geode collection is " << mult << endl;

    file.close();
    return 0;
}
