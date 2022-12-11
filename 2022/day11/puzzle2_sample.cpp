#include <iostream>
#include <queue>
#include <vector>
using std::queue; using std::vector;
using std::cout; using std::endl;

/* Small, complicated input... faster to just enter everything in myself! */

class Monkey {
    public:
        queue<vector<int>> worry_level;
        int id;

        // Constructor
        Monkey(int i) {
            id = i;
            vector<int> items;

            if (i == 0) {
                items = {79, 98};
            } else if (i == 1) {
                items = {54, 65, 75, 74};
            } else if (i == 2) {
                items = {79, 60, 97};
            } else {
                items = {74};
            }

            for (int i = 0; i < (int)items.size(); i++) {
                vector<int> a;
                for (int j = 0; j < 8; j++) a.push_back(items.at(i));
                worry_level.push(a);
            }
        }

        void operation(vector<int>& nums) {
            switch (id) {
                case 0:
                    for (int i = 0; i < 4; i++) nums[i] *= 19;
                    break;
                case 1:
                    for (int i = 0; i < 4; i++) nums[i] += 6;
                    break;
                case 2:
                    for (int i = 0; i < 4; i++) nums[i] *= nums[i];
                    break;
                case 3:
                    for (int i = 0; i < 4; i++) nums[i] += 3;
                    break;
            }
            return;
        }

        bool test(vector<int> nums) {
            long n = nums[id];
            switch (id) {
                case 0:
                    return n % 23 == 0;
                    break;
                case 1:
                    return n % 19 == 0;
                    break;
                case 2:
                    return n % 13 == 0;
                    break;
                case 3:
                    return n % 17 == 0;
                    break;
            }
            return true;
        }

        int throw_to(bool res) {
            switch (id) {
                case 0:
                    return res ? 2 : 3;
                    break;
                case 1:
                    return res ? 2 : 0;
                    break;
                case 2:
                    return res ? 1 : 3;
                    break;
                case 3:
                    return res ? 0 : 1;
                    break;
            }
            return 0;
        }
};

int main() {
    Monkey* monkey[4]; // array of 8 monkeys
    for (int i = 0; i < 4; i++) {
        monkey[i] = new Monkey(i);
    }

    int inspect[4] = {0, 0, 0, 0};

    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < 4; j++) {
            while (!monkey[j]->worry_level.empty()) {
                // Dequeue item
                vector<int> nums = monkey[j]->worry_level.front();
                monkey[j]->worry_level.pop();

                // Increase inspection count
                inspect[j]++;
                
                // Carry out operation
                monkey[j]->operation(nums);

                // Test
                bool res = monkey[j]->test(nums);

                // REDUCE WORRY LEVEL - TAKE MODULO OF LCM
                nums[0] %= 23;
                nums[1] %= 19;
                nums[2] %= 13;
                nums[3] %= 17;

                // Throw
                monkey[monkey[j]->throw_to(res)]->worry_level.push(nums);
            }
        }
    }

    int max = 0;
    long multiple = 1;
    for (int i = 0; i < 4; i++) {
        cout << "Monkey " << i << " inspected items " << inspect[i] << " times.\n";
        if (i != max && (long)inspect[max] * (long)inspect[i] > multiple) {
            multiple = (long)inspect[max] * (long)inspect[i];
            max = inspect[i] > inspect[max] ? i : max;
        }
        delete monkey[i];
    }
    cout << multiple << endl;

    return 0;
}
