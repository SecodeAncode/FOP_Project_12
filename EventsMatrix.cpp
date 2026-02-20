#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfx.h>
#include <cstring>



using namespace std;


const int w = 800;
const int h = 600;

struct ScratchMonitor {

    char currentAction[100];

    char lastMessage[50];
    bool hasNewMessage;

    Uint32 messageTimer;
};

void drawGreenFlag(SDL_Renderer* renderer , ScratchMonitor & monitor) ;

int main(int argc , char *argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Scratch_Engine_Events" , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_SHOWN );

    SDL_Renderer* r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



    ScratchMonitor monitor = { "Waiting ..." , "" ,false , 0};


    bool quit = false;
    SDL_Event e;

    while(!quit) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT)
                quit = true;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x , y;
                SDL_GetMouseState(&x, &y);

                if (x > 20 && x < 80 && y > 20 && y < 50) {
                  strcpy(monitor.currentAction , "PROGRAM STARTED! ");
                    monitor.messageTimer = SDL_GetTicks()+2000;
                }
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    strcpy(monitor.currentAction , "KEY SPACE PRESSED!");
                    monitor.messageTimer = SDL_GetTicks()+2000;

                    strcpy(monitor.lastMessage , "LevelUp");

                    monitor.hasNewMessage = true;
                }

            }
        }
        if (monitor.hasNewMessage) {
            if (strcmp(monitor.lastMessage , "LevelUp") == 0) {
                strcpy(monitor.currentAction , "RECEIVED: Level Up!");
                monitor.messageTimer = SDL_GetTicks()+3000;
            }

            monitor.hasNewMessage = false;
        }

        SDL_SetRenderDrawColor(r , 44 , 62 , 80 , 255);
        SDL_RenderClear(r);

        drawGreenFlag(r, monitor);
        SDL_RenderPresent(r);

    }

   // SDL_RenderPresent(r);

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawGreenFlag(SDL_Renderer* renderer , ScratchMonitor & monitor) {
    boxRGBA(renderer , 20 , 20 , 80 , 50 , 0 , 200 , 0 , 255);

    stringRGBA(renderer , 30 ,30 ,"START"  , 255 ,255 , 255 , 255);

    rectangleRGBA(renderer , 10 , 60 , 790 , 550 , 200 , 200 , 200 , 255);
    if (SDL_GetTicks() < monitor.messageTimer) {
        boxRGBA(renderer ,  300 , 255 , 500 , 300 , 255 , 255 , 255 , 200);

        stringRGBA(renderer , 320 , 270 , monitor.currentAction , 0 , 0 , 0 , 255);
    }
}
