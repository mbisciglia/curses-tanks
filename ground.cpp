#include "curses.h"
#include "ground.hpp"

int lines = 40;
int cols = 120;
/*	this is a constant that determines how far down the screen the ground starts
	having it be 8 means we start 1/8th of the screen's height up
	having it be 2 means we start 1/2 of the screen's height up
	*/
int base_height_divisor = 3;
int max_height_divisor = 2;

void Ground::InitializeGround()
{
	int current_height = lines - lines / base_height_divisor;
	int maximum_height = lines / max_height_divisor;
	//this is a const that can be 0 to 5000. the greater the number the more the ground will vary
	const int VOLATILITY = 2000;

	for (int i = 1; i < cols - 1; i++) {
		int h = current_height;
		int r = rand() % 10000;

		if (r < VOLATILITY) {
			current_height++;
			if (current_height >= lines - 2)
				current_height = lines - 2;
		}
		else if (r < VOLATILITY * 2) {
			current_height--;
			if (current_height < 1)
				current_height = 1;
		}
		ground.push_back(h);
	}
}

void Ground::Draw()
{
	for (size_t i = 0; i < ground.size(); i++) 
	{
		mvaddch(ground.at(i), i + 1, '-');
	}
}
