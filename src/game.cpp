#include "game.h"

#include <SDL2/SDL.h>
#include <iostream>

/********************************
 * Currently a test app for     *
 * the SDL2 library import      *
 * and game engine abstractions *
 ********************************/

int initGame(SDL_State *state, int argc, char *argv[]) {

        std::cout << "Initializing: timer, audio, video, joystick, haptic, gamecontroller and events subsystems\n";
        /* Initializes the timer, audio, video, joystick,
        haptic, gamecontroller and events subsystems */
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                std::cout << "Error initializing SDL: " << SDL_GetError() << "\n";
                return 1;
        }

        std::cout << "Creating window\n";
        /* Create a window */
        state->wind = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
        if (!state->wind) {
                std::cout << "Error creating window: " << SDL_GetError() << "\n";
                SDL_Quit();
                return 1;
        }

        std::cout << "Creating renderer\n";
        /* Create a renderer */
        Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
        state->rend = SDL_CreateRenderer(state->wind, -1, render_flags);
        if (!state->rend) {
                std::cout << "Error creating renderer: " << SDL_GetError() << "\n";
                SDL_DestroyWindow(state->wind);
                SDL_Quit();
                return 1;
        }

        return 0;
}

int destroyGame(SDL_State *state) {
        /* Release resources */
        SDL_DestroyRenderer(state->rend);
        SDL_DestroyWindow(state->wind);
        SDL_Quit();
        return 0;
}

int runGame(SDL_State *state) {

        GameState gameState = {
            true,
            false,
            true,
            false,
            false,
            Vector((WIDTH - SIZE) / 2, SIZE / 2),
            Vector(0, 0),
            {(WIDTH - SIZE) / 2, (HEIGHT - SIZE) / 2, SIZE, SIZE},

        };

        while (gameState.running) {
                update(state, &gameState);
        }

        return 0;
}

int update(SDL_State *state, GameState *gameState) {
        /* Main loop */

        /* Process Events */
        processEvents(gameState);

        /* Clear screen */
        SDL_SetRenderDrawColor(state->rend, 0, 0, 0, 255);
        SDL_RenderClear(state->rend);

        /* Move the rectangle */
        gameState->vel.x = (gameState->right_pressed - gameState->left_pressed) * SPEED;
        gameState->vel.y += GRAVITY;

        if (gameState->jump_pressed && gameState->can_jump) {
                gameState->can_jump = false;
                gameState->vel.y = JUMP;
        }

        gameState->pos += gameState->vel / FPS;

        if (gameState->pos.x <= 0)
                gameState->pos.x = 0;

        if (gameState->pos.x >= WIDTH - gameState->rect.w)
                gameState->pos.x = WIDTH - gameState->rect.w;

        if (gameState->pos.y <= 0)
                gameState->pos.y = 0;

        if (gameState->pos.y >= HEIGHT - gameState->rect.h) {
                gameState->vel.y = 0;
                gameState->pos.y = HEIGHT - gameState->rect.h;

                if (!gameState->jump_pressed)
                        gameState->can_jump = true;
        }

        gameState->rect.x = (int)gameState->pos.x;
        gameState->rect.y = (int)gameState->pos.y;

        /* Draw the rectangle */
        SDL_SetRenderDrawColor(state->rend, 255, 0, 0, 255);
        SDL_RenderFillRect(state->rend, &gameState->rect);

        /* Draw to window and loop */
        SDL_RenderPresent(state->rend);
        SDL_Delay(1000 / FPS);

        return 0;
}

int processEvents(GameState *gameState) {
        /* Process events */
        while (SDL_PollEvent(&gameState->event)) {
                switch (gameState->event.type) {
                case SDL_QUIT:
                        gameState->running = false;
                        break;
                case SDL_KEYDOWN:
                        keyDownEvent(gameState);
                        break;
                case SDL_KEYUP:
                        keyUpEvent(gameState);
                        break;
                default:
                        break;
                }
        }

        return 0;
}

int keyDownEvent(GameState *gameState) {
        switch (gameState->event.key.keysym.scancode) {
        case SDL_SCANCODE_SPACE:
                gameState->jump_pressed = true;
                break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
                gameState->left_pressed = true;
                break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
                gameState->right_pressed = true;
                break;
        default:
                break;
        }

        return 0;
}

int keyUpEvent(GameState *gameState) {
        switch (gameState->event.key.keysym.scancode) {
        case SDL_SCANCODE_SPACE:
                gameState->jump_pressed = false;
                break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
                gameState->left_pressed = false;
                break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
                gameState->right_pressed = false;
                break;
        default:
                break;
        }

        return 0;
}