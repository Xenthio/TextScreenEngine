#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <thread>
#include "engine.hpp"

void trueMain() {
	drawChars("C:\\> echo What's up, Dude?\n");
	drawChars("What's up, Dude?");
	SDL_Delay(2000);
	drawChars("\nHello");
	SDL_Delay(2000);
	drawChars("\nHello Again");

} 

int main (int argc, char *argv[]) {
	initWindow();
	// load the 'main' function
	std::thread t1(trueMain);
	mainLoop();
	t1.join();
	return 0;

}

