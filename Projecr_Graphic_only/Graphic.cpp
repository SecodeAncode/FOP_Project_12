#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

enum BlockCategory {
    CAT_MOTION,
    CAT_LOOKS,
    CAT_SOUND,
    CAT_EVENTS,
    CAT_CONTROL,
    CAT_SENSING,
    CAT_OPERATORS,
    CAT_VARIABLES,
    CAT_MY_BLOCK,
    CAT_PEN

};

enum BlockType {
        //MOTION BLOCKS
        BLOCK_MOTION_MOVE_STEPS,
        BLOCK_MOTION_TURN_RIGHT,
        BLOCK_MOTION_TURN_LEFT,
        BLOCK_MOTION_GO_TO_XY,
        BLOCK_MOTION_CHANGR_X,
        BLOCK_MOTION_CHANGE_Y,
        BLOCK_MOTION_POINT_DIRECTION,
        BLOCK_MOTION_GO_TO,

        //LOOKS BLOCKS
        BLOCK_LOOKS_SAY,
        BLOCK_LOOKS_SAY_FOR_TIME,
        BLOCK_LOOKS_THINK,
        BLOCK_LOOKS_THINK_FOR_TIME,
        BLOCK_LOOKS_SWITCH_COSTUME,
        BLOCK_LOOKS_NEXT_COSTUME,
        BLOCK_LOOKS_SWITCH_BACKDROP,
        BLOCK_LOOKS_NEXT_BACKDROP,
        BLOCK_LOOKS_CHANGE_SIZE,
        BLOCK_LOOKS_SET_SIZE,
        BLOCK_LOOKS_CHANGE_EFFECT,
        BLOCK_LOOKS_SET_EFFECT,
        BLOCK_LOOKS_CLEAR_EFFECTS,
        BLOCK_LOOKS_SHOW,
        BLOCK_LOOKS_HIDE,
        BLOCK_LOOKS_GO_TO_FRONT_BACK,
        BLOCK_LOOKS_GO_FORWARD_BACKWARD,
        BLOCK_LOOKS_SIZE,
        BLOCK_LOOKS_COSTUME_NUMBER_NAME,
        BLOCK_LOOKS_BACKDROP_NUMBER_NAME,

        //SOUND BLOCKS
        BLOCK_SOUND_PLAY,
        BLOCK_SOUND_PLAY_UNTIL_DONE,
        BLOCK_SOUND_STOP_ALL,
        BLOCK_SOUND_SET_VOLUME,
        BLOCK_SOUND_CHANGE_VOLUME,
        BLOCK_SOUND_CLEAR_EFFECTS,

        //EVENT BLOCKS
        BLOCK_EVENT_FLAG_CLICKED,
        BLOCK_EVENT_KEY_PRESSED,
        // BLOCK_EVENT_SPRITE_CLICKED,
        BLOCK_EVENT_BROADCAST,
        BLOCK_EVENT_WHEN_RECIEVE,

        //CONTROL FLOW BLOCKS
        BLOCK_CONTROL_WAIT,
        BLOCK_CONTROL_REPEAT,
        BLOCK_CONTROL_FOREVER,
        BLOCK_CONTROL_IF,
        BLOCK_CONTROL_IF_ELSE,
        BLOCK_CONTROL_WAIT_UNTIL,
        REPEAT_UNTIL,
        BLOCK_CONTROL_STOP_ALL,

        //SENSING BLOCKS
        BLOCK_SENSING_TOUCHING_OBJECT,
        BLOCK_SENSING_TOUCHING_COLOR,
        BLOCK_SENSING_COLOR_TOUCHING,
        BLOCK_SENSING_DISTANCE_TO,
        BLOCK_SENSING_ASK_AND_WAIT,
        BLOCK_SENSING_ANSWER,
        BLOCK_SENSING_KEY_PRESSED,
        BLOCK_SENSING_MOUSE_DOWN,
        BLOCK_SENSING_MOUSE_X,
        BLOCK_SENSING_MOUSE_Y,
        BLOCK_SENSING_SET_DRAG_MODE,
        BLOCK_SENSING_TIMER,
        BLOCK_SENSING_RESET_TIMER,

