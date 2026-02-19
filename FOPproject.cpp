#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfx.h>
#include <ctime>


using namespace std;


struct sprite {

    float x , y ;
    float angle ;

    bool isVisible ;
};


void moveSteps (sprite & s , float steps) ;
void turnRight ( sprite & s , float degree);

void turnLeft ( sprite & s , float degree) ;

void setPosition (sprite & s , float newX, float newY) ;
void changeX (sprite &s , float amount);
void goToRandom (sprite & s , int W , int H);

void bounce0ffEdge (sprite & s , int w , int h);
void renderSprite (SDL_Renderer  * renderer , sprite & s );

int main(int argc , char *argv[]) {

 SDL_Init(SDL_INIT_VIDEO);

    int w =800 , h= 600 ;


SDL_Window* window = SDL_CreateWindow("scratch-motion" , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
SDL_Renderer* r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



sprite cat ;
    cat.x = w/2 ;
    cat.angle = 90;
    cat.y= h/2;
    cat.isVisible = true ;


    bool running = false ;
    SDL_Event e;

    srand(time(NULL));

    while (!running) {
        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                running =true ;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        moveSteps (cat , 10); break;
                        case SDLK_RIGHT:
                        turnRight (cat , 15); break;
                        case SDLK_LEFT:
                        turnLeft (cat , 15); break;
                    case SDLK_r :
                        goToRandom (cat , w , h);break;
                        case SDLK_SPACE:
                        setPosition (cat , w/2 , h/2); break;
                    case SDLK_h:
                        cat.isVisible = false ; break;

                    case SDLK_s:
                        cat.isVisible = true ; break;
                        case SDLK_ESCAPE:
                        running = true ; break;
                }
            }
        }


        bounce0ffEdge (cat , w , h);

        SDL_SetRenderDrawColor(r , 30 , 30 , 30 , 255 );
        SDL_RenderClear(r);

        if (cat.isVisible) {
            renderSprite(r , cat);

        }

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }







    SDL_RenderPresent(r);


    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    SDL_Quit();



    return 0;
}




void moveSteps (sprite & s , float steps) {
    float radian = (s.angle - 90 ) * (M_PI / 180.0);

    s.x += steps * cos(radian);
    s.y += steps * sin(radian);
}


void turnRight ( sprite & s , float degree) {

    s.angle += degree;
}

void turnLeft ( sprite & s , float degree) {
    s.angle -= degree;
}

void setPosition (sprite & s , float newX, float newY) {
    s.x = newX;
    s.y = newY;
}

void changeX (sprite &s , float amount) {
    s.x += amount;
}

void goToRandom (sprite & s , int W , int H) {
   // srand(time(NULL));

    s.x = rand() % W;
    s.y = rand() % H;
}

void bounce0ffEdge (sprite & s , int w , int h) {
    if (s.x <= 0 || s.x >= w) {
        s.angle = 360 - s.angle;
        s.x= (s.x <= 0) ? 0 : w-5;
    }
    if (s.y <= 0 || s.y >= h) {
        s.angle = 180 - s.angle;
        s.y = (s.y <= 0) ? 0 : h-5;
    }
}
void renderSprite (SDL_Renderer  * renderer , sprite & s ) {

    float rad = (s.angle - 90 ) * (M_PI / 180.0);


    short x1 = s.x + 20 * cos(rad);
    short y1 = s.y + 20 * sin(rad);
    short x2 = s.x +10 * cos(rad +2.5);
    short y2 = s.y +10 * sin(rad +2.5);
    short x3 = s.x + 10 * cos(rad -2.5);
    short y3 = s.y +10 * sin(rad -2.5);


    filledTrigonRGBA(renderer , x1 , y1 , x2 , y2 , x3 , y3 ,76 , 151 , 255 , 255   );
}