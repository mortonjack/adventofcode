#include "input.cpp"

int main() {
    // Read file name
    cout << "Reading lines...\n";
    auto raw_lines = read_lines("input.txt");
    auto lines = split_lines(raw_lines);
    cout << "Outputting strings...\n";
    for (auto& line: lines) {
        for (auto& split: line) {
            cout << split << " $ ";
        }
        cout << '\n';
    }

    auto nums = extract_nums(raw_lines);
    for (auto& line: nums) {
        for (int n: line) {
            cout << n << ' ';
        }
        cout << '\n';
    }

    return 0;
}
