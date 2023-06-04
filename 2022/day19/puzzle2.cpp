#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
using namespace std;

#define MAX_LENGTH 165 // max length of lines

/*
int dp_hits = 0;
int dp_attempts = 0;
int time_saved = 0;
*/

class State {
public:
    short int ore = 0;
    short int orebots = 1;
    short int clay = 0;
    short int claybots = 0;
    short int ob = 0;
    short int obbots = 0;
    short int geodes = 0;
    short int geodebots = 0;
    short int time = 32;

    void produce() {
        ore = min(ore+orebots, 31);
        clay = min(clay+claybots, 31);
        ob = min(ob+obbots, 31);
        geodes += geodebots;
        time--;
    }

    int64_t key() {
        int64_t k = time;
        k <<= 5;
        k += orebots;
        k <<= 5;
        k += claybots;
        k <<= 5;
        k += obbots;
        k <<= 5;
        k += geodebots;
        k <<= 5;
        k += ore;
        k <<= 5;
        k += clay;
        k <<= 5;
        k += ob;
        return k;
    }

    bool canBeat(int global_max) {
        if (global_max > geodes + (geodebots*time) + ((time-1)*(time)/2)) {
//            time_saved += time;
            return false;
        }
        return true;
    }
};

int max_geodes(array<int, 7>& cost, State state, int global_max, unordered_map<int64_t, int>& dp) {
    state.produce();
    if (state.time == 0) return 0;

    int64_t key = state.key();
//    dp_attempts++;
    if (dp.count(key)) {
//        dp_hits++;
        return dp[key];
    }
    
    // Backtrack early if making 1 geode robot/min won't reach the global max
    if (!state.canBeat(global_max)) return 0;

    int maximum = 0;
    bool unaffordable = false;

    // Make ore robot
    if (state.ore >= cost[0] && state.orebots < cost[6]) {
        State newstate = state;
        newstate.ore -= cost[0]+1;
        newstate.orebots++;
        maximum = max(maximum, max_geodes(cost, newstate, global_max, dp));
        global_max = max(maximum, global_max);
    } else unaffordable = true;

    // Make clay robot
    if (state.ore >= cost[1] && state.claybots < cost[3]) {
        State newstate = state;
        newstate.ore -= cost[1];
        newstate.clay--;
        newstate.claybots++;
        maximum = max(maximum, max_geodes(cost, newstate, global_max, dp));
        global_max = max(maximum, global_max);
    } else unaffordable = true;

    // Make obby robot
    if (state.ore >= cost[2] && state.clay >= cost[3] && state.obbots < cost[5]) {
        State newstate = state;
        newstate.ore -= cost[2];
        newstate.clay -= cost[3];
        newstate.ob--;
        newstate.obbots++;
        maximum = max(maximum, max_geodes(cost, newstate, global_max, dp));
        global_max = max(maximum, global_max);
    } else if (state.claybots) unaffordable = true;

    // Make geode robot
    if (state.ore >= cost[4] && state.ob >= cost[5]) {
        State newstate = state;
        newstate.ore -= cost[4];
        newstate.ob -= cost[5];
        newstate.geodes--;
        newstate.geodebots++;
        maximum = max(maximum, max_geodes(cost, newstate, global_max, dp));
        global_max = max(maximum, global_max);
    } else if (state.obbots) unaffordable = true;

    // Save up (ONLY if some robot couldn't be made)
    if (unaffordable) maximum = max(maximum, max_geodes(cost, state, global_max, dp));

    maximum += state.geodebots;
    dp[key] = maximum;
    return maximum;
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    vector<array<int, 7>> blueprints;
    int count = 0;
    // Each blueprint vector contains a cost vector of the form
    // {ore_cost, clay_cost, obsidian_ore_cost, obsidian_clay_cost, geode_ore_cost, geode_obsidian_cost};
    
    cout << "\n##### BLUEPRINT COSTS #####\n";

    // Read input
    while (!file.eof() && count < 3) {
        count++;
        array<int, 7> cost = {0};
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

        cost[6] = max(max(cost[0], cost[1]), max(cost[2], cost[4]));

        blueprints.push_back(cost);
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }
    
    cout << "\n##### QUALITY LEVELS #####\n";

    // Calculate quality levels
    // Cost: Ore, Ore, Ore/Clay, Ore/Obsidian, MAX_ORE
    vector<int> quality_levels(blueprints.size(), 0);
    int mult = 1;
    for (int i = 0; i < (int)quality_levels.size(); i++) {
        State state;
        unordered_map<int64_t, int> dp;
        quality_levels.at(i) = max_geodes(blueprints[i], state, 0, dp);
        int q = quality_levels.at(i);
        cout << "Geodes gathered from blueprint " << i+1 << " is " << q << endl;
        mult *= q;
    }
    cout << "The multiple of possible geode collection is " << mult << endl;
/*    cout << "DP ATTEMPTS: " << dp_attempts << endl;
    cout << "DP HITS: " << dp_hits << endl;
    cout << (float)dp_hits * 100.0 / (float)dp_attempts << "%\n";
    cout << "TIME SAVED: " << time_saved << endl;
    cout << (float)time_saved * 100.0 / (float)dp_attempts << "%\n";
*/
    file.close();
    return 0;
}
