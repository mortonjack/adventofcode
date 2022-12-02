#include <iostream>
#include <fstream>
#include <string>
using std::cout; using std::endl;
using std::ifstream; using std::string;

#define ROCK 'A'
#define PAPER 'B'
#define SCISSORS 'C'

#define LOSE 'X'
#define DRAW 'Y'
#define WIN 'Z'

int main() {
    ifstream file;
    file.open("input.txt");
    
    char thisLine[5];
    file.getline(thisLine, 4);
    int score = 0;
    int shape = 0;
    int outcome = 0;

    char result;
    char play;

    while (!file.eof()) {
        result = thisLine[2];
        play = thisLine[0];
        
        switch (result) {
            case LOSE:
                outcome = 0;
                play -= 1;
                break;
            case DRAW:
                outcome = 3;
                break;
            case WIN:
                outcome = 6;
                play += 1;
                break;
        }
        if (play > SCISSORS) play = ROCK;
        if (play < ROCK) play = SCISSORS;

        switch (play) {
            case ROCK:
                shape = 1;
                break;
            case PAPER:
                shape = 2;
                break;
            case SCISSORS:
                shape = 3;
                break;
        }

        score += outcome + shape;
        file.getline(thisLine, 4);
    }

    cout << "Total score: " << score << endl;

    file.close();
}
