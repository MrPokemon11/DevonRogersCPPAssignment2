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
#include <conio.h> //this is used for the movement of the player's paddle

using namespace std;

unsigned int currentScore = 0;

vector<string> visuals;

//the ball
class Ball {
public:
    void flipVert() { goingUp = !goingUp; };
    void flipHorix() { goingRight = !goingRight; };
protected:
    pair<int, int> position;
    bool goingRight = true;
    bool goingUp = false;
};

//the paddles
class Paddle {
public:
    Paddle(int pos);
    int yPos = 8;
protected:
    int xPos;

    int height = 3;
};
//the paddle constructor
Paddle::Paddle(int pos) {
    xPos = pos;
}

//the leaderboard
class Leaderboard {
public:
    void readHighscores();
    void writeHighScores();
    void writeHighscoresFile();
    vector<pair<string, int>> getHighscores() const { return highscores; }
protected:
    vector<pair<string, int>> highscores;
};

Leaderboard bestScores;

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

//this function writes new highscores into the highscores vector
void Leaderboard::writeHighScores() {

}

//handles the end of the game
void gameEnd() {
    cout << "Game over! Your score: " << currentScore << endl;
    if(currentScore > 0){}
}

//the gameplay
void gameplay() {
    //make paddles for the playre and the computer
    Paddle player(1);
    Paddle computer(18);
    //make a ball
    Ball ball;
}

bool isPlaying = true; //when this is set to false the program will terminate
//the menu
void menu() {

    cout << "Welcome to PONG! Please select an option:" << endl;
    
    int option = 0;
    while (option < 1 || option > 3) {
        cout << "1. Start the game\n2. View highscores\n3.Quit" << endl;
        cin >> option;
        if (option == 3) {
            isPlaying = false;
            return;
        }
        else if (option == 1) {
            gameplay();
        }
        else if (option == 2) {

        }
        else {
            cout << "That's not a valid option!\n";
        }
    }
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
