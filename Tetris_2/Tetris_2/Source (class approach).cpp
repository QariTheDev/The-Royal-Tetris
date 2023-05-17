#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Global.h"
#include "FileHandling.h"
#include "Tetriminos.h"
#include "Game.h"

using namespace std;
using namespace sf;

int main() {
	srand(time(NULL));

	Game tetris;

	tetris.run();

	return 0;

}
