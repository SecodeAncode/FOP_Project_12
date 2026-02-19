#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfx.h>
#include <cmath>

using namespace std;


bool  ispenExtensionAdded=false;

bool showErrorMessage=false;

Uint32 errorTime=0;

const int ERROR_DURATION=2000;

SDL_Rect penIcon{20 , 530 , 60 , 60};

struct pen {
    bool isDown = false;
    Uint8 red=60 , green = 250 , blue =0 ;


    int brightness=100;

    int saturation=100;

    int size =2;

    Uint32 currentColor;

};

pen myPen ;


SDL_Texture* spriteTexture = nullptr;
SDL_Rect spriteRect ={375 , 275 ,50 , 50};

void eraseAll (SDL_Renderer* renderer , SDL_Texture* canvas) ;

void drawLine (SDL_Renderer* renderer  ,  SDL_Texture* canvas , int x1 , int y1 , int x2 , int y2) ;


void stamp (SDL_Renderer* renderer , SDL_Texture* texture , SDL_Rect* pos, SDL_Texture* texture2) ;


void updateColor ( );

void checkExtension (int mousex , int mousey) ;



void renderErrorMessage(SDL_Renderer* renderer);


void triggerExtensionError();

int main(int argc , char *argv[]) {



        SDL_Init(SDL_INIT_VIDEO);

        SDL_Window* window = SDL_CreateWindow("my_window" , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_SHOWN  );

        SDL_Renderer* r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


        SDL_Texture* canvas = SDL_CreateTexture(r , SDL_PIXELFORMAT_RGBA8888 , SDL_TEXTUREACCESS_TARGET, 800, 600);



    SDL_SetRenderTarget(r,canvas);
    SDL_SetRenderDrawColor(r,255,255,255,255);
    SDL_RenderClear(r);
    SDL_SetRenderTarget(r,NULL);




    bool quit = false;
    SDL_Event event;

    updateColor();


    while(!quit) {

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                quit = true;


            if(event.type == SDL_MOUSEBUTTONDOWN) {
                if(event.button.button == SDL_BUTTON_LEFT) {
                    checkExtension(event.button.x , event.button.y);
                }
            }

            if (event.type==SDL_KEYDOWN ) {
                int oldx=spriteRect.x + spriteRect.w/2;
                int oldy=spriteRect.y + spriteRect.h/2;

                bool isMovement=false;

                if(event.key.keysym.sym == SDLK_UP) {
                    spriteRect.y -= 10;
                    isMovement=true;
                }
                else if (event.key.keysym.sym ==SDLK_DOWN) {
                    spriteRect.y += 10;
                    isMovement=true;
                }
                else if ( event.key.keysym.sym ==SDLK_LEFT) {
                    spriteRect.x -= 10;
                    isMovement=true;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    spriteRect.x += 10;
                    isMovement=true;
                }
                switch (event.key.keysym.sym) {
                    case SDLK_d:  case SDLK_s:  case SDLK_e:   case SDLK_EQUALS:  case SDLK_MINUS:   case SDLK_b:




                        if ( ispenExtensionAdded){
                            if (event.key.keysym.sym== SDLK_d)
                                myPen.isDown = ! myPen.isDown;
                            if (event.key.keysym.sym== SDLK_s)
                                stamp( r , canvas , &spriteRect , spriteTexture );

                            if (event.key.keysym.sym== SDLK_e)
                                eraseAll(r , canvas);

                            if (event.key.keysym.sym== SDLK_EQUALS)
                                myPen.size++;
                            if (event.key.keysym.sym== SDLK_MINUS&& (myPen.size > 1) )
                                myPen.size--;

                            if (event.key.keysym.sym== SDLK_b) {
                                myPen.brightness = (myPen.brightness <=10) ? 100 : myPen.brightness -10;
                                updateColor();
                            }

                        }
                        else {
                            triggerExtensionError();
                        }

                        break;
                }

                if (isMovement && ispenExtensionAdded ){

                    int newx=spriteRect.x + spriteRect.w/2;
                    int newy=spriteRect.y + spriteRect.h/2;
                    drawLine(r , canvas , oldx , oldy, newx, newy);
                }
            }
        }


        SDL_SetRenderDrawColor(r,200,200,200,255);

        SDL_RenderClear(r);
        SDL_RenderCopy(r, canvas, NULL , NULL );

        if (! ispenExtensionAdded) {

            SDL_SetRenderDrawColor(r,0,100,255,200);
            SDL_RenderFillRect(r , &penIcon);

        }
        SDL_RenderCopy(r, spriteTexture , NULL , &spriteRect );


        renderErrorMessage(r);
        SDL_RenderPresent(r);
    }


        SDL_RenderPresent(r);
     //   SDL_Delay(6000);


    SDL_DestroyTexture(canvas);
    SDL_DestroyTexture(spriteTexture);
   // SDL_DestroyTexture(canvas);
        SDL_DestroyRenderer(r);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }



void eraseAll (SDL_Renderer* renderer , SDL_Texture* canvas) {

    SDL_SetRenderTarget(renderer, canvas);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
  //SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, NULL);
   // SDL_RenderPresent(renderer);
}

void drawLine (SDL_Renderer* renderer , SDL_Texture* canvas , int x1 , int y1 , int x2 , int y2) {

    if (myPen.isDown) {

        SDL_SetRenderTarget(renderer, canvas);
        thickLineColor(renderer , x1 , y1 , x2 , y2 , myPen.size , myPen.currentColor);

        SDL_SetRenderTarget(renderer, NULL);
       // SDL_RenderPresent(renderer);
    }
}

void stamp (SDL_Renderer* renderer , SDL_Texture* texture , SDL_Rect* pos, SDL_Texture* texture2) {

    SDL_SetRenderTarget( renderer , texture);
    SDL_RenderCopy( renderer , texture2 , NULL , pos );

    SDL_SetRenderTarget(renderer, NULL);
}


void updateColor ( ) {

    float s = myPen.saturation/100.0f;
    float v = myPen.brightness/100.0f;


    Uint8 finalR = (Uint8)(myPen.red * s * v);
    Uint8 finalG = (Uint8)(myPen.green * s * v);
    Uint8 finalB = (Uint8)(myPen.blue * s * v);

    myPen.currentColor = (finalR << 24) | (finalG << 16) | (finalB << 8) | 0xFF;
}


void checkExtension (int mousex , int mousey) {
    SDL_Point mousepos ={mousex,mousey};

    if (SDL_PointInRect(&mousepos, &penIcon)) {
        if (!ispenExtensionAdded) {
            ispenExtensionAdded = true;

            //cout << "Pen Extension Activated"<<endl;

            updateColor();
        }
    }
}


void renderErrorMessage(SDL_Renderer* renderer) {

    if (showErrorMessage) {
        if (SDL_GetTicks()- errorTime > ERROR_DURATION ) {
            showErrorMessage = false;
        }
        else {

            SDL_Rect errorRect = {200 , 540 , 400 , 50};
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
            SDL_RenderFillRect(renderer, &errorRect);
        }
    }
}

void triggerExtensionError() {
    if (!ispenExtensionAdded) {
        showErrorMessage = true;
        errorTime = SDL_GetTicks();

        cout<<"Error: You must add the pen Etension first!"<<endl;
    }
}