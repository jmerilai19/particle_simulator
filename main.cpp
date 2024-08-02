#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PIXEL_SIZE = 8;
const int UPDATE_INTERVAL = 50; // Update interval in milliseconds

int main(int argc, char *argv[]) {
    bool running = true;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event windowEvent;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Rect pixel = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, PIXEL_SIZE, PIXEL_SIZE};

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    int world[SCREEN_HEIGHT / PIXEL_SIZE][SCREEN_WIDTH / PIXEL_SIZE] = {0};
    Uint32 lastUpdateTime = SDL_GetTicks();

    bool isMouseDown = false;
    int mouseX = 0;
    int mouseY = 0;

    while (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 236, 204, 162, 255);

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
            for (int y = SCREEN_HEIGHT / PIXEL_SIZE - 1; y >= 0; y--) {
                for (int x = 0; x < SCREEN_WIDTH / PIXEL_SIZE; x++) {
                    if (world[y][x] == 1 && y + 1 < SCREEN_HEIGHT / PIXEL_SIZE) {
                        if (world[y + 1][x] == 0) {
                            world[y + 1][x] = 1;
                            world[y][x] = 0;
                        } else if (x - 1 >= 0 && world[y + 1][x - 1] == 0) {
                            world[y + 1][x - 1] = 1;
                            world[y][x] = 0;
                        } else if (x + 1 < SCREEN_WIDTH / PIXEL_SIZE && world[y + 1][x + 1] == 0) {
                            world[y + 1][x + 1] = 1;
                            world[y][x] = 0;
                        }
                    }
                }
            }
            lastUpdateTime = currentTime;
        }

        for (int y = 0; y < SCREEN_HEIGHT / PIXEL_SIZE; y++) {
            for (int x = 0; x < SCREEN_WIDTH / PIXEL_SIZE; x++) {
                if (world[y][x] == 1) {
                    pixel.x = x * PIXEL_SIZE;
                    pixel.y = y * PIXEL_SIZE;
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT) {
                running = false;
                break;
            } else if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                isMouseDown = true;
                mouseX = windowEvent.button.x;
                mouseY = windowEvent.button.y;
            } else if (windowEvent.type == SDL_MOUSEBUTTONUP) {
                isMouseDown = false;
            } else if (windowEvent.type == SDL_MOUSEMOTION) {
                mouseX = windowEvent.motion.x;
                mouseY = windowEvent.motion.y;
            }
        }

        if (isMouseDown) {
            int x = int(mouseX / PIXEL_SIZE);
            int y = int(mouseY / PIXEL_SIZE);
            if (world[y][x] == 0) {
                world[y][x] = 1;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
