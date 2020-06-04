#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <array>
#include <string>
#include <iostream>
#include <cstring>
#include "engine.hpp"

#define WIDTH 600
#define HEIGHT 300
#define IMG_PATH "font.png"
#define NEWLINE_PATH "newLine.png"

bool operator==(const SDL_Rect& a1, const SDL_Rect& a2)
{
    return a1.x == a2.x && a1.y == a2.y && a1.w == a2.w && a1.h == a2.h;
}

SDL_Texture *images[64];

SDL_Window *win = NULL;
SDL_Texture *img = NULL;
int count = 0;
int charX = 0;
int charY = 0;
SDL_Renderer *renderer = NULL;
SDL_Texture *font = NULL;
SDL_Texture *newLineImage = NULL;
SDL_Rect newLine = { 279, 48, 9, 16 };


char codePage[128] = {
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '\?', 
'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', 
'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '\n'
};

SDL_Rect prevRect = { 0, 0, 0, 0 };

SDL_Texture* GetAreaTextrue(SDL_Rect rect, SDL_Renderer* renderer, SDL_Texture* source)
{
	if (rect == newLine) {
		return newLineImage;
	}
	SDL_Texture* result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 9, 16);          
	SDL_SetRenderTarget(renderer, result);
	SDL_RenderCopy(renderer, source, &rect, NULL);
	// the folowing line should reset the target to default(the screen)
	SDL_SetRenderTarget(renderer, NULL);  
	// I also removed the RenderPresent funcion as it is not needed here      
	return result;
}

SDL_Rect codePageDecode(char letter) {
	int count = 0;
	int posX = 0; // times by 9!
	int posY = 0; // times by 16!
	int found = 0;
	for( unsigned int a = 0; a < sizeof(codePage)/sizeof(codePage[0]); a = a + 1 ) {

		//if (codePage[a] == '\n') {
			//std::cout << reinterpret_cast<unsigned char*>(&newLineImage);
			//std::cout << reinterpret_cast<unsigned char*>(&GetAreaTextrue( newLine, renderer, font ));
			//return newLine;
		//}
		if (letter == codePage[a]) {
			int a = posX * 9;
			int b = posY * 16;
			SDL_Rect OK = { a, b, 9, 16 };
			prevRect = OK;
			
			//std::cout << "(";
			//std::cout << std::to_string(posX * 9) << ", ";
			//std::cout << std::to_string(posY * 16) << ")\n";
			return OK;
			
		}
		if (posX == 31) {
			posX = 0;
			
			posY += 1;
		} else {
			posX += 1;
		}


	
	}
	
} 

void drawChar(char letter) {
	SDL_Texture *img = NULL;
	
	
	SDL_Rect area = codePageDecode(letter);
	img = GetAreaTextrue( area, renderer, font );
	images[count] = img;
	count += 1;

}

void drawChars(char* str) {
	for(char* it = str; *it; ++it) {
   		drawChar(*it);
	} 
	charX = 0;
}

int initWindow() {
	// variable declarations
	
	
	
	
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;
	
	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	font = IMG_LoadTexture(renderer, IMG_PATH);
	newLineImage = IMG_LoadTexture(renderer, NEWLINE_PATH);
	
   	unsigned char* raw_newline = reinterpret_cast<unsigned char*>(&newLineImage);
	//std::cout << newLineImage;
	
} 

int mainLoop() {
	// main loop
	
	while (1) {
		if (sizeof(images) != 0) {
		charX = 0;
		charY = 0;
		
		
		// event handling
		SDL_Event e;
		if ( SDL_PollEvent(&e) ) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
		} 
		
		// clear the screen
		//SDL_RenderClear(renderer);
		// copy the texture to the rendering context
		
		for( unsigned int a = 0; a < sizeof(images)/sizeof(images[0]); a = a + 1 )
		{
			SDL_Rect texr; texr.x = charX; texr.y = charY; texr.w = 9; texr.h = 16; 
			unsigned char* raw_image = reinterpret_cast<unsigned char*>(&images[a]);
			if (images[a] == newLineImage) {
				charX = 0;
				charY += 16;
			} else {
				SDL_RenderCopy(renderer, images[a], NULL, &texr);
				charX += 9;
			}
			
		}
		// flip the backbuffer
		// this means that everything that we prepared behind the screens is actually shown
		SDL_RenderPresent(renderer);
		}
		
	}
	SDL_DestroyTexture(img);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
}