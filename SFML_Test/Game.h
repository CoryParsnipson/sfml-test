#ifndef GAME_H
#define GAME_H

#include "dependencies.h"

#include "Entities/ScreenWriter.h"

class Game {
public:
	Game();
	~Game();

	void loop(); // game activity happens here

	// helper functions
	void process_event();

	ScreenWriter sw;
	sf::RenderWindow window;
};

#endif