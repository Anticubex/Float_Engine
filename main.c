#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "src/game.h"

int main(int argc, char* argv[]) {
        SDL_State state;

        int initGameCode = initGame(&state, argc, argv);

        if (initGameCode != 0) {
                return initGameCode;
        }

        if (runGame(&state) != 0) {
                destroyGame(&state);
                return 1;
        }

        destroyGame(&state);

        return 0;
}