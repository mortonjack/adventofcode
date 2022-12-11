#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
using std::stoi;
using std::string; using std::ifstream;
using std::queue; using std::vector;
using std::cout; using std::endl;

/* Small, complicated input... faster to just enter everything in myself! */

class Monkey {
    public:
        queue<vector<int>> worry_level;
        int operate; // 0 = add, 1 = multiply, 2 = square
        int op_num, divisor, id;
        int throwee[2];

        // Constructor
        Monkey(int i) {
            id = i;
        }

        void operation(vector<int>& nums) {
            for (int i = 0; i < (int)nums.size(); i++) {
                if (operate == 0) nums[i] += op_num;
                else if (operate == 1) nums[i] *= op_num;
                else nums[i] *= nums[i];
            }
        }

        bool test(vector<int> nums) {
            return nums[id] % divisor == 0;
        }

        int throw_to(bool res) {
            return res ? throwee[0] : throwee[1];
        }
};

#define MAX_LENGTH 50
#define N_MONKEYS 8

int main() {
    
    int monkeycount = 0;
    Monkey* monk;
    Monkey* monkey[N_MONKEYS];

    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);

    while (!file.eof()) {
        // Read line
        if (line.at(0) == 'M') {
            monk = new Monkey(monkeycount);
            monkey[monkeycount++] = monk;
        }
        
        else if (line.at(2) == 'S') {
            /* Starting items */
            vector<int> nums;
            for (int i = 18; i < (int)line.length(); i+= 4) {
                nums.push_back(stoi( line.substr(i, i+1) ));
            }
            for (int i = 0; i < (int)nums.size(); i++) {
                vector<int> a;
                for (int j = 0; j < N_MONKEYS; j++) a.push_back(nums.at(i));
                monk->worry_level.push(a);
            }
        }

        else if (line.at(2) == 'O') {
            /* Define operation */
            if (line.at(23) == '+') monk->operate = 0;
            else if (line.at(23) == '*' && line.at(25) != 'o') monk->operate = 1;
            else monk->operate = 2;

            if (monk->operate != 2) {
                monk->op_num = stoi( line.substr(25, line.length()-1) );
            }
        }

        else if (line.at(2) == 'T') {
            /* Define test */
            monk->divisor = stoi( line.substr(21, line.length()-1) );
        }

        else if (line.at(4) == 'I') {
            /* Define true/false conditions */
            if (line.at(7) == 't') {
                monk->throwee[0] = stoi(line.substr(29, 29));
            } else {
                monk->throwee[1] = stoi(line.substr(30, 30));
                file.getline(thisLine, MAX_LENGTH);
                line.assign(thisLine);
            }
        }

        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }

    int inspect[N_MONKEYS] = {0};

    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < N_MONKEYS; j++) {
            inspect[j] += monkey[j]->worry_level.size();

            while (!monkey[j]->worry_level.empty()) {
                // Dequeue item
                vector<int> nums = monkey[j]->worry_level.front();
                monkey[j]->worry_level.pop();
                
                // Carry out operation
                monkey[j]->operation(nums);
                
                // Test
                bool res = monkey[j]->test(nums);

                // Manage worry levels
                for (int k = 0; k < N_MONKEYS; k++) {
                    nums[k] %= monkey[k]->divisor;
                }
                
                // Throw
                monkey[monkey[j]->throw_to(res)]->worry_level.push(nums);
            }
        }
    }

    int max = 0;
    long multiple = 1;
    for (int i = 0; i < 8; i++) {
        cout << "Monkey " << i << " inspected " << inspect[i] << " times\n";
        if (i != max && (long)inspect[max] * (long)inspect[i] > multiple) {
            multiple = (long)inspect[max] * (long)inspect[i];
            cout << "New max: " << inspect[max] << " * " << inspect[i] << " == " << multiple << endl;
            max = inspect[i] > inspect[max] ? i : max;
        }
        delete monkey[i];
    }
    cout << multiple << endl;

    return 0;
}