        //OPERATOR BLOCKS
        BLOCK_OPERATOR_ADD,
        BLOCK_OPERATOR_SUBTRACT,
        BLOCK_OPERATOR_MULTIPLY,
        BLOCK_OPERATOR_DIVIDE,
        BLOCK_OPERATOR_GREATER_THAN,
        BLOCK_OPERATOR_LESS_THAN,
        BLOCK_OPERATOR_EQUAL,
        BLOCK_OPERATOR_AND,
        BLOCK_OPERATOR_OR,
        BLOCK_OPERATOR_NOT,
        BLOCK_OPERATOR_LENGTH_OF,
        BLOCK_OPERATOR_JOIN_STRING,
        BLOCK_OPERATOR_LETTER_OF,

        //VARIABLE BLOCKS
        BLOCK_VARIABLE_SET,
        BLOCK_VARIABLE_CHANGE,
        BLOCK_VARIABLE_VALUE,

        //COSTUME BLOCKS
        BLOCK_COSTUME_DEFINE,
        BLOCK_COSTUME_CALL,

        //PEN EXTENSIONS
        BLOCK_PEN_ERASE_ALL,
        BLOCK_PEN_STAMP,
        BLOCK_PEN_DOWN,
        BLOCK_PEN_UP,
        BLOCK_PEN_SET_COLOR_RGB,
        BLOCK_PEN_SET_COLOR_ATTR,
        BLOCK_PEN_CHANGE_COLOR_ATTR,
        BLOCK_PEN_SET_SIZE,
        BLOCK_PEN_CHANGE_SIZE

};

struct CategoryButton {
    BlockCategory category;
    SDL_Texture* texture;
    SDL_Rect rect;

};

struct MenuItem {
    BlockType type;             
    BlockCategory category;    
    SDL_Texture* texture;       
    SDL_Rect rect;              
    bool isHovered;          

    bool hasDropdown;        
    SDL_Rect dropdownRect;   
    bool isDropdownOpen;     
    vector<string> options;
    int selectedOptionIndex;;        
};

void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, int x, int y, SDL_Color color);
SDL_Texture* loadTex(const char* path, SDL_Renderer* renderer);
bool isMouseInside(int mouseX, int mouseY, SDL_Rect rect);
void setupMenu(SDL_Renderer* renderer, vector<CategoryButton>& catButtons, vector<MenuItem>& allBlocks);

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Scratch Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("arial.ttf", 16); 
    if(!font) { cout << "Font not found!\n"; }

    SDL_Color colorWhite = {255, 255, 255, 255};
    SDL_Color colorBlack = {0, 0, 0, 255};

    SDL_Texture* background = loadTex("background_notComplete_2.png", renderer);

    vector<CategoryButton> catButtons;
    vector<MenuItem> menuBlocks;

    setupMenu(renderer, catButtons, menuBlocks);

    BlockCategory activeCategory = CAT_MOTION;
    bool running = true;
    SDL_Event e;

    while (running) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                
                for (const auto& btn : catButtons) {
                    if (isMouseInside(mouseX, mouseY, btn.rect)) {
                        activeCategory = btn.category;
                        break;
                    }
                }

                for (auto& block : menuBlocks) {
                    if (block.category == activeCategory) {
                        
                        if (block.hasDropdown) {
                            SDL_Rect actualDarkBox = { block.rect.x + block.dropdownRect.x, block.rect.y + block.dropdownRect.y, block.dropdownRect.w, block.dropdownRect.h };
                            
                            if (block.isDropdownOpen) {
                                int menuY = actualDarkBox.y + actualDarkBox.h;
                                bool optionClicked = false;
                                for (size_t i = 0; i < block.options.size(); ++i) {
                                    SDL_Rect optRect = { actualDarkBox.x, menuY + (int)(i * 25), actualDarkBox.w, 25 };
                                    if (isMouseInside(mouseX, mouseY, optRect)) {
                                        block.selectedOptionIndex = i; 
                                        block.isDropdownOpen = false; 
                                        optionClicked = true;
                                        break;
                                    }
                                }
                                if (!optionClicked) block.isDropdownOpen = false;
                                break; 
                            } 
                            
                            else if (isMouseInside(mouseX, mouseY, actualDarkBox)) {
                                block.isDropdownOpen = true;
                                break;
                            }
                        }

                        
                        if (isMouseInside(mouseX, mouseY, block.rect) && !block.isDropdownOpen) {
                            cout << "Clicked block for Drag!\n";
                        }
                    }
                }
            }
        }

        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        for (const auto& btn : catButtons) {
            SDL_RenderCopy(renderer, btn.texture, NULL, &btn.rect);
        }

        for (const auto& block : menuBlocks) {
            if (block.category == activeCategory) {
                SDL_RenderCopy(renderer, block.texture, NULL, &block.rect);

            
                if (block.hasDropdown && font) {
                    int textX = block.rect.x + block.dropdownRect.x + 5;
                    int textY = block.rect.y + block.dropdownRect.y + 5;
                    
                    renderText(renderer, font, block.options[block.selectedOptionIndex], textX, textY, colorWhite);

                    if (block.isDropdownOpen) {
                        int boxX = block.rect.x + block.dropdownRect.x;
                        int boxY = block.rect.y + block.dropdownRect.y + block.dropdownRect.h;
                        
                        SDL_Rect menuBg = { boxX, boxY, block.dropdownRect.w, (int)block.options.size() * 25 };
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderFillRect(renderer, &menuBg);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer, &menuBg);

                        for (size_t i = 0; i < block.options.size(); ++i) {
                            renderText(renderer, font, block.options[i], boxX + 5, boxY + (i * 25) + 2, colorBlack);
                        }
                    }
                }
            }
        }

        SDL_RenderPresent(renderer);
    }


    SDL_DestroyTexture(background);
    for (auto& b : menuBlocks) SDL_DestroyTexture(b.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, int x, int y, SDL_Color color) {
    if (text.empty() || !font) return;
    SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect rect = { x, y, surf->w, surf->h };
    SDL_RenderCopy(renderer, tex, NULL, &rect);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}
