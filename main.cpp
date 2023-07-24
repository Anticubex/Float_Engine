#include <SDL2/SDL.h>
#include <iostream>
#include <stdbool.h>

#include "src/game.h"

#include "src/core/xmath.h"

#include <math.h>

int main(int argc, char *argv[]) {

        using namespace xmath;
        using namespace std;

        Matrix A(1, 2);

        cout << "(" << A.rows << ", " << A.cols << "): [";
        for (float x : A.values) {
                cout << x << "\t";
        }
        cout << "]\n";

        cout << A(0, 0) << "\t" << A(0, 1) << "\n";
        // cout << A(1, 0) << "\t" << A(1, 1) << "\n";

        A(0, 1) = 10.01;
        cout << "\n";

        cout << A(0, 0) << "\t" << A(0, 1) << "\n";
        // cout << A(1, 0) << "\t" << A(1, 1) << "\n";

        // Main loop
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