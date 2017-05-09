//Lesya Protasova
//Michael Bisciglia

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cmath>
#include <Windows.h>
#undef MOUSE_MOVED

#include "curses.h"
#include "ground.hpp"
#include "player.hpp"

using namespace std;

extern int lines;
extern int cols;
extern int base_height_divisor;
extern int max_height_divisor;

const double PI = 3.141592653589793238463;


/*	
	Coord2D is a constructed type that holds two doubles as a pair of coordinates
	The struct Coord2D also has operators to allow for math computations
	Computations that produces the position in type Coord2D of the bomb
	This is to make neat all usage of coordnate based positions ex. Tank1 and 2 force, gravity, and bomb
*/
struct Coord2D
{
	double xComponent = 0.0;
	double yComponent = 0.0;
	void Initialize(double, double);

	Coord2D operator*(int);
	Coord2D operator*(double);
	Coord2D operator+(const Coord2D &);
	bool operator==(Coord2D &);
};

//	Used to assign two double values construct each Coord2D
void Coord2D::Initialize(double xVal, double yVal)
{
	xComponent = xVal;
	yComponent = yVal;
}

Coord2D Coord2D::operator* (int multiplier)
{
	Coord2D productCoord;
	productCoord.xComponent = this->xComponent * multiplier;
	productCoord.yComponent = this->yComponent * multiplier;
	return productCoord;
}

Coord2D Coord2D::operator* (double multiplier)
{
	Coord2D productCoord;
	productCoord.xComponent = this->xComponent * multiplier;
	productCoord.yComponent = this->yComponent * multiplier;
	return productCoord;
}

Coord2D Coord2D::operator+ (const Coord2D & otherCoord)
{
	Coord2D sumCoord;
	sumCoord.xComponent = this->xComponent + otherCoord.xComponent;
	sumCoord.yComponent = this->yComponent + otherCoord.yComponent;
	return sumCoord;
}

//	Used in hit detection
bool Coord2D::operator== (Coord2D & otherCoord)
{
	bool areEqual = true;
	
	if ((int) this->xComponent != (int) otherCoord.xComponent)
	{
		areEqual = false;
	}
	else if ((int) this->yComponent != (int) otherCoord.yComponent)
	{
		areEqual = false;
	}

	return areEqual;
}
/*
	DrawScreen passes in ground as a paramater to place add the correct '-' in the correct positions
	players is also passed in as a pointer, since it is an array
	turn is passed in for the DrawSettings, which places each tank at the position on the ground
	DrawScreen is a void as it only draws the screen
*/
void DrawScreen(Ground g, Player * players, int turn)
{
	erase();
	box(stdscr, 0, 0);
	g.Draw();
	players[0].Draw(g);
	players[1].Draw(g);
	players[0].DrawSettings(turn);
	players[1].DrawSettings(turn);
	move(0, 0);
	refresh();
}

/*
	ShuffleScreen passes in the reference to tank 1 and 2
	and the reference to the position of ground 
	Ground is re-initialized and made random
	Then the tanks are placed randomly on the ground where we allow
*/
void ShuffleScreen(Player & left, Player & right, Ground & gee)
{
	clear();
	gee.InitializeGround();
	left.Initialize(rand() % (cols / 4) + 1, LEFT);
	right.Initialize(rand() % (cols / 4) + 3 * cols / 4 - 3, RIGHT);
}

/*
	Function that returns bool to determine if a life needs to be taken from a player
	This function is called twice, once for each player
	The position of tank and bomb are passed in
*/
bool TankHit(Coord2D tank, Coord2D bomb)
{
	bool hit = false;
	Coord2D tempTank;

	//	For loop that check 9 boxes around the tank
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; i <= 1; i++)
		{
			tempTank.xComponent = tank.xComponent + i;
			tempTank.yComponent = tank.yComponent + j;
			if (tempTank == bomb)
			{
				hit = true;
				break;
			}
		}
		if (hit == true)
		{
			break;
		}
	}
	return hit;
}

/*
	Shoot function is a bool, and returns a bool of tank being hit or not
	The Ground and players are passed into shoot to determine where the shot is coming from
	turn is passed in to determine which player will be firing the next shot
	tankWasHit bool is changed within the TankHit function
*/
bool Shoot(Ground & g, Player * players, int turn)
{
	bool tankWasHit = false;

	//conversion from degrees to radians
	double angle = players[turn].angle / 180.0 * PI;

	Coord2D force;
	force.Initialize(cos(angle) * players[turn].power * 0.2, sin(angle) * players[turn].power * 0.2);

	//declaring bombPos
	Coord2D bombPos;

	if (players[turn].s == RIGHT)
		force.xComponent *= -1;

	Coord2D tankPos;
	tankPos.Initialize(players[turn].col, lines - g.ground.at(players[turn].col));
	Coord2D tempTankPos;
	tempTankPos.Initialize(players[turn].col, g.ground.at(players[turn].col));

	Coord2D oppTankPos;
	oppTankPos.Initialize(players[1-turn].col, lines - g.ground.at(players[1-turn].col));
	Coord2D tempOppTankPos;
	tempOppTankPos.Initialize(players[1 - turn].col, g.ground.at(players[1 - turn].col));

	Coord2D gravity;
	gravity.Initialize(0, -0.98);

	int delayTimer = 5;

	for (int i = 1; i < 5000; i++)
	{
		//the larger the divisor, the smaller the interval between asterisks
		double di = i / 10.0;

		//defining bombPos each time we come through the for loop
		bombPos = tankPos + force * di + gravity * 0.5 * (di * di + di);
		bombPos.yComponent = lines - bombPos.yComponent;

		if (bombPos.xComponent < 1 || bombPos.xComponent >= cols - 3)
			break;

		if (bombPos.yComponent < 2)
		{
			//Change this const as needed for speed of **
			Sleep(25);
			continue;
		}

		move((int)bombPos.yComponent - 1, (int)bombPos.xComponent + 1);
		addch('*');
		refresh();
		
		if (delayTimer > 0)
			delayTimer--;
		else
		{
			//hit detection
			if (TankHit(tempTankPos, bombPos))
			{
				players[turn].lives--;
				tankWasHit = true;
				break;
			}
			else if (TankHit(tempOppTankPos, bombPos))
			{
				players[1 - turn].lives--;
				tankWasHit = true;
				break;
			}
		}

		//breaks out of loop if bomb would be below ground
		if (bombPos.yComponent - 1 > g.ground.at((int)bombPos.xComponent))
			break;

		Sleep(25);
	}
	return tankWasHit;
}