SDL_Texture* loadTex(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surf = IMG_Load(path);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return tex;
}
bool isMouseInside(int mouseX, int mouseY, SDL_Rect rect) {
    return (mouseX >= rect.x && mouseX <= (rect.x + rect.w) &&
            mouseY >= rect.y && mouseY <= (rect.y + rect.h));
}
void setupMenu(SDL_Renderer* renderer, vector<CategoryButton>& catButtons, vector<MenuItem>& allBlocks) {
    
    catButtons.push_back({CAT_MOTION, loadTex("Motion_tab.png", renderer), { 20, 100, 70, 70 }});    
    catButtons.push_back({CAT_LOOKS, loadTex("looks_tab.png", renderer), { 20, 180, 60, 60 }});
    catButtons.push_back({CAT_SOUND, loadTex("sound_tab.png", renderer), { 20, 260, 60, 60 }});
    catButtons.push_back({CAT_EVENTS, loadTex("events_tab.png", renderer), { 20, 340, 60, 60 }});
    catButtons.push_back({CAT_CONTROL, loadTex("control_tab.png", renderer), { 20, 420, 60, 60 }});
    catButtons.push_back({CAT_SENSING, loadTex("sensing_tab.png", renderer), { 20, 500, 60, 60 }});
    catButtons.push_back({CAT_OPERATORS, loadTex("operators_tab.png", renderer), { 20, 580, 60, 60 }});
    // catButtons.push_back({CAT_VARIABLES, { 20, 660, 60, 60 }});
    // catButtons.push_back({CAT_MY_BLOCK,  { 20, 740, 60, 60 }});
    // catButtons.push_back({CAT_PEN,       { 20, 820, 60, 60 }});

    int startX = 120; 
    int startY = 80;
    int gap = 50;     
    int w = 150;     
    int h = 40;       

    //MOTION BLOCKS
    int y = startY;
    allBlocks.push_back({BLOCK_MOTION_MOVE_STEPS, CAT_MOTION, loadTex("Motion_1.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_MOTION_TURN_RIGHT, CAT_MOTION, loadTex("Motion_2.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_MOTION_TURN_LEFT, CAT_MOTION, loadTex("Motion_3.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_MOTION_GO_TO_XY, CAT_MOTION, loadTex("Motion_4.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_MOTION_CHANGR_X, CAT_MOTION, loadTex("Motion_5.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_MOTION_CHANGE_Y, CAT_MOTION, loadTex("Motion_6.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_MOTION_POINT_DIRECTION, CAT_MOTION, loadTex("Motion_7.png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem gotoRandBlock;
    gotoRandBlock.type = BLOCK_MOTION_GO_TO;
    gotoRandBlock.category = CAT_MOTION;
    gotoRandBlock.texture = loadTex("Motion_8.png", renderer);
    gotoRandBlock.rect = {startX, y, 180, 40};
    gotoRandBlock.isHovered = false;
    gotoRandBlock.hasDropdown = true;
    gotoRandBlock.dropdownRect = {70, 5, 80, 25}; 
    gotoRandBlock.isDropdownOpen = false;
    gotoRandBlock.options = {"random", "mouse"}; 
    gotoRandBlock.selectedOptionIndex = 0;       
    allBlocks.push_back(gotoRandBlock); 
    y += gap;

    //LOOKS BLOCKS
    y = startY; 
    allBlocks.push_back({BLOCK_LOOKS_SAY, CAT_LOOKS, loadTex("Looks_1.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_SAY_FOR_TIME, CAT_LOOKS, loadTex("Looks_3.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_THINK, CAT_LOOKS, loadTex("Looks_2.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_THINK_FOR_TIME, CAT_LOOKS, loadTex("Looks_4.png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem costumeBlock;
    costumeBlock.type = BLOCK_LOOKS_SWITCH_COSTUME;
    costumeBlock.category = CAT_LOOKS;
    costumeBlock.texture = loadTex("your_costume_file.png", renderer); 
    costumeBlock.rect = {startX, y, 200, 40}; 
    costumeBlock.isHovered = false;
    costumeBlock.hasDropdown = true;
    costumeBlock.dropdownRect = {100, 5, 90, 30}; 
    costumeBlock.isDropdownOpen = false;
    costumeBlock.options = {"costume1", "costume2"};
    costumeBlock.selectedOptionIndex = 0;
    allBlocks.push_back(costumeBlock);
    y += gap;
    allBlocks.push_back({BLOCK_LOOKS_NEXT_COSTUME, CAT_LOOKS, loadTex("Looks_6.png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem backdropBlock;
    backdropBlock.type = BLOCK_LOOKS_SWITCH_BACKDROP;
    backdropBlock.category = CAT_LOOKS;
    backdropBlock.texture = loadTex("your_backdrop_file.png", renderer);
    backdropBlock.rect = {startX, y, 220, 40}; 
    backdropBlock.isHovered = false;
    backdropBlock.hasDropdown = true;
    backdropBlock.dropdownRect = {120, 5, 90, 30};
    backdropBlock.isDropdownOpen = false;
    backdropBlock.options = {"backdrop1", "next backdrop"}; 
    backdropBlock.selectedOptionIndex = 0;
    allBlocks.push_back(backdropBlock);
    y += gap;
    allBlocks.push_back({BLOCK_LOOKS_NEXT_BACKDROP, CAT_LOOKS, loadTex("Looks_8.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_CHANGE_SIZE, CAT_LOOKS, loadTex("Looks_9.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_SET_SIZE, CAT_LOOKS, loadTex("Looks_10.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_CHANGE_EFFECT, CAT_LOOKS, loadTex("Looks_11.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_SET_EFFECT, CAT_LOOKS, loadTex("Looks_12.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_CLEAR_EFFECTS, CAT_LOOKS, loadTex("Looks_13.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_SHOW, CAT_LOOKS, loadTex("Looks_14.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_HIDE, CAT_LOOKS, loadTex("Looks_15 .png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_LOOKS_SIZE, CAT_LOOKS, loadTex("Looks_20 .png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem layerBlock;
    layerBlock.type = BLOCK_LOOKS_GO_FORWARD_BACKWARD;
    layerBlock.category = CAT_LOOKS;
    layerBlock.texture = loadTex("Looks_17.png", renderer);
    layerBlock.rect = {startX, y, 260, 40};
    layerBlock.isHovered = false;
    layerBlock.hasDropdown = true;
    layerBlock.dropdownRect = {60, 5, 80, 30};
    layerBlock.isDropdownOpen = false;
    layerBlock.options = {"forward", "backward"}; 
    layerBlock.selectedOptionIndex = 0;
    allBlocks.push_back(layerBlock);
    y += gap;
    MenuItem layerBlock1;
    layerBlock1.type = BLOCK_LOOKS_GO_TO_FRONT_BACK;
    layerBlock1.category = CAT_LOOKS;
    layerBlock1.texture = loadTex("Looks_16", renderer); // اسم فایل عکست
    layerBlock1.rect = {startX, y, 180, 40}; 
    layerBlock1.isHovered = false;
    layerBlock1.hasDropdown = true;
    layerBlock1.dropdownRect = {70, 5, 60, 25}; 
    layerBlock1.isDropdownOpen = false;
    layerBlock1.options = {"front", "back"}; 
    layerBlock1.selectedOptionIndex = 0;  
    allBlocks.push_back(layerBlock1); 
    y += gap;
    MenuItem costumeReporter;
    costumeReporter.type = BLOCK_LOOKS_COSTUME_NUMBER_NAME;
    costumeReporter.category = CAT_LOOKS;
    costumeReporter.texture = loadTex("Looks_18.png", renderer); 
    costumeReporter.rect = {startX, y, 150, 35}; 
    costumeReporter.isHovered = false;
    costumeReporter.hasDropdown = true;
    costumeReporter.dropdownRect = {80, 5, 60, 25};
    costumeReporter.isDropdownOpen = false;
    costumeReporter.options = {"number", "name"}; 
    costumeReporter.selectedOptionIndex = 0;
    allBlocks.push_back(costumeReporter); 
    y += gap;
    MenuItem backdropReporter;
    backdropReporter.type = BLOCK_LOOKS_BACKDROP_NUMBER_NAME;
    backdropReporter.category = CAT_LOOKS;
    backdropReporter.texture = loadTex("Looks_19.png", renderer);
    backdropReporter.rect = {startX, y, 150, 35}; 
    backdropReporter.isHovered = false;
    backdropReporter.hasDropdown = true;
    backdropReporter.dropdownRect = {85, 5, 60, 25};
    backdropReporter.isDropdownOpen = false;
    backdropReporter.options = {"number", "name"};
    backdropReporter.selectedOptionIndex = 0;
    allBlocks.push_back(backdropReporter); 
    y += gap;

    //SOUND BLOCKS
    y = startY;
    MenuItem startSoundBlock;
    startSoundBlock.type = BLOCK_SOUND_PLAY;
    startSoundBlock.category = CAT_SOUND;
    startSoundBlock.texture = loadTex("Suonds_2.png", renderer);
    startSoundBlock.rect = {startX, y, 170, 40}; 
    startSoundBlock.isHovered = false;
    startSoundBlock.hasDropdown = true;
    startSoundBlock.dropdownRect = {85, 5, 70, 30};
    startSoundBlock.isDropdownOpen = false;
    startSoundBlock.options = {"Meow", "record..."};
    startSoundBlock.selectedOptionIndex = 0;
    allBlocks.push_back(startSoundBlock);
    y += gap;
    MenuItem playUntilBlock;
    playUntilBlock.type = BLOCK_SOUND_PLAY_UNTIL_DONE;
    playUntilBlock.category = CAT_SOUND;
    playUntilBlock.texture = loadTex("Suonds_1.png", renderer); 
    playUntilBlock.rect = {startX, y, 220, 40}; 
    playUntilBlock.isHovered = false;
    playUntilBlock.hasDropdown = true;
    playUntilBlock.dropdownRect = {90, 5, 70, 30}; 
    playUntilBlock.isDropdownOpen = false;
    playUntilBlock.options = {"Meow", "record..."};
    playUntilBlock.selectedOptionIndex = 0;
    allBlocks.push_back(playUntilBlock);
    y += gap;    
    allBlocks.push_back({BLOCK_SOUND_STOP_ALL, CAT_SOUND, loadTex("Suonds_3.png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem setEffectBlock;
    setEffectBlock.type = BLOCK_SOUND_SET_VOLUME;
    setEffectBlock.category = CAT_SOUND;
    setEffectBlock.texture = loadTex("Suonds_5.png", renderer);
    setEffectBlock.rect = {startX, y, 200, 40}; 
    setEffectBlock.isHovered = false;
    setEffectBlock.hasDropdown = true;
    setEffectBlock.dropdownRect = {45, 5, 60, 30};
    setEffectBlock.isDropdownOpen = false;
    setEffectBlock.options = {"pitch", "pan left/right"};
    setEffectBlock.selectedOptionIndex = 0;
    allBlocks.push_back(setEffectBlock);
    y += gap;    
    MenuItem changeEffectBlock;
    changeEffectBlock.type = BLOCK_SOUND_CHANGE_VOLUME;
    changeEffectBlock.category = CAT_SOUND;
    changeEffectBlock.texture = loadTex("Suonds_4.png", renderer);
    changeEffectBlock.rect = {startX, y, 220, 40}; 
    changeEffectBlock.isHovered = false;
    changeEffectBlock.hasDropdown = true;
    changeEffectBlock.dropdownRect = {70, 5, 60, 30}; 
    changeEffectBlock.isDropdownOpen = false;
    changeEffectBlock.options = {"pitch", "pan left/right"};
    changeEffectBlock.selectedOptionIndex = 0;
    allBlocks.push_back(changeEffectBlock);
    y += gap;
    allBlocks.push_back({BLOCK_SOUND_CLEAR_EFFECTS, CAT_SOUND, loadTex("Suonds_6.png", renderer), {startX, y, w, h}, false}); y += gap;
    
    //EVENT BLOCKS
    y = startY;
    allBlocks.push_back({BLOCK_EVENT_FLAG_CLICKED, CAT_EVENTS, loadTex("Events_1.png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem keyPressBlock;
    keyPressBlock.type = BLOCK_EVENT_KEY_PRESSED;
    keyPressBlock.category = CAT_EVENTS;
    keyPressBlock.texture = loadTex("Events_2.png", renderer);
    keyPressBlock.rect = {startX, y, 240, 50}; 
    keyPressBlock.isHovered = false;
    keyPressBlock.hasDropdown = true;
    keyPressBlock.dropdownRect = {60, 15, 70, 25}; 
    keyPressBlock.isDropdownOpen = false;
    keyPressBlock.options = {"space", "up arrow", "down arrow", "any", "a"};
    keyPressBlock.selectedOptionIndex = 0;
    allBlocks.push_back(keyPressBlock);
    y += gap;    
    // allBlocks.push_back({BLOCK_EVENT_SPRITE_CLICKED, CAT_EVENTS, loadTex("blocks/ev_sprite.png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem broadcastBlock;
    broadcastBlock.type = BLOCK_EVENT_BROADCAST;
    broadcastBlock.category = CAT_EVENTS;
    broadcastBlock.texture = loadTex("Events_3.png", renderer);
    broadcastBlock.rect = {startX, y, 200, 40}; // Standard height 40
    broadcastBlock.isHovered = false;
    broadcastBlock.hasDropdown = true;
    broadcastBlock.dropdownRect = {85, 5, 100, 30}; 
    broadcastBlock.isDropdownOpen = false;
    broadcastBlock.options = {"message1", "new message..."};
    broadcastBlock.selectedOptionIndex = 0;
    allBlocks.push_back(broadcastBlock);
    y += gap;
    MenuItem receiveBlock;
    receiveBlock.type = BLOCK_EVENT_WHEN_RECIEVE;
    receiveBlock.category = CAT_EVENTS;
    receiveBlock.texture = loadTex("Events_4.png", renderer);
    receiveBlock.rect = {startX, y, 220, 50}; 
    receiveBlock.isHovered = false;
    receiveBlock.hasDropdown = true;
    receiveBlock.dropdownRect = {120, 15, 90, 25}; 
    receiveBlock.isDropdownOpen = false;
    receiveBlock.options = {"message1", "new message..."};
    receiveBlock.selectedOptionIndex = 0;
    allBlocks.push_back(receiveBlock);
    y += gap;

    //CONTROL BLOCKS
    y = startY;
    allBlocks.push_back({BLOCK_CONTROL_WAIT, CAT_CONTROL, loadTex("Control_1.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_CONTROL_REPEAT, CAT_CONTROL, loadTex("Control_2.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_CONTROL_FOREVER, CAT_CONTROL, loadTex("Control_3.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_CONTROL_IF, CAT_CONTROL, loadTex("Control_4.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_CONTROL_IF_ELSE, CAT_CONTROL, loadTex("Control_5.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_CONTROL_WAIT_UNTIL, CAT_CONTROL, loadTex("Control_6.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({REPEAT_UNTIL, CAT_CONTROL, loadTex("Control_8.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_CONTROL_STOP_ALL, CAT_CONTROL, loadTex("Control_7.png", renderer), {startX, y, w, h}, false});

    //SENSING BLOCKS
    y = startY;
    MenuItem touchingBlock;
    touchingBlock.type = BLOCK_SENSING_TOUCHING_OBJECT;
    touchingBlock.category = CAT_SENSING;
    touchingBlock.texture = loadTex("Sensing_2.png", renderer);
    touchingBlock.rect = {startX, y, 210, 40}; 
    touchingBlock.isHovered = false;
    touchingBlock.hasDropdown = true;
    touchingBlock.dropdownRect = {90, 5, 85, 30}; 
    touchingBlock.isDropdownOpen = false;
    touchingBlock.options = {"mouse-pointer", "edge"};
    touchingBlock.selectedOptionIndex = 0;
    allBlocks.push_back(touchingBlock);
    y += gap;    
    allBlocks.push_back({BLOCK_SENSING_TOUCHING_COLOR, CAT_SENSING, loadTex("Sensing_3.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_SENSING_COLOR_TOUCHING, CAT_SENSING, loadTex("Sensing_1.png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem distanceToBlock;
    distanceToBlock.type = BLOCK_SENSING_DISTANCE_TO;
    distanceToBlock.category = CAT_SENSING;
    distanceToBlock.texture = loadTex("Sensing_4.png", renderer); 
    distanceToBlock.rect = {startX, y, 190, 40}; 
    distanceToBlock.isHovered = false;
    distanceToBlock.hasDropdown = true;
    distanceToBlock.dropdownRect = {90, 5, 85, 30}; 
    distanceToBlock.isDropdownOpen = false;
    distanceToBlock.options = {"mouse-pointer"};
    distanceToBlock.selectedOptionIndex = 0;
    allBlocks.push_back(distanceToBlock);
    y += gap;    
    allBlocks.push_back({BLOCK_SENSING_ASK_AND_WAIT, CAT_SENSING, loadTex("Sensing_5.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_SENSING_ANSWER, CAT_SENSING, loadTex("Sensing_6.png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem keyPressedBlock;
    keyPressedBlock.type = BLOCK_SENSING_KEY_PRESSED;
    keyPressedBlock.category = CAT_SENSING;
    keyPressedBlock.texture = loadTex("Sensing_7.png", renderer);
    keyPressedBlock.rect = {startX, y, 220, 40}; 
    keyPressedBlock.isHovered = false;
    keyPressedBlock.hasDropdown = true;
    keyPressedBlock.dropdownRect = {50, 5, 80, 30}; 
    keyPressedBlock.isDropdownOpen = false;
    keyPressedBlock.options = {"space", "up arrow", "down arrow"};
    keyPressedBlock.selectedOptionIndex = 0;
    allBlocks.push_back(keyPressedBlock);
    y += gap;
    allBlocks.push_back({BLOCK_SENSING_MOUSE_DOWN, CAT_SENSING, loadTex("Sensing_8.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_SENSING_MOUSE_X, CAT_SENSING, loadTex("Sensing_9.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_SENSING_MOUSE_Y, CAT_SENSING, loadTex("Sensing_10.png", renderer), {startX, y, w, h}, false}); y += gap;
    MenuItem dragModeBlock;
    dragModeBlock.type = BLOCK_SENSING_SET_DRAG_MODE;
    dragModeBlock.category = CAT_SENSING;
    dragModeBlock.texture = loadTex("Sensing_11.png", renderer);
    dragModeBlock.rect = {startX, y, 240, 40}; 
    dragModeBlock.isHovered = false;
    dragModeBlock.hasDropdown = true;
    dragModeBlock.dropdownRect = {130, 5, 95, 30}; 
    dragModeBlock.isDropdownOpen = false;
    dragModeBlock.options = {"draggable", "not draggable"};
    dragModeBlock.selectedOptionIndex = 0;
    allBlocks.push_back(dragModeBlock);
    y += gap;
    allBlocks.push_back({BLOCK_SENSING_TIMER, CAT_SENSING, loadTex("Sensing_12.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_SENSING_RESET_TIMER, CAT_SENSING, loadTex("Sensing_13.png", renderer), {startX, y, w, h}, false});

    //OPERATOR BLOCKS
    y = startY;
    allBlocks.push_back({BLOCK_OPERATOR_ADD, CAT_OPERATORS, loadTex("Operators_plus.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_SUBTRACT, CAT_OPERATORS, loadTex("Operators_minus.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_MULTIPLY, CAT_OPERATORS, loadTex("Operators_multiply.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_DIVIDE, CAT_OPERATORS, loadTex("Operators_devision.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_GREATER_THAN, CAT_OPERATORS, loadTex("Operators_greater than.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_LESS_THAN, CAT_OPERATORS, loadTex("Operators_less than.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_EQUAL, CAT_OPERATORS, loadTex("Operators_equals.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_AND, CAT_OPERATORS, loadTex("Operators_and.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_OR, CAT_OPERATORS, loadTex("Operators_or.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_NOT, CAT_OPERATORS, loadTex("Operators_not.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_LENGTH_OF, CAT_OPERATORS, loadTex("Operators_LengthOfString.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_JOIN_STRING, CAT_OPERATORS, loadTex("Operators_joinToString.png", renderer), {startX, y, w, h}, false}); y += gap;
    allBlocks.push_back({BLOCK_OPERATOR_LETTER_OF, CAT_OPERATORS, loadTex("Operators_LetterOfString.png", renderer), {startX, y, w, h}, false});

    // //VARIABLE BLOCKS
    // y = startY;
    // allBlocks.push_back({BLOCK_VARIABLE_SET, CAT_VARIABLES, loadTex("blocks/var_set.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_VARIABLE_CHANGE, CAT_VARIABLES, loadTex("blocks/var_change.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_VARIABLE_VALUE, CAT_VARIABLES, loadTex("blocks/var_val.png", renderer), {startX, y, w, h}, false});

    // //COSTUME/MY BLOCKS
    // y = startY;
    // allBlocks.push_back({BLOCK_COSTUME_DEFINE, CAT_MY_BLOCK, loadTex("blocks/my_def.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_COSTUME_CALL, CAT_MY_BLOCK, loadTex("blocks/my_call.png", renderer), {startX, y, w, h}, false});

    // //PEN EXTENSION BLOCKS
    // y = startY;
    // allBlocks.push_back({BLOCK_PEN_ERASE_ALL, CAT_PEN, loadTex("blocks/pen_erase.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_PEN_STAMP, CAT_PEN, loadTex("blocks/pen_stamp.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_PEN_DOWN, CAT_PEN, loadTex("blocks/pen_down.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_PEN_UP, CAT_PEN, loadTex("blocks/pen_up.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_PEN_SET_COLOR_RGB, CAT_PEN, loadTex("blocks/pen_setrgb.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_PEN_SET_COLOR_ATTR, CAT_PEN, loadTex("blocks/pen_setattr.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_PEN_CHANGE_COLOR_ATTR, CAT_PEN, loadTex("blocks/pen_chattr.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_PEN_SET_SIZE, CAT_PEN, loadTex("blocks/pen_setsize.png", renderer), {startX, y, w, h}, false}); y += gap;
    // allBlocks.push_back({BLOCK_PEN_CHANGE_SIZE, CAT_PEN, loadTex("blocks/pen_chsize.png", renderer), {startX, y, w, h}, false});
}