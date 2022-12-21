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
    // Order: first op second
    // e.g. first = sqbt, op = *, second = rvzp
    // sqbt * rvzp
    
    Monkey(char operation, string a, string b) {
        first = a;
        second = b;
        op = operation;
    }

    Monkey(long long num) {
        number = num;
        op = '\0';
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
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    string root = "root";
    cout << map[root]->yell(map) << endl;
    
    for (int i = 0; i < (int)keys.size(); i++) {
        delete map[keys[i]];
    }

    file.close();
    return 0;
}
