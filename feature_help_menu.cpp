#include <bits/stdc++.h>
#include <SDL2/SDL.h>

using namespace std;

//Debug(headers): 
const int DeBug_Off = 0;
const int Debug_On = 1;

const int Step_Off = 0;
const int Step_On = 1;

struct DebugManager {
    int DeBug_Mode;
    int Step_Mode;
    int Cycle_Counter;
    int WatchDog_Counter;
    int WatchDog_Limit;

};

//Debugging Functions:
void DeBugManager_Init(DebugManager* mx);
void DeBug_Enable(DebugManager* mx);
void DeBug_Disable(DebugManager* mx);
void WatchDog_Increment(DebugManager* mx);
void WatchDog_Reset(DebugManager* mx);

//Logger(headers):
const int Log_Info = 0;
const int Log_Warning = 1;
const int Log_Error = 2;

#define Max_Logs 100

struct LogEntery {
    int Cycle;
    int Line;
    int Level;
    char Messsage[200];

};
struct Logger {
    LogEntery Logs[Max_Logs];
    int Count;

};

//Logger Functions:
void Logger_Init(Logger* dLog);
void Log_Event(Logger* dLog, int Cycle, int Line, int Level, const char* Message);

//Renderer(headers/functions):
bool IsRunning = true;

void Input_Handeling(DebugManager* dm);
void Logic_Updates(DebugManager* dm, Logger* dLog);
void Renderer(SDL_Renderer* m_Renderer, Logger* dLog);

//A suggestion for desgining main part later..:
int main(int argc, char* argv[]) {
    // SDL_Init(SDL_INIT_VIDEO);

    // SDL_Window* m_window = SDL_CreateWindow("Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    // SDL_Renderer* m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    // DebugManager debugmanager;
    // Logger logger;

    // DeBugManager_Init(&debugmanager);
    // Logger_Init(&logger);

    // while (IsRunning) {
    //     Input_Handeling(&debugmanager);

    //     Logic_Updates(&debugmanager, &logger);

    //     Renderer(m_renderer, &logger);

    //     SDL_Delay(10);
    // }

    // SDL_DestroyRenderer(m_renderer);
    // SDL_DestroyWindow(m_window);
    // SDL_Quit();

    // return 0;
}

//Debugging Functions:
void DeBugManager_Init(DebugManager* mx) {
    mx->DeBug_Mode = DeBug_Off;
    mx->Step_Mode = Step_Off;
    mx->Cycle_Counter = 0;
    mx->WatchDog_Counter = 0;
    mx->WatchDog_Limit = 1000;

}
void DeBug_Enable(DebugManager* mx) {
    mx->DeBug_Mode = Debug_On;

}
void DeBug_Disable(DebugManager* mx) {
    mx->DeBug_Mode = DeBug_Off;

}
void WatchDog_Increment(DebugManager* mx) {
    mx->WatchDog_Counter++;

}
void WatchDog_Reset(DebugManager* mx) {
    mx->WatchDog_Counter = 0;

}

//Logger Functions:
void Logger_Init(Logger* dLog) {
    dLog->Count = 0;

}
void Log_Event(Logger* dLog, int Cycle, int Line, int Level, const char* Message) {
    if (dLog->Count == Max_Logs)
        return;

    LogEntery* Entery = &dLog->Logs[dLog->Count];

    Entery->Cycle = Cycle;
    Entery->Line = Line;
    Entery->Level = Level;

    strcpy(Entery->Messsage, Message);
    //strcpy: copies data from one C/C++-style string into the memory of another string!

    dLog->Count++;

}

//Renerer Functions:
void Input_Handeling(DebugManager* dm) {
    SDL_Event m_event;

    while (SDL_PollEvent(&m_event)) {
        if (m_event.type == SDL_QUIT) {
            IsRunning = false;
        }
        if (m_event.type == SDL_KEYDOWN) {
            if (m_event.key.keysym.sym == SDLK_F1) {
                DeBug_Enable(dm);
            }
            if (m_event.key.keysym.sym == SDLK_F2) {
                dm->Step_Mode = Step_On;
            }
            if (m_event.key.keysym.sym == SDLK_ESCAPE) {
                IsRunning = false;
            }
        }
    }
}
void Logic_Updates(DebugManager* dm, Logger* dLog) {
    dm->Cycle_Counter++;

    WatchDog_Increment(dm);

    if (dm->WatchDog_Counter > dm->WatchDog_Limit) {
        Log_Event(dLog, dm->Cycle_Counter, 0, Log_Error, "Oops, Infinite loop detected :(");

        IsRunning = false;
    }

    if (dm->DeBug_Mode == Debug_On && dm->Step_Mode == Step_On) {
        cout << "Press SPACE to continue..:)";

        bool Waiting = true;

        while (Waiting) {
            SDL_Event m_event;

            while (SDL_PollEvent(&m_event)) {
                if (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_SPACE) {
                    Waiting = false;
                }
            }
        }
    }
}
void Renderer(SDL_Renderer* m_Renderer, Logger* dLog) {
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_Renderer);
    SDL_RenderPresent(m_Renderer);

}
