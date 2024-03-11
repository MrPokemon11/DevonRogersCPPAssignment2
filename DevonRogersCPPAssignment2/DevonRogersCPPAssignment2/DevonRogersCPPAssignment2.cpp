// DevonRogersCPPAssignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h> //used to set rand() seed
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <conio.h> //this is used for the movement of the player's paddle, as well as other things
#include <stdlib.h> // this is used to clear the screen
#include <Windows.h> //this is used to make the program sleep temporarily
#include <utility> //has rand()
#include <chrono>

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

class Paddle;

//the ball
class Ball {
public:
	Ball(int y, int x);
	void flipVert() { goingUp = !goingUp; };
	void flipHoriz() { goingRight = !goingRight; };
	//remember that it is y position then x position
	pair<int, int> getPos() const { return position; }
	void resetPos() { position = make_pair(6 + (rand() % 3), 8 + (rand() % 3)); }//reseting the ball will put it within a 3x3 box surrounding the rough center of the grid
	void moveBall(int horizMult, int vertMult, Paddle& plyr, Paddle& cmptr);//the variables are the x speed of the ball,
	//the y speed of the ball, the y pos of the player's paddle, and the y pos of the computer's paddle
//draws the ball into the visuals
	void drawBall();

	//chooses whether the ball goes up or down by default at random
	void randoY();

	void setVert(bool vert) { goingUp = vert; }
protected:
	//default ball position is (7,9) [0-indexed]
	pair<int, int> position;
	pair<int, int> lastPos;
	bool goingRight = true;
	bool goingUp = false;
};
//the ball constructor
Ball::Ball(int y, int x) {
	position = make_pair(y, x);
	lastPos = make_pair(7, 9);
	lastPos = position;
}
void Ball::drawBall() {
	if (position.second > 19 || position.second < 0) {
		return;
	}
	visuals[lastPos.first][lastPos.second] = '.';
	visuals[position.first][position.second] = 'O';
	lastPos = position;
}
void Ball::randoY() {
	//this "randomizes" whether the ball goes up of down by default
	bool direction = rand() % 2;
	if (direction == true) {
		flipVert();
	}
}

//the paddles
class Paddle {
public:
	Paddle(int pos);
	int yPos = 7;
	//draws the paddles into the visuals
	void drawPaddle();
	void setDelay(int del) { delay = del; }

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

//this variable is only global so that the move ball function can change it
int paddleHits = 0;

void Ball::moveBall(int horizMult, int vertMult, Paddle& pPos, Paddle& cPos) {
	if (vertMult == NULL) {
		horizMult = 1;
	}
	if (horizMult == NULL) {
		vertMult = 1;
	}
	int vertDir = 1;
	int horizDir = 1;

	for (int i = 0; i < horizMult;i++) {


		//if the ball is in the third column and between 1 above and 1 below pPos OR in the 18th column and between 1 above and 1 below cPos, it reflects horizontally
		if ((position.second == 2 && position.first >= pPos.yPos - 1 && position.first <= pPos.yPos + 1) || (position.second == 17 && position.first >= cPos.yPos - 1 && position.first <= cPos.yPos + 1)) {
			flipHoriz();
			paddleHits++;
			if (position.second == 2 && position.first == pPos.yPos - 1 || position.second == 17 && position.first == cPos.yPos - 1) {
				if (position.first != 1) {
					setVert(true);
				}

			}
			else if (position.second == 2 && position.first == pPos.yPos + 1 || position.second == 17 && position.first == cPos.yPos + 1) {
				if (position.first != 13) {
					setVert(false);
				}

			}
		}
		//if the ball has ended up inside the paddle, correct that
		if (position.second == 1 && position.first == pPos.yPos - 1 || position.second == 18 && position.first == cPos.yPos - 1 || position.second == 1 && position.first == pPos.yPos + 1 || position.second == 18 && position.first == cPos.yPos + 1 || position.second == 1 && position.first == pPos.yPos || position.second == 18 && position.first == cPos.yPos ) {
			flipHoriz();
			flipVert();
			if (position.second == 1) {
				position.second = 2;
			}
			else {
				position.second = 17;
			}
		}

		if (goingRight == false) {
			horizDir = -1;
		}
		else { horizDir = 1; }
		position.second += horizDir;

	}

	for (int i = 0; i < vertMult;i++) {
		if (position.first == 1 || position.first == 13) {
			flipVert();
		}

		if (goingUp == false) {
			vertDir = -1;
		}
		else { vertDir = 1; }
		position.first += vertDir;
	}



	if (position.second < 1 || position.second > 18) {
		return;
	}

}

//the leaderboard
class Leaderboard {
public:
	void readHighscores();
	void writeHighscoresFile();
	void printHighscores();
	void insertHighscore(int spot, string name, int score);

