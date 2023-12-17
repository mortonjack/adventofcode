#include <vector>
#include <unordered_set>
#include <iostream>
#include <string>
#include <queue>
using namespace std;
#define MAP vector<vector<int>>
#define PQ priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>>

MAP read_input() {
  MAP map;
  while (cin) {
    string line; 
    getline(cin, line);
    if (line == "") continue;
    vector<int> nums;
    for (char c : line) {
      nums.push_back(c-'0');
    }
    map.push_back(nums);
  }
  return map;
}

void push_if_valid(int r, int c, int moves, int dir, int heat_lost, PQ& pq, MAP& map) {
  if (r < 0 || r >= map.size() || c < 0 || c >= map.front().size()) return;
  pq.push({heat_lost + map[r][c], r, c, moves+1, dir});
}

int gen_key(int r, int c, int moves, int dir) {
  return (((((r << 8) + c) << 8) + moves) << 3) + dir;
}

int main() {
  MAP map = read_input();
  // Idea: {heat_lost, row, col, moves, dir}
  // UP 0 DOWN 1 LEFT 2 RIGHT 3
  PQ pq;
  unordered_set<int> seen;
  pq.push({0, 0, 0, 0, 1});
  pq.push({0, 0, 0, 0, 3});
  while (!pq.empty()) {
    auto pos = pq.top(); pq.pop();
    int heat_lost = pos[0], r = pos[1], c = pos[2], moves = pos[3], dir = pos[4];
    
    if (r == map.size()-1 && c == map.front().size()-1) {
      cout << heat_lost << endl;
      return 0;
    }

    int key = gen_key(r, c, moves, dir);
    if (seen.count(key)) continue;
    seen.insert(key); 

    // Straight
    if (moves < 3) {
      int new_r = r + (dir < 2 ? (dir == 0 ? -1 : 1) : 0);
      int new_c = c + (dir > 1 ? (dir == 2 ? -1 : 1) : 0);
      push_if_valid(new_r, new_c, moves, dir, heat_lost, pq, map);
    }

    // Left/Right
    if (dir < 2) {
      push_if_valid(r, c-1, 0, 2, heat_lost, pq, map);
      push_if_valid(r, c+1, 0, 3, heat_lost, pq, map);
    }

    // Up/Down
    if (dir > 1) {
      push_if_valid(r-1, c, 0, 0, heat_lost, pq, map);
      push_if_valid(r+1, c, 0, 1, heat_lost, pq, map);
    }
  }
  return 0;
}