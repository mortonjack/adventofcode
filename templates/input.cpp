/* Functions to help read input */
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using std::cout;
using std::string;
using std::ifstream;
using std::vector;

// Read lines as strings
vector<vector<string>> split_lines(vector<string> lines, char split = ' ') {
    vector<vector<string>> res = vector(lines.size(), vector<string>());

    for (int i = 0; i < (int)lines.size(); i++) {
        // Read current line
        int start = 0, end = 1;
        while (start < (int)lines[i].length()) {
            // Find next substring to split
            while ((end < (int)lines[i].length() && lines[i][end] != split) || end < start) end++;
            // Append substring to vector
            res[i].push_back(lines[i].substr(start, end-start));
            // Find beginning of next substring
            start = end;
            while (start < (int)lines[i].length() && lines[i][start] == split) start++;
        }
    }

    return res;
}

vector<string> read_lines(const char* fname) {
    ifstream file;
    file.open(fname);
    vector<string> res;
    string line;
    getline(file, line);
    while (!file.eof()) {
        res.push_back(line);
        getline(file, line);
    }
    return res;
}

// Extract numbers from line
vector<int> extract_nums_in_line(string& line) {
    vector<int> res;
    int i = 0;
    while (i < (int)line.length()) {
        // Found number, append to vector
        if (line[i] >= '0' && line[i] <= '9') res.push_back(stoi(line.substr(i, 10)));
        else if (i+1 < (int)line.length() && line[i] == '-' && line[i+1] >= '0' && line[i+1] <= '9') {
            res.push_back(stoi(line.substr(i++, 11)));
        }
        // Increment i
        while (i < (int)line.length() && line[i] >= '0' && line[i] <= '9') i++;
        i++;
    }
    return res;
}

vector<vector<int>> extract_nums(vector<string>& lines) {
    vector<vector<int>> res;
    for (int i = 0; i < (int)lines.size(); i++) {
        res.push_back(extract_nums_in_line(lines[i]));
    }
    return res;
}