	vector<pair<string, unsigned int>> getHighscores() const { return highscores; }
protected:
	vector<pair<string, unsigned int>> highscores;
	void resetHighscores();
};

Leaderboard bestScores;

void Leaderboard::printHighscores() {
	readHighscores();

	bool scoresPage = true;
	while (scoresPage) {
		clear();
		cout << " Highscores" << endl;
		for (int i = 0;i < 5;i++) {
			cout << highscores[i].first << "    " << highscores[i].second << endl;
		}
		while (highscores.size() > 5) {
			highscores.pop_back();
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
				else if (highscores[iAns - 1].second == 0) {
					cout << "That entry is already empty!\n";
				}
				else {
					cout << "Are you sure you want to the following entry? Y/N\n";
					cout << highscores[iAns - 1].first << ", " << highscores[iAns - 1].second << endl;
					char cAns;
					cin >> cAns;
					if (tolower(cAns) == 'y') {
						deleting = false;
						auto iter = highscores.begin() + (iAns - 1);
						highscores.erase(iter);
						highscores.push_back(make_pair("None ", 0));
						writeHighscoresFile();
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
			if (tempVec.size() < 2) {
				continue;
			}
			highscores.push_back(make_pair(tempVec[0], stoi(tempVec[1])));
		}
	}
	else {
		cout << "Error: No file found for highscores!";
	}

	highscoreFile.close();
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

//this function inserts a new highscore into the highscore vector
void Leaderboard::insertHighscore(int spot, string name, int score) {
	auto iter = highscores.begin() + spot;
	highscores.insert(iter, make_pair(name, score));
	highscores.pop_back();
	writeHighscoresFile();
}


void Leaderboard::resetHighscores() {
	for (int i = 0; i < 5; i++) {
		highscores[i] = make_pair("None", 0);
	}
	writeHighscoresFile();
}

//handles the end of the game
void gameEnd() {
	system("cls");//this clears the console
	if (currentScore > 0) {
		cout << "Game over! Your score: " << currentScore << endl;
		for (int i = 0; i < 5; i++) {
			if (bestScores.getHighscores()[i].second > currentScore) {
				continue;
			}
			else {
				cout << "You're in the top 5 players! Please enter your name (there is a 5 character limit): ";
				string name;
				cin >> name;
				while (name.size() > 5) {
					name.pop_back();
				}
				bestScores.insertHighscore(i, name, currentScore);
				break;
			}
		}
		if (bestScores.getHighscores()[4].second > currentScore) {
			cout << "You did not place on the leaderboard." << endl;
			Sleep(3000);
		}

	}
	else {
		cout << "Game over! You didn't score any points!" << endl;
		Sleep(3000);
	}
}

//sets/resets the positions of the important parts
void initializeGameplay(Ball& ball, Paddle& pc, Paddle& cpup); //the actual function is below the menu function for some reason

//the gameplay
void gameplay() {
	//make paddles for the player and the computer
	Paddle player(1);
	Paddle computer(18);
	//make a ball
	Ball ball(9, 7);
	int ballXSpeed = 1;
	int ballYSpeed = 1;

	visuals.clear();//remove whatever is currently in the visuals
	loadVisuals();//load visuals with the default state

	bool looping = true;
	bool isPlaying = true;
	//outer game loop
	while (looping) {
		initializeGameplay(ball, player, computer);

		//int reactionTime = 100; //- (currentScore*5);
		//debug
		//reactionTime = 5;
		//if (reactionTime < 250) {
			//reactionTime = 250;
		//}

		double timestep = 200;//the time between steps in milliseconds

		int delayMod = 4;

		paddleHits = 0;

		ballXSpeed = 1;

		//this while loop is the part where you actually play
		while (isPlaying) {

			clear();
			printVisuals();


			//every 6 paddle hits, the ball speeds up.
			if (paddleHits % 10 == 0 && paddleHits != 0) {
				ballXSpeed++;
			}

			char playerInput = 'Q';

			std::chrono::time_point<std::chrono::system_clock> start, end;
			std::chrono::duration<double> elapsed_seconds;
			start = std::chrono::system_clock::now();
			do {
				if (_kbhit()) {
					playerInput = _getch();//this gets input from the user *without requiring the user to press enter, and will continue on it's own even with no input*
					//break;
				}

				end = std::chrono::system_clock::now();
				elapsed_seconds = end - start;
			} while (elapsed_seconds.count() < timestep / 1000);



			if (playerInput == 'w' || playerInput == 'W') {
				if (player.yPos > 2) {
					player.yPos -= 1;//this moves the paddle up
				}

			}
			else if (playerInput == 's' || playerInput == 'S') {
				if (player.yPos < 12) {
					player.yPos += 1;//this moves the paddle down
				}

			}

			player.drawPaddle();


			if (currentScore >= 15) {
				delayMod = 10;//this one makes it a 90% chance of moving, because a mod of 1 is impossible
			}
			else if (delayMod <= 2) {
				//do nothing
			}
			else if (currentScore % 2 == 0 && currentScore != 0) {
				delayMod--;
			}

			if (delayMod == 10) {
				if (rand() % delayMod != 0) {
					if (computer.yPos < ball.getPos().first && computer.yPos != 12) {
						computer.yPos++;
					}
					else if (computer.yPos > ball.getPos().first && computer.yPos != 2) {
						computer.yPos--;
					}
				}
			}
			else if (rand() % delayMod == 0) {
				if (computer.yPos < ball.getPos().first && computer.yPos != 12) {
					computer.yPos++;
				}
				else if (computer.yPos > ball.getPos().first && computer.yPos != 2) {
					computer.yPos--;
				}
			}
			else if (rand() % 3 == 0) {
				if (computer.yPos < ball.getPos().first && computer.yPos != 12) {
					computer.yPos++;
				}
				else if (computer.yPos > ball.getPos().first && computer.yPos != 2) {
					computer.yPos--;
				}
			}
			Sleep(1);
			ball.moveBall(ballXSpeed, ballYSpeed, player, computer);
			computer.drawPaddle();
			ball.drawBall();




			//put this part at the very end

			if (ball.getPos().second < 1) {
				isPlaying = false;
				looping = false;
				gameEnd();
				return;
			}
			else if (ball.getPos().second > 18) {
				for (int i = 0;i < 13;i++) {
					visuals[i + 1][19] = '.';
				}
				currentScore++;
				break;
			}
		}

	}
}

bool isPlaying = true; //when this is set to false the program will terminate
//the menu
void menu() {
	clear();
	cout << "Welcome to PONG! Please select an option:" << endl;

	int option = 0;
	while (option < 1 || option > 4) {
		cout << "1. Start the game\n2. View highscores\n3. View the rules (recommended)\n4. Quit" << endl;
		cin >> option;
		if (option == 4) {
			isPlaying = false;
			return;
		}
		else if (option == 1) {
			currentScore = 0;
			gameplay();
		}
		else if (option == 2) {
			bestScores.printHighscores();
		}
		else if (option == 3) {
			cout << "Welcome to Pong! When you're done reading the rules, press enter to return to the menu." << endl;
			cout << "In this version, your goal is to score as many points as possible before your opponent scores one." << endl;
			cout << "While the computer starts off relatively easy, it will get tougher as you score more points." << endl;
			cout << "It is also recommended to zoom in, but that's not required." << endl;
			cout << "One last thing: try to score quickly, because the ball speeds up every 10 paddle hits!" << endl;
			cout << "Good luck!" << endl;
			cout << endl << "Controls:\nW: Move Paddle Up\nS: Move Paddle Down\n" << endl;

			//this is just so that you have time to read the rules
			string nada;
			std::getline(cin, nada);
			std::getline(cin, nada);
		}
		else {
			cout << "That's not a valid option!\n";
		}
	}
}

void initializeGameplay(Ball& ball, Paddle& pc, Paddle& cpup) {
	ball.resetPos();
	pc.yPos = 7;
	cpup.yPos = 7;

	paddleHits = 0;

	ball.randoY();

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
}



int main()
{
	bestScores.readHighscores();

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
