#include <SDL2/SDL.h>
#include <iostream>
#include <stdbool.h>

#include "src/game.h"

#include "src/core/xmath.h"

#include <math.h>

int main(int argc, char *argv[]) {

        using namespace xmath;

        float x = 100;
        float invSqrt = 0.1f;
        float norm = 1 / sqrt(x);
        float fast = fastInvSqrt(x);

        std::cout << x << "\n"
                  << invSqrt << "\n"
                  << norm << "\n"
                  << fast << "\n";

        for (size_t i = 0; i < 10; i++) {
                float precise_fast = precise_fastInvSqrt(x, i);
                std::cout << "(" << i << "): " << precise_fast << "\n";
        }

        // SDL_State state;

        // int initGameCode = initGame(&state, argc, argv);

        // if (initGameCode != 0) {
        //         return initGameCode;
        // }

        // if (runGame(&state) != 0) {
        //         destroyGame(&state);
        //         return 1;
        // }

        // destroyGame(&state);

        return 0;
}