#pragma once

#define WIDTH 600
#define HEIGHT 300
#define IMG_PATH "font.png"
#define NEWLINE_PATH "newLine.png"

extern SDL_Window *win;
extern SDL_Texture *img;
extern int count;
extern int charX;
extern int charY;
extern SDL_Renderer *renderer;
extern SDL_Texture *font;
extern SDL_Texture *newLineImage;
extern SDL_Rect newLine;
extern char codePage[128];

extern bool operator==(const SDL_Rect& a1, const SDL_Rect& a2);

extern SDL_Texture* GetAreaTextrue(SDL_Rect rect, SDL_Renderer* renderer, SDL_Texture* source);

extern SDL_Rect codePageDecode(char letter);

extern void drawChar(char letter);

extern void drawChars(char* str);

extern void clearScreen();

extern int initWindow();
extern int mainLoop();