#include <SDL2/SDL.h>
#include <core.h>
#include <stdbool.h>
#define WIDTH 640
#define HEIGHT 580
#define SIZE 200
#define NAME "Hello Platformer!"

typedef struct SDL_State {
        SDL_Window *wind;
        SDL_Renderer *rend;
} SDL_State;

typedef struct GameState {
        bool running;
        bool jump_pressed;
        bool can_jump;
        bool left_pressed;
        bool right_pressed;
        Vector pos;
        Vector vel;
        SDL_Rect rect;
        SDL_Event event;
} GameState;

int initGame(SDL_State *state, int argc, char *argv[]);
int destroyGame(SDL_State *state);

int runGame(SDL_State *state);

int update(SDL_State *state, GameState *gameState);

int processEvents(GameState *gameState);

int keyDownEvent(GameState *gameState);
int keyUpEvent(GameState *gameState);
