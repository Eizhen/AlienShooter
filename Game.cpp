#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "header.h"
/*#include "Graphic.h"
#include "Map.h"
#include "Spaceship.h"*/

using namespace std;

int main () {
	Graphic graphic;
	Movement movement;
//	int sdf = 0;
	const int clockPerMS = 1000, pressDelay = 8;
	int game_time = 0;
	srand (time(NULL));
	//clock_t lastPress = 0, lastTime = 0;
//	graphic.start_game ();
	graphic.showMenu();
	movement.changeLevel (graphic.map);
	Server server;
	
	while (graphic.is_open()) {

		//cerr << sdf++ << endl;
	
		graphic.game.clear();
		graphic.repaint ();
        	graphic.game.display();
		
		if (gameMode == 1) {	
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
				graphic.map.spaceship1.move_up();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				graphic.map.spaceship1.move_down();
			}
			/*else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){// && (clock()-lastPress)/clockPerMS > pressDelay ) {
				//lastPress = clock();
				graphic.map.spaceship1.overtake(graphic.map.spaceship2);
			}*/
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
				movement.shoot (graphic.map.spaceship1, graphic.map, 1);
			}
			else {
				graphic.map.spaceship1.move();
			}
		}
		
		else {
			int nextShip1Move = server.nextMove (graphic.map, graphic.map.spaceship1, graphic.map.spaceship2); 
			switch (nextShip1Move) {
				case up:
					graphic.map.spaceship1.move_up();
				break;
				case down:
					graphic.map.spaceship1.move_down();
				break;
				case left:
					graphic.map.spaceship1.overtake (graphic.map.spaceship2);
				break;
				case right:
					movement.shoot (graphic.map.spaceship1, graphic.map, 1);
				break;
					
			}
			graphic.map.spaceship1.move();
			movement.shoot (graphic.map.spaceship2, graphic.map, 2);
			/*int nextShip2Move = server.nextMove (graphic.map, graphic.map.spaceship2, graphic.map.spaceship1); 
			switch (nextShip2Move) {
				case up:
					graphic.map.spaceship2.move_up();
				break;
				case down:
					graphic.map.spaceship2.move_down();
				break;
				case left:
					graphic.map.spaceship2.overtake (graphic.map.spaceship1);
				break;
				//case right:
				//	movement.shoot (graphic.map.spaceship2, graphic.map, 2);
				//break;
					
			}
			graphic.map.spaceship2.move();*/
		}
			
		graphic.moveBackground();
		movement.move (graphic.map, graphic);
		if (gameClock.getElapsedTime() >= sf::milliseconds(gameDuration)) {
			level++;
			if (gameMode == 2 && level == 4)
				graphic.gameOver();
			/*if (gameMode == 1 && level == 5)
				graphic.gameOver();*/
			gameClock.restart();
			movement.changeLevel (graphic.map);
			graphic.startLevel();
		}
		//cerr << game_time++ << endl;
	        sf::Event event;
        	while (graphic.game.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
		                graphic.game.close();
        	}
		
    	}
	return 0;
}
