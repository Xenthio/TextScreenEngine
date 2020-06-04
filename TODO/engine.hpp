#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 570
#define HEIGHT 350

int CHARX = 0;
int CHARY = 0;

SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *img = NULL;

void drawChar (char *toprint) {
	
	int w, h; // texture width & height
	char *IMG_PATH = "nui";
	if (toprint == "a") {
		IMG_PATH = "CapitalA.png";
	}
	if (toprint == "b") {
		IMG_PATH = "CapitalB.png";
	}
	if (toprint == "c") {
		IMG_PATH = "CapitalC.png";
	}
	printf("ok");
	img = IMG_LoadTexture(renderer, IMG_PATH);
	SDL_QueryTexture(img, NULL, NULL, &w, &h);
	SDL_Rect texr; texr.x = CHARX; texr.y = CHARY; texr.w = w; texr.h = h; 
	CHARX += 9;

}
int main (int argc, char *argv[]) {

	// variable declarations
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;
	
	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Text Engine test", 100, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	
	printf("fd");
	drawChar("a");
	drawChar("b");
	drawChar("c");
	// main loop
	SDL_Rect texr; texr.x = CHARX; texr.y = CHARY; texr.w = 9; texr.h = 16; 
	while (1) {
		
		// event handling
		SDL_Event e;
		if ( SDL_PollEvent(&e) ) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
		} 
		
		// clear the screen
		SDL_RenderClear(renderer);
		// copy the texture to the rendering context
		SDL_RenderCopy(renderer, img, NULL, &texr);
		// flip the backbuffer
		// this means that everything that we prepared behind the screens is actually shown
		SDL_RenderPresent(renderer);
		
	}
	
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	
	return 0;
}