/*
	A simple EndScreen that passes in the winning player to display the winner
	move is generalized to center strings of all fitting sizes
*/
bool EndScreen(bool didPlayer1Win)
{
	bool wantsToContinue = false;
	for (int row = 0; row < lines; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			mvaddch(row, col, '-');
		}
	}

	string contents;
	contents = "Player ";
	contents += didPlayer1Win ? "1" : "2";
	contents += " won!";
	move(lines / 2, cols / 2 - (contents.length() / 2));
	addstr(contents.c_str());

	contents = "To exit the game, press Q. To play again, press any other button";
	move(lines / 2 + 1, cols / 2 - (contents.length() / 2));
	addstr(contents.c_str());

	refresh();
	char input = getch();

	if (input != 'q' && input != 'Q')
	{
		wantsToContinue = true;
	}
	return wantsToContinue;
}


/*
	A simple start screen that tells the players how to play
	Nothing is passed in, since the instructions will always be the same,
	and nothing is returned, since Instructions() is a void function
*/
void Instructions()
{
	for (int row = 0; row < lines; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			mvaddch(row, col, '-');
		}
	}

	string contents;

	contents = "Welcome to ML Tanks!";
	move(lines / 2 - 2, cols / 2 - (contents.length() / 2));
	addstr(contents.c_str());

	contents = "Player 1 uses WASD to adjust power and angle";
	move(lines / 2 - 1, cols / 2 - (contents.length() / 2));
	addstr(contents.c_str());

	contents = "Player 2 uses Arrow Keys to adjust power and angle";
	move(lines / 2, cols / 2 - (contents.length() / 2));
	addstr(contents.c_str());

	contents = "Both players use N and M to move left and right, and enter to shoot";
	move(lines / 2 + 1, cols / 2 - (contents.length() / 2));
	addstr(contents.c_str());

	contents = "Press any key to begin!";
	move(lines / 2 + 2, cols / 2 - (contents.length() / 2));
	addstr(contents.c_str());
	refresh();
	getch();
}

/*
	int main is where the code starts, and where the loop of gameplay is housed
*/
int main(int argc, char * argv[])
{
	srand((unsigned int)time(nullptr));

	int turn = 0;
	bool keep_going = true;
	Ground g;
	Player players[2];

	initscr();
	noecho();
	resize_term(lines, cols);
	keypad(stdscr, 1);

	Instructions();

	ShuffleScreen(players[0], players[1], g);

	DrawScreen(g, players, turn);
	while (keep_going)
	{
		bool show_char = false;
		int c = getch();
		switch (c)
		{
		case 27:
			keep_going = false;
			break;
			
		//the WASD keys for P1
		case 's':
			players[0].PowerDown();
			break;

		case 'w':
			players[0].PowerUp();
			break;

		case 'd':
			players[0].AngleUp();
			break;

		case 'a':
			players[0].AngleDown();
			break;
			
		//the arrow keys for P2
		case 259:
			players[1].PowerUp();
			break;
		case 258:
			players[1].PowerDown();
			break;
		case 261:
			players[1].AngleUp();
			break;
		case 260:
			players[1].AngleDown();
			break;

		//using n and m to move tank left or right instead of shooting
		case 'n':
			if (players[turn].col > 3)
			{
				players[turn].col -= 2;
				turn = 1 - turn;
			}
			break;
		case 'm':
			if (players[turn].col < cols - 4)
			{
				players[turn].col += 2;
				turn = 1 - turn;
			}
			break;

		case 10:
		case KEY_ENTER:
		case PADENTER:
			
			if (Shoot(g, players, turn))
			{
				if (players[0].lives == 0)
				{
					if (EndScreen(true) == true)
					{
						players[0].lives = 3;
						players[1].lives = 3;
					}
					else
					{
						keep_going = false;
					}
				}
				else if (players[1].lives == 0)
				{
					if (EndScreen(false) == true)
					{
						players[0].lives = 3;
						players[1].lives = 3;
					}
					else
					{
						keep_going = false;
					}
				}
				ShuffleScreen(players[0], players[1], g);
			}
		
			turn = 1 - turn;
			break;

		default:
			show_char = true;
			break;
		}
		if (keep_going)
			DrawScreen(g, players, turn);
		if (show_char) {
			move(0, 1);
			stringstream ss;
			ss << setw(4) << c << " ";
			addstr(ss.str().c_str());
			refresh();
		}
	}
	erase();
	addstr("Hit any key to exit");
	refresh();
	getch();
	echo();
	endwin();
	return 0;
}