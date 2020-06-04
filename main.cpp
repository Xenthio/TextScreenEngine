#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <thread>
#include "engine.hpp"

void trueMain() {
	drawChars("C:\\> echo What's up, Dude?\n");
	drawChars("What's up, Dude?\n");
	SDL_Delay(2000);
	drawChars("Hello\n");
	SDL_Delay(2000);
	drawChars("Hello Again\n");
	SDL_Delay(5000);


} 

int main (int argc, char *argv[]) {
	initWindow();
	// load the 'main' function
	std::thread t1(trueMain);
	mainLoop();
	t1.join();
	return 0;

}

