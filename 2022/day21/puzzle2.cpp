#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
using std::vector; using std::unordered_map;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 50 // max length of lines

class Monkey {
public:
    long long number;
    char op;
    string first;
    string second;
    string key;
    bool human_in_chain;
    // Order: first op second
    // e.g. first = sqbt, op = *, second = rvzp
    // sqbt * rvzp
    
    Monkey(char operation, string a, string b) {
        first = a;
        second = b;
        op = operation;
        human_in_chain = false;
    }

    Monkey(long long num) {
        number = num;
        op = '\0';
        human_in_chain = false;
    }

    bool find_human(unordered_map<string, Monkey*>& m) {
        if (key == "humn") return human_in_chain = true;
        if (op == '\0') return human_in_chain;
        human_in_chain = m[first]->find_human(m) || m[second]->find_human(m);
        return human_in_chain;
    }

    long long get_human_num(unordered_map<string, Monkey*>& map) {
        if (map[first]->human_in_chain) {
            return map[first]->human_num(map, true, '+', map[second]->yell(map), 0);
        } else return map[second]->human_num(map, false, '+', map[first]->yell(map), 0);
    }

    long long human_num(unordered_map<string, Monkey*>& m, bool f, char operation, long long target, long long other) {
        if (operation == '+') target -= other;
        else if (operation == '-') target = f ? target+other : other-target;
        else if (operation == '*') target /= other;
        else if (operation == '/') target = f ? target*other : other/target;

        if (key == "humn") {
            number = target;
            op = '\0';
            return target;
        }
        if (m[first]->human_in_chain) return m[first]->human_num(m, true, op, target, m[second]->yell(m));
        else return m[second]->human_num(m, false, op, target, m[first]->yell(m));
    }
    
    long long yell(unordered_map<string, Monkey*>& map) {
        if (op == '\0') return number;
        long long a = map[first]->yell(map);
        long long b = map[second]->yell(map);
        switch (op) {
            case '+':
                return a+b;
            case '-':
                return a-b;
            case '*':
                return a*b;
            case '/':
                return a/b;
        }
        return -1;
    }

};

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    unordered_map<string, Monkey*> map;
    vector<string> keys;

    while (!file.eof()) {
        string key = line.substr(0, 4);

        if (line[6] >= '0' && line[6] <= '9') {
            map[key] = new Monkey( stoi(line.substr(6, 10)));
        } else {
            map[key] = new Monkey(line[11], line.substr(6, 4), line.substr(13, 4));
        }

        keys.push_back(key);
        map[key]->key = key;
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    string root = "root";
    map[root]->find_human(map);
    cout << "Puzzle 1: " << map[root]->yell(map) << endl;
    cout << "Puzzle 2: " << map[root]->get_human_num(map) << endl;
    cout << map[map[root]->first]->yell(map) << " == " << map[map[root]->second]->yell(map) << endl;
    
    for (int i = 0; i < (int)keys.size(); i++) {
        delete map[keys[i]];
    }

    file.close();
    return 0;
}
