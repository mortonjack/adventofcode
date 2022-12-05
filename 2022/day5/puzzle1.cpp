#include <iostream>
#include <string>
#include <fstream>
#include <stack>
using std::cout; using std::endl;
using std::string; using std::ifstream;
using std::stack;

#define MAX_LENGTH 40 // max length of lines

void read_nums(int nums[3], string line) {
    nums[0] = line[5] - '0';
    int i = 12;
    if (line[6] != ' ') {
        i++;
        nums[0] *= 10;
        nums[0] += line[6] - '0';
    }
    nums[1] = line[i] - '1';
    i += 5;
    nums[2] = line[i] - '1';
    return;
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    stack<char> crates_temp[9]; // stacks of crates
    stack<char> crates[9];

    // Attain initial information
    while (line[1] != '1') {
        
        int j = 0;
        for (int i = 1; i < 35; i += 4) {
            if (line[i] != ' ') crates_temp[j].push(line[i]);
            j++;
        }

        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    // Reverse order of crates
    for (int i = 0; i < 9; i++) {
        while (!crates_temp[i].empty() && crates_temp[i].top() != ' ') {
            crates[i].push(crates_temp[i].top());
            crates_temp[i].pop();
        }
    }
    
    // Input empty line    
    file.getline(thisLine, MAX_LENGTH);
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    // Move crates around
    while (!file.eof()) {

        // Read nums
        int nums[3];
        read_nums(nums, line);
        
        // Move stacks
        for (int i = 0; i < nums[0]; i++) {
            crates[nums[2]].push(crates[nums[1]].top());
            crates[nums[1]].pop();
        }

        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    for (int i = 0; i < 9; i++) {
        cout << crates[i].top() << " ";
    }
    cout << endl;
    for (int i = 0; i < 9; i++) {
        cout << i << ": ";
        while (!crates[i].empty()) {
            cout << crates[i].top() << " ";
            crates[i].pop();
        }
        cout << endl;
    }

    file.close();
    return 0;
}
