#include <stdio.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <array>
#include <string>
#include <iostream>
#include <cstring>
#include "engine.hpp"
#include <vector>
//#include <GL/gl.h>
 
#define WIDTH 600
#define HEIGHT 300
#define IMG_PATH "font.png"
#define NEWLINE_PATH "newLine.png"
#define CURSOR_PATH "cursor.png"

class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};
Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

bool operator==(const SDL_Rect& a1, const SDL_Rect& a2)
{
    return a1.x == a2.x && a1.y == a2.y && a1.w == a2.w && a1.h == a2.h;
}

std::vector<SDL_Texture*> images;

int frame = 0;
const int FRAMES_PER_SECOND = 10;
Timer fps;
SDL_Window *win = NULL;
SDL_Rect texr;
SDL_Texture *img = NULL;
int count = 0;
int charX = 0;
int charY = 0;
SDL_Rect bruh;
SDL_Renderer *renderer = NULL;
SDL_Texture *font = NULL;
SDL_Texture *newLineImage = NULL;
SDL_Texture *cursorImage = NULL;
SDL_Rect newLine = { 279, 48, 9, 16 };
SDL_Rect cursor = { 279, 32, 9, 16 };


char codePage[128] = {
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '\?', 
'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', 
'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '\n'
};

SDL_Rect prevRect = { 0, 0, 0, 0 };

SDL_Texture* GetAreaTextrue(SDL_Rect rect, SDL_Renderer* renderer2, SDL_Texture* source)
{
	if (rect == newLine) {
		return newLineImage;
	}
	SDL_Texture* result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 9, 16);          
	SDL_SetRenderTarget(renderer2, result);
	SDL_RenderCopy(renderer2, source, &rect, NULL);
	// the folowing line should reset the target to default(the screen)
	SDL_SetRenderTarget(renderer2, NULL);  
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
	images.push_back(img);
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
	
	auto WindowFlags = SDL_WINDOW_OPENGL;
	
	
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;
	
	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, WindowFlags);
	//SDL_GLContext Context = SDL_GL_CreateContext(win);
	 //SDL_GLContext Context = SDL_GL_CreateContext(Window);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
	//renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	font = IMG_LoadTexture(renderer, IMG_PATH);
	cursorImage = GetAreaTextrue( cursor, renderer, font );
	newLineImage = IMG_LoadTexture(renderer, NEWLINE_PATH);
	//cursorImage = GetAreaTextrue( cursor, renderer, font );
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
   	unsigned char* raw_newline = reinterpret_cast<unsigned char*>(&newLineImage);
	//std::cout << newLineImage;
	
} 
int Blink = 0; 

void clearScreen() {
	SDL_RenderClear(renderer);
	charX = 0;
	charY = 0;
} 
int mainLoop() {
	// main loop
	
	while (1) {
		
		int newLineMode = 0;
		//charX = 0;
		//charY = 0;
		Blink += 1;
		
		// event handling
		SDL_Event e;
		if ( SDL_PollEvent(&e) ) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
		} 
		
		// clear the screen
		// SDL_RenderClear(renderer);
		// copy the texture to the rendering context
		
		SDL_Rect temp;temp.x = charX; temp.y = charY; temp.w = 9; temp.h = 16; 
		
		//SDL_RenderFillRect(renderer, &temp);
		auto TEMPORARY = images;
		SDL_RenderPresent(renderer);
		
		for(SDL_Texture *image : TEMPORARY) 
		{
			texr.x = charX; texr.y = charY; texr.w = 9; texr.h = 16; 
			if (image == newLineImage) {
				SDL_RenderFillRect(renderer, &bruh);
				bruh.x = charX; bruh.y = charY; bruh.w = 9; bruh.h = 16; 
				temp.x = charX; temp.y = charY; temp.w = 9; temp.h = 16;
				
				newLineMode = 1;
				//SDL_Delay(1000);
				Blink = 0;
				charX = 0;
				charY += 16;
				SDL_RenderFillRect(renderer, &bruh);
				
			} else {
				SDL_RenderCopy(renderer, image, NULL, &texr);
				temp.x = charX; temp.y = charY; temp.w = 9; temp.h = 16;
				charX += 9;
			}
			images.erase(images.begin());
		}
		 
		
		
		if (Blink == 10) {
			
			SDL_RenderFillRect(renderer, &bruh);
			
		} else if (Blink == 5) {
			
			SDL_RenderCopy(renderer, cursorImage, NULL, &bruh);
			
		}
		if (Blink == 10) {
			Blink = 0;
		}
		//SDL_RenderFillRect(renderer, &bruh);
		
		// flip the backbuffer NO FUCK YOU DOUBLE BUFFERING
		// YOU CAN SUCK MY
		// this means that everything that we prepared behind the screens is actually shown
		SDL_RenderPresent(renderer);
		SDL_RenderPresent(renderer);
		
		fps.start();
		SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
		
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
}