// DevonRogersCPPAssignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h> //used to update the console after a period of time
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <conio.h> //this is used for the movement of the player's paddle, as well as other things
#include <stdlib.h> // this is used to clear the screen
#include <Windows.h> //this is used to make the program sleep temporarily
#include <utility>

using namespace std;

unsigned int currentScore = 0;

//a shortened version of system("cls") because i'm gonna be writing that multiple times
void clear() {
    system("cls");
}

vector<string> visuals;

void loadVisuals() {
    ifstream baseState("BaseGameState.txt");//the default state of the game
    string line;

    if (baseState.ifstream::is_open()) {
        while (getline(baseState, line)) {

            visuals.push_back(line);
        }
    }
    else {
        cout << "Error: Default visuals not found!";
    }

    baseState.close();
}

//prints the visuals to the console
void printVisuals() {
    for (int i = 0; i < visuals.size();i++) {
        cout << visuals[i] << endl;
    }
}

//the ball
class Ball {
public:
    Ball(int x, int y);
    void flipVert() { goingUp = !goingUp; };
    void flipHoriz() { goingRight = !goingRight; };
    pair<int, int> getPos() const { return position; }
    void resetPos() { position = make_pair(9, 7); }
    void moveBall(int horizMult = 1,int vertMult = 1, int pPos = -1, int cPos = -1);//the variables are the x speed of the ball,
                                                         //the y speed of the ball, the y pos of the player's paddle, and the y pos of the computer's paddle
    //draws the ball into the visuals
    void drawBall();

protected:
    //default ball position is (9,7) [0-indexed]
    pair<int, int> position;
    pair<int, int> lastPos;
    bool goingRight = true;
    bool goingUp = false;
};
//the ball constructor
Ball::Ball(int x, int y) {
    position = make_pair(x, y);
    lastPos = position;
}
void Ball::drawBall() {
    visuals[lastPos.first][lastPos.second] = '.';
    visuals[position.first][position.second] = 'O';
    lastPos = position;
}

//the paddles
class Paddle {
public:
    Paddle(int pos);
    int yPos = 8;
    //draws the paddles into the visuals
    void drawPaddle();

protected:
    int xPos;
    int delay = 0; //currently only used by the computer player. since the cpu and the ball normally move at the same time scoring would be impossible for the player
};
//the paddle constructor
Paddle::Paddle(int pos) {
    xPos = pos;
}
void Paddle::drawPaddle() {
    for (int i = 0;i < 13;i++) {
        visuals[i + 1][xPos] = '.';
    }
    visuals[yPos - 1][xPos] = '#';//draw 1 above the center
    visuals[yPos][xPos] = '#';//draw the center
    visuals[yPos + 1][xPos] = '#';//draw 1 below the center
}

void Ball::moveBall(int horizMult, int vertMult, int pPos, int cPos) {
    int vertDir = 1;
    int horizDir = 1;
    
    for (int i = 0; i < horizMult;i++) {
        
        
        //if the ball is in the third column and between 1 above and 1 below pPos OR in the 18th column and between 1 above and 1 below cPos, it reflects horizontally
        if ((position.first == 2 && position.first >= pPos - 1 && position.first <= pPos + 1) || (position.first == 17 && position.first >= cPos - 1 && position.first <= cPos + 1)) {
            flipHoriz();
        }

        if (goingRight == false) {
            horizDir = -1;
        }
        else { horizDir = 1; }
        position.first += horizDir;

    }

    for (int i = 0; i < vertMult;i++) {
        if (position.second == 1 || position.second == 13) {
            flipVert();
        }

        if (goingUp == false) {
            vertDir = -1;
        }
        else { vertDir = 1; }
        position.second += vertDir;
    }

    if (position.first < 1 || position.first > 18) {
        return;
    }

}

//the leaderboard
class Leaderboard {
public:
    void readHighscores();
    void writeHighscoresFile();
    void printHighscores();
    void insertHighscore();

    vector<pair<string, int>> getHighscores() const { return highscores; }
protected:
    vector<pair<string, int>> highscores;
    void removeHighscore();
    void resetHighscores();
};

Leaderboard bestScores;

void Leaderboard::printHighscores() {
    bool scoresPage = true;
    while (scoresPage) {
        clear();
        cout << "     Highscores" << endl;
        for (int i = 0;i < 5;i++) {
            cout << highscores[i].first << "    " << highscores[i].second << endl;
        }
        cout << endl;

        int answer = 0;
            
        cout << "What would you like to do?\n1. Delete an entry\n2. Reset all entries\n3. Return to menu\n";
        cin >> answer;
        if (answer == 3) {
            scoresPage = false;
            return;
        }
        else if (answer == 1) {
            int iAns = -1;
            bool deleting = true;
            while (deleting) {
                cout << "Which entry do you want to delete? To cancel, enter 0.\n";
                cin >> iAns;
                if (iAns == 0) {
                    deleting = false;
                    break;
                }
                else if (iAns < 0 || iAns > 5) {
                    cout << "That's not a valid entry!\n";
                }
                else if (highscores[iAns].first == "None") {
                    cout << "That entry is already empty!\n";
                }
                else {
                    cout << "Are you sure you want to delete entry " << iAns << "? Y/N\n";
                    char cAns;
                    cin >> cAns;
                    if (tolower(cAns) == 'y') {
                        deleting = false;
                        auto iter = highscores.begin() + (iAns - 1);
                    }
                    else {
                        continue;
                    }
                    
                    
                }
            }
        }
        else if (answer == 2) {
            cout << "Are you sure? Y/N\n";
            char cAns;
            cin >> cAns;
            if (tolower(cAns) == 'y') {
                resetHighscores();
            }
            else {
                continue;
            }
        }
        else {
            "That's not a valid option!\n";
        }
    }
}

void Leaderboard::readHighscores() {
    //holds the highscores file
    ifstream highscoreFile("highscores.txt");

    //holds what the stream pulled
    string scoreDat;

    if (highscoreFile.ifstream::is_open()) {
        while (getline(highscoreFile, scoreDat)) {

            vector<string> tempVec;
            istringstream curr(scoreDat);
            string s;
            while (std::getline(curr, s, ',')) {
                tempVec.push_back(s);
            }
            highscores.push_back(make_pair(tempVec[0],stoi(tempVec[1])));
        }
    }
    else {
        cout << "Error: No file found for highscores!";
    }

    highscoreFile.close();
}

//this function inserts a new highscore into the highscore vector
void Leaderboard::insertHighscore() {
    
}
//removes a highscore
void Leaderboard::removeHighscore() {

}

void Leaderboard::resetHighscores() {
    for (int i = 0; i < 5; i++) {
        highscores[i] = make_pair("None", 0);
    }
}

//handles the end of the game
void gameEnd() {
    system("cls");//this clears the console
    cout << "Game over! Your score: " << currentScore << endl;
    if(currentScore > 0){}
}

void initializeGameplay(Ball& ball, Paddle& pc, Paddle& cpup);

//the gameplay
void gameplay() {
    //make paddles for the playre and the computer
    Paddle player(1);
    Paddle computer(18);
    //make a ball
    Ball ball(7,9);
    int ballXSpeed = 1;
    int ballYSpeed = 1;

    visuals.clear();//remove whatever is currently in the visuals
    loadVisuals();//load visuals with the default state
    
    //the below section is the countdown
    for (int i = 3; i > 0; i--) {
        char num = i + 48;
        clear();
        visuals[7][9] = num;
        printVisuals();
        Sleep(500);
    }
    clear();
    visuals[7][8] = 'G';
    visuals[7][9] = 'O';
    visuals[7][10] = '!';
    printVisuals();
    visuals.clear();
    loadVisuals();
    Sleep(500);
    //the above is the countdown

    bool direction = rand() % 2;
    if (direction == true) {

    }

    bool isPlaying = true;
    //this while loop is the part where you actually play
    while (isPlaying) {
        clear();
        printVisuals();

    }

    //put this part at the very end
    ball.moveBall(ballXSpeed,ballYSpeed,player.yPos,computer.yPos);
    if (ball.getPos().first < 1) {
        isPlaying = false;
        gameEnd();
    }
    else if (ball.getPos().first > 18) {
        currentScore++;
    }
}

bool isPlaying = true; //when this is set to false the program will terminate
//the menu
void menu() {

    cout << "Welcome to PONG! Please select an option:" << endl;
    
    int option = 0;
    while (option < 1 || option > 3) {
        cout << "1. Start the game\n2. View highscores\n3. Quit" << endl;
        cin >> option;
        if (option == 3) {
            isPlaying = false;
            return;
        }
        else if (option == 1) {
            currentScore = 0;
            gameplay();
        }
        else if (option == 2) {

        }
        else {
            cout << "That's not a valid option!\n";
        }
    }
}

void initializeGameplay(Ball& ball, Paddle& pc, Paddle& cpup) {

}

//this function writes the current highscores to highscores.txt when called
void Leaderboard::writeHighscoresFile() {
    ofstream highscoresFile("highscores.txt");//the file to write to
    for (int i = 0; i < highscores.size();i++) {
        while (highscores[i].first.size() < 5) {
            highscores[i].first.append(" ");
        }
        highscoresFile << highscores[i].first << ", " << highscores[i].second << endl;
    }
    highscoresFile.close();

}

int main()
{
    srand(time(0));

    while (isPlaying) {
        menu();
    }

    bestScores.writeHighscoresFile();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
