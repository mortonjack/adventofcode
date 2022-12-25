#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
using std::vector; using std::unordered_map;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 30 /* max length of lines */

void add_snafu_nums(vector<int>& total, string s, unordered_map<char, long long> snafu) {
    /* First: Appropriately add things */
    for (int i = s.length()-1; i >= 0; i--) {
        total.at(s.length()-1-i) += snafu.at(s.at(i));
    }
    /* Now: Carry everything over */
    for (int i = 0; i < (int)total.size(); i++) {
        while (total.at(i) < -2) {
            total.at(i) += 5;
            if (i+1 >= (int)total.size()) total.push_back(0);
            total.at(i+1)--;
        }
        while (total.at(i) > 2) {
            total.at(i) -= 5;
            if (i+1 >= (int)total.size()) total.push_back(1);
            else total.at(i+1)++;
        }
    }
}

string vec_to_snafu(vector<int>& total, unordered_map<int, char> decimal) {
    bool first = false;
    string res;
    for (int i = (int)total.size()-1; i >= 0; i--) {
        if (!first) {
            if (total.at(i) != 0) first = true;
        }
        if (first) {
            res.push_back(decimal.at(total.at(i)));
        }
    }
    return res;
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    unordered_map<char, long long> snafu;
    snafu['='] = -2;
    snafu['-'] = -1;
    snafu['0'] = 0;
    snafu['1'] = 1;
    snafu['2'] = 2;

    unordered_map<int, char> decimal;
    decimal[-2] = '=';
    decimal[-1] = '-';
    decimal[0] = '0';
    decimal[1] = '1';
    decimal[2] = '2';

    vector<int> total(100, 0);

    while (!file.eof()) {
        add_snafu_nums(total, line, snafu);
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }
    cout << "The final number is " << vec_to_snafu(total, decimal) << endl;
    cout << "Merry christmas!" << endl;

    file.close();
    return 0;
}
