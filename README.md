# Welcome to ML Tanks!

## The final project for CSC 1120 by Michael Bisciglia and Lesya Protasova

This repo contains ML Tanks, a hyper-realistic war simulator.
The baseline of the program was provided by Professor Perry Kivolowitz, and all the good stuff was written by Michael Bisciglia and Lesya Protasova.

## Summary

ML Tanks is a game written in C++, and uses ncurses. The goal of the game is to shoot your opponent three times. The two players take turns changing the power and angle of their missile with WASD before firing with enter.

## Instructions

The gameplay consists of taking turns between the two players, Player 1 on the left side on the screen, and Player 2 on the right.

At any point when a shot is not taking place, either player may adjust their angle and power. Player 1 uses the WASD keys, with _w_ and _s_ for power and _a_ and _d_ for angle. Player 2 uses the arrow keys, with _up_ and _down_ for power and _left_ and _right_ for angle.

Once the player whose turn it is is satisfied with the angle and power of their missile, they may hit enter to fire. It will proceed to launch, and if it hits the other tank (or your own!), a life will be deducted, and a whole new terrain will be generated to battle in.

Alternatively, a player can give up their turn to move left or right with _n_ and _m_, repositioning themself to take advantage of features of the terrain, to get behind cover or get to higher ground, or simply to throw the opponent off if it seems they've almost worked out a perfect shot.

The first player to get off three hits on the opponent wins the game. At the end of a game, the players may choose to quit with the _q_ key, or they might want to play a rematch!

## Features

In addition to the requirements of simply finishing the game (hit detection, life system, redrawing after a hit, and a data structure for grouping elements that have an x and y component), ML Tanks has several noteworthy additional features.

The left/right movement is one example of this. This adds an additional layer of strategy to gameplay, and providing a greater sense of dynamicism.

Another innovation is the use of separate sets of controls for each player, improving the flow of the game by letting each player think and plan even while it is not their turn.

Notably, the terrain generation was also changed to improve replayability. Playing on the same field every time is likely to get stale fast, but the increased chance of hills, valleys, and peaks being spawned makes each round of each game unique and interesting.

## Future Changes

Over the course of development, other ideas were introduced that might be considered for future versions:
- playing a short sound file of an explosion every time a missile connected with a tank
- making different terain types that would be randomly selected from each time the terrain was generated, such as __hills__, where the ground would rise, then fall; __valleys__, which would first fall, then rise; __plains__, which would provide an even playing field; and __mountains__, where steep peaks would provide protection from opponent fire, but make it just as difficult to get your own shot off.
- And more!

## Conclusion

ML Tanks is a project we have invested a considerable amount of time and energy into, and we sincerely hope you enjoy playing it!
