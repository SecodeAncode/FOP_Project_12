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
    bool isSelected;
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
    int selectedOptionIndex;        
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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    SDL_RenderSetLogicalSize(renderer, 1920, 1080);

    TTF_Font* font = TTF_OpenFont("ARIAL.ttf", 16); 
    if(!font) { cout << "Font not found!\n"; }

    SDL_Color colorWhite = {255, 255, 255, 255};
    SDL_Color colorBlack = {0, 0, 0, 255};

    SDL_Texture* background = loadTex("background_notComplete_2.png", renderer);

    vector<CategoryButton> catButtons;
    vector<MenuItem> menuBlocks;

    setupMenu(renderer, catButtons, menuBlocks);

    BlockCategory activeCategory = CAT_MOTION;
    const MenuItem* selectedBlock = nullptr;
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
                
                for (auto& btn : catButtons) {
                    btn.isSelected = (btn.category == activeCategory);
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
                            selectedBlock = &block;
                        }
                    }
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        for (const auto& btn : catButtons) {
            if (btn.category == activeCategory) {
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 120);   
                SDL_RenderFillRect(renderer, &btn.rect);
            }
            SDL_RenderCopy(renderer, btn.texture, NULL, &btn.rect);
        }

        for (const auto& block : menuBlocks) {
            if (block.category == activeCategory) {
                if (selectedBlock == &block) {
                    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 120); 
                    SDL_RenderFillRect(renderer, &block.rect);
                }

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
    int btnX = 16;  
    int btnY = 160;  
    int btnW = 55;   
    int btnGap = 85; 

    auto addCategory = [&](BlockCategory cat, const char* path, int index) {
        SDL_Texture* tex = loadTex(path, renderer);
        int originalW, originalH;
        SDL_QueryTexture(tex, NULL, NULL, &originalW, &originalH);
        int btnH = (originalH * btnW) / originalW; 
        catButtons.push_back({cat, tex, {btnX, btnY + (index * btnGap), btnW, btnH}, false});
    };
    
    addCategory(CAT_MOTION, "Motion_tab.png", 0);
    addCategory(CAT_LOOKS, "looks_tab.png", 1);
    addCategory(CAT_SOUND, "sound_tab.png", 2);
    addCategory(CAT_EVENTS, "events_tab.png", 3);
    addCategory(CAT_CONTROL, "control_tab.png", 4);
    addCategory(CAT_SENSING, "sensing_tab.png", 5);
    
    btnX = 8;
    btnW = 80;
    addCategory(CAT_OPERATORS, "operators_tab.png", 6);

    int startX = 120; 
    int startY = 80;
    int currentY = startY; 
    int blockGap = 15;       
    float blockScale = 0.6f; 

    auto addBlock = [&](BlockType type, BlockCategory cat, const char* path) {
        SDL_Texture* tex = loadTex(path, renderer);
        int origW, origH;
        SDL_QueryTexture(tex, NULL, NULL, &origW, &origH); 
        int destW = (int)(origW * blockScale);
        int destH = (int)(origH * blockScale);
        SDL_Rect blockRect = {startX, currentY, destW, destH};
        SDL_Rect emptyDropRect = {0, 0, 0, 0};
        allBlocks.push_back({
            type, cat, tex, blockRect, false, false, emptyDropRect, false, {}, 0                  
        });
        currentY += destH + blockGap; 
    };

    //MOTION BLOCKS
    addBlock(BLOCK_MOTION_MOVE_STEPS, CAT_MOTION, "Motion_1.png");
    addBlock(BLOCK_MOTION_TURN_RIGHT, CAT_MOTION, "Motion_2.png");
    addBlock(BLOCK_MOTION_TURN_LEFT, CAT_MOTION, "Motion_3.png");
    addBlock(BLOCK_MOTION_GO_TO_XY, CAT_MOTION, "Motion_4.png");
    addBlock(BLOCK_MOTION_CHANGR_X, CAT_MOTION, "Motion_5.png");
    addBlock(BLOCK_MOTION_CHANGE_Y, CAT_MOTION, "Motion_6.png");
    addBlock(BLOCK_MOTION_POINT_DIRECTION, CAT_MOTION, "Motion_7.png");     
    
    {
        SDL_Texture* texRand = loadTex("Motion_8.png", renderer);
        int origW, origH;
        SDL_QueryTexture(texRand, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale);
        int destH = (int)(origH * blockScale);
        MenuItem gotoRandBlock;
        gotoRandBlock.type = BLOCK_MOTION_GO_TO;
        gotoRandBlock.category = CAT_MOTION;
        gotoRandBlock.texture = texRand;
        gotoRandBlock.rect = {startX, currentY, destW, destH};
        gotoRandBlock.isHovered = false;
        gotoRandBlock.hasDropdown = true;
        gotoRandBlock.dropdownRect = {70, 5, 80, 25}; 
        gotoRandBlock.isDropdownOpen = false;
        gotoRandBlock.options = {"random", "mouse"}; 
        gotoRandBlock.selectedOptionIndex = 0;       
        allBlocks.push_back(gotoRandBlock); 
        currentY += destH + blockGap;
    }

    //LOOKS BLOCKS
    currentY = startY;
    addBlock(BLOCK_LOOKS_SAY, CAT_LOOKS, "Looks_1.png");
    addBlock(BLOCK_LOOKS_SAY_FOR_TIME, CAT_LOOKS, "Looks_3.png");
    addBlock(BLOCK_LOOKS_THINK, CAT_LOOKS, "Looks_2.png");
    addBlock(BLOCK_LOOKS_THINK_FOR_TIME, CAT_LOOKS, "Looks_4.png");

    {   
        SDL_Texture* tex = loadTex("your_costume_file.png", renderer);
        int origW, origH;
        SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale);
        int destH = (int)(origH * blockScale);
        MenuItem costumeBlock;
        costumeBlock.type = BLOCK_LOOKS_SWITCH_COSTUME;
        costumeBlock.category = CAT_LOOKS;
        costumeBlock.texture = tex; 
        costumeBlock.rect = {startX, currentY, destW, destH}; 
        costumeBlock.isHovered = false;
        costumeBlock.hasDropdown = true;
        costumeBlock.dropdownRect = {100, 5, 90, 30}; 
        costumeBlock.isDropdownOpen = false;
        costumeBlock.options = {"costume1", "costume2"};
        costumeBlock.selectedOptionIndex = 0;
        allBlocks.push_back(costumeBlock);
        currentY += destH + blockGap;
    }

    addBlock(BLOCK_LOOKS_NEXT_COSTUME, CAT_LOOKS, "Looks_6.png");

    {
        SDL_Texture* tex = loadTex("your_backdrop_file.png", renderer);
        int origW, origH;
        SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale);
        int destH = (int)(origH * blockScale);
        MenuItem backdropBlock;
        backdropBlock.type = BLOCK_LOOKS_SWITCH_BACKDROP;
        backdropBlock.category = CAT_LOOKS;
        backdropBlock.texture = tex;
        backdropBlock.rect = {startX, currentY, destW, destH}; 
        backdropBlock.isHovered = false;
        backdropBlock.hasDropdown = true;
        backdropBlock.dropdownRect = {120, 5, 90, 30};
        backdropBlock.isDropdownOpen = false;
        backdropBlock.options = {"backdrop1", "next backdrop"}; 
        backdropBlock.selectedOptionIndex = 0;
        allBlocks.push_back(backdropBlock);
        currentY += destH + blockGap;
    }

    addBlock(BLOCK_LOOKS_NEXT_BACKDROP, CAT_LOOKS, "Looks_8.png");
    addBlock(BLOCK_LOOKS_CHANGE_SIZE, CAT_LOOKS, "Looks_9.png");
    addBlock(BLOCK_LOOKS_SET_SIZE, CAT_LOOKS, "Looks_10.png");
    addBlock(BLOCK_LOOKS_CHANGE_EFFECT, CAT_LOOKS, "Looks_11.png");
    addBlock(BLOCK_LOOKS_SET_EFFECT, CAT_LOOKS, "Looks_12.png");
    addBlock(BLOCK_LOOKS_CLEAR_EFFECTS, CAT_LOOKS, "Looks_13.png");
    addBlock(BLOCK_LOOKS_SHOW, CAT_LOOKS, "Looks_14.png");
    addBlock(BLOCK_LOOKS_HIDE, CAT_LOOKS, "Looks_15 .png");
    addBlock(BLOCK_LOOKS_SIZE, CAT_LOOKS, "Looks_20 .png");

    {
        SDL_Texture* tex = loadTex("Looks_17.png", renderer);
        int origW, origH;
        SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale);
        int destH = (int)(origH * blockScale);
        MenuItem layerBlock;
        layerBlock.type = BLOCK_LOOKS_GO_FORWARD_BACKWARD;
        layerBlock.category = CAT_LOOKS;
        layerBlock.texture = tex;
        layerBlock.rect = {startX, currentY, destW, destH};
        layerBlock.isHovered = false;
        layerBlock.hasDropdown = true;
        layerBlock.dropdownRect = {60, 5, 80, 30};
        layerBlock.isDropdownOpen = false;
        layerBlock.options = {"forward", "backward"}; 
        layerBlock.selectedOptionIndex = 0;
        allBlocks.push_back(layerBlock);
        currentY += destH + blockGap;
    }

    {
        SDL_Texture* tex = loadTex("Looks_16", renderer);
        int origW, origH;
        SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale);
        int destH = (int)(origH * blockScale);
        MenuItem layerBlock1;
        layerBlock1.type = BLOCK_LOOKS_GO_TO_FRONT_BACK;
        layerBlock1.category = CAT_LOOKS;
        layerBlock1.texture = tex; 
        layerBlock1.rect = {startX, currentY, destW, destH}; 
        layerBlock1.isHovered = false;
        layerBlock1.hasDropdown = true;
        layerBlock1.dropdownRect = {70, 5, 60, 25}; 
        layerBlock1.isDropdownOpen = false;
        layerBlock1.options = {"front", "back"}; 
        layerBlock1.selectedOptionIndex = 0;  
        allBlocks.push_back(layerBlock1); 
        currentY += destH + blockGap;
    }

    {
        SDL_Texture* tex = loadTex("Looks_18.png", renderer);
        int origW, origH;
        SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale);
        int destH = (int)(origH * blockScale);
        MenuItem costumeReporter;
        costumeReporter.type = BLOCK_LOOKS_COSTUME_NUMBER_NAME;
        costumeReporter.category = CAT_LOOKS;
        costumeReporter.texture = tex; 
        costumeReporter.rect = {startX, currentY, destW, destH}; 
        costumeReporter.isHovered = false;
        costumeReporter.hasDropdown = true;
        costumeReporter.dropdownRect = {80, 5, 60, 25};
        costumeReporter.isDropdownOpen = false;
        costumeReporter.options = {"number", "name"}; 
        costumeReporter.selectedOptionIndex = 0;
        allBlocks.push_back(costumeReporter); 
        currentY += destH + blockGap;
    }

    {
        SDL_Texture* tex = loadTex("Looks_19.png", renderer);
        int origW, origH;
        SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale);
        int destH = (int)(origH * blockScale);
        MenuItem backdropReporter;
        backdropReporter.type = BLOCK_LOOKS_BACKDROP_NUMBER_NAME;
        backdropReporter.category = CAT_LOOKS;
        backdropReporter.texture = tex;
        backdropReporter.rect = {startX, currentY, destW, destH}; 
        backdropReporter.isHovered = false;
        backdropReporter.hasDropdown = true;
        backdropReporter.dropdownRect = {85, 5, 60, 25};
        backdropReporter.isDropdownOpen = false;
        backdropReporter.options = {"number", "name"};
        backdropReporter.selectedOptionIndex = 0;
        allBlocks.push_back(backdropReporter); 
        currentY += destH + blockGap;
    }

    //SOUND BLOCKS
    currentY = startY;
    {
        SDL_Texture* tex = loadTex("Suonds_2.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem startSoundBlock;
        startSoundBlock.type = BLOCK_SOUND_PLAY;
        startSoundBlock.category = CAT_SOUND;
        startSoundBlock.texture = tex;
        startSoundBlock.rect = {startX, currentY, destW, destH}; 
        startSoundBlock.isHovered = false;
        startSoundBlock.hasDropdown = true;
        startSoundBlock.dropdownRect = {85, 5, 70, 30};
        startSoundBlock.isDropdownOpen = false;
        startSoundBlock.options = {"Meow", "record..."};
        startSoundBlock.selectedOptionIndex = 0;
        allBlocks.push_back(startSoundBlock);
        currentY += destH + blockGap;
    }

    {
        SDL_Texture* tex = loadTex("Suonds_1.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem playUntilBlock;
        playUntilBlock.type = BLOCK_SOUND_PLAY_UNTIL_DONE;
        playUntilBlock.category = CAT_SOUND;
        playUntilBlock.texture = tex; 
        playUntilBlock.rect = {startX, currentY, destW, destH}; 
        playUntilBlock.isHovered = false;
        playUntilBlock.hasDropdown = true;
        playUntilBlock.dropdownRect = {90, 5, 70, 30}; 
        playUntilBlock.isDropdownOpen = false;
        playUntilBlock.options = {"Meow", "record..."};
        playUntilBlock.selectedOptionIndex = 0;
        allBlocks.push_back(playUntilBlock);
        currentY += destH + blockGap;
    }
    
    addBlock(BLOCK_SOUND_STOP_ALL, CAT_SOUND, "Suonds_3.png");

    {
        SDL_Texture* tex = loadTex("Suonds_5.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem setEffectBlock;
        setEffectBlock.type = BLOCK_SOUND_SET_VOLUME;
        setEffectBlock.category = CAT_SOUND;
        setEffectBlock.texture = tex;
        setEffectBlock.rect = {startX, currentY, destW, destH}; 
        setEffectBlock.isHovered = false;
        setEffectBlock.hasDropdown = true;
        setEffectBlock.dropdownRect = {45, 5, 60, 30};
        setEffectBlock.isDropdownOpen = false;
        setEffectBlock.options = {"pitch", "pan left/right"};
        setEffectBlock.selectedOptionIndex = 0;
        allBlocks.push_back(setEffectBlock);
        currentY += destH + blockGap;
    }

    {
        SDL_Texture* tex = loadTex("Suonds_4.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem changeEffectBlock;
        changeEffectBlock.type = BLOCK_SOUND_CHANGE_VOLUME;
        changeEffectBlock.category = CAT_SOUND;
        changeEffectBlock.texture = tex;
        changeEffectBlock.rect = {startX, currentY, destW, destH}; 
        changeEffectBlock.isHovered = false;
        changeEffectBlock.hasDropdown = true;
        changeEffectBlock.dropdownRect = {70, 5, 60, 30}; 
        changeEffectBlock.isDropdownOpen = false;
        changeEffectBlock.options = {"pitch", "pan left/right"};
        changeEffectBlock.selectedOptionIndex = 0;
        allBlocks.push_back(changeEffectBlock);
        currentY += destH + blockGap;
    }

    addBlock(BLOCK_SOUND_CLEAR_EFFECTS, CAT_SOUND, "Suonds_6.png");
    
    //EVENT BLOCKS
    currentY = startY;
    addBlock(BLOCK_EVENT_FLAG_CLICKED, CAT_EVENTS, "Events_1.png");

    {
        SDL_Texture* tex = loadTex("Events_2.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem keyPressBlock;
        keyPressBlock.type = BLOCK_EVENT_KEY_PRESSED;
        keyPressBlock.category = CAT_EVENTS;
        keyPressBlock.texture = tex;
        keyPressBlock.rect = {startX, currentY, destW, destH}; 
        keyPressBlock.isHovered = false;
        keyPressBlock.hasDropdown = true;
        keyPressBlock.dropdownRect = {60, 15, 70, 25}; 
        keyPressBlock.isDropdownOpen = false;
        keyPressBlock.options = {"space", "up arrow", "down arrow", "any", "a"};
        keyPressBlock.selectedOptionIndex = 0;
        allBlocks.push_back(keyPressBlock);
        currentY += destH + blockGap;
    }

    {
        SDL_Texture* tex = loadTex("Events_3.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem broadcastBlock;
        broadcastBlock.type = BLOCK_EVENT_BROADCAST;
        broadcastBlock.category = CAT_EVENTS;
        broadcastBlock.texture = tex;
        broadcastBlock.rect = {startX, currentY, destW, destH}; 
        broadcastBlock.isHovered = false;
        broadcastBlock.hasDropdown = true;
        broadcastBlock.dropdownRect = {85, 5, 100, 30}; 
        broadcastBlock.isDropdownOpen = false;
        broadcastBlock.options = {"message1", "new message..."};
        broadcastBlock.selectedOptionIndex = 0;
        allBlocks.push_back(broadcastBlock);
        currentY += destH + blockGap;
    }

    {
        SDL_Texture* tex = loadTex("Events_4.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem receiveBlock;
        receiveBlock.type = BLOCK_EVENT_WHEN_RECIEVE;
        receiveBlock.category = CAT_EVENTS;
        receiveBlock.texture = tex;
        receiveBlock.rect = {startX, currentY, destW, destH}; 
        receiveBlock.isHovered = false;
        receiveBlock.hasDropdown = true;
        receiveBlock.dropdownRect = {120, 15, 90, 25}; 
        receiveBlock.isDropdownOpen = false;
        receiveBlock.options = {"message1", "new message..."};
        receiveBlock.selectedOptionIndex = 0;
        allBlocks.push_back(receiveBlock);
        currentY += destH + blockGap;
    }

    //CONTROL BLOCKS
    currentY = startY;
    addBlock(BLOCK_CONTROL_WAIT, CAT_CONTROL, "Control_1.png");
    addBlock(BLOCK_CONTROL_REPEAT, CAT_CONTROL, "Control_2.png");
    addBlock(BLOCK_CONTROL_FOREVER, CAT_CONTROL, "Control_3.png");
    addBlock(BLOCK_CONTROL_IF, CAT_CONTROL, "Control_4.png");
    addBlock(BLOCK_CONTROL_IF_ELSE, CAT_CONTROL, "Control_5.png");
    addBlock(BLOCK_CONTROL_WAIT_UNTIL, CAT_CONTROL, "Control_6.png");
    addBlock(REPEAT_UNTIL, CAT_CONTROL, "Control_8.png");
    addBlock(BLOCK_CONTROL_STOP_ALL, CAT_CONTROL, "Control_7.png");

    //SENSING BLOCKS
    currentY = startY;
    {
        SDL_Texture* tex = loadTex("Sensing_2.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem touchingBlock;
        touchingBlock.type = BLOCK_SENSING_TOUCHING_OBJECT;
        touchingBlock.category = CAT_SENSING;
        touchingBlock.texture = tex;
        touchingBlock.rect = {startX, currentY, destW, destH}; 
        touchingBlock.isHovered = false;
        touchingBlock.hasDropdown = true;
        touchingBlock.dropdownRect = {90, 5, 85, 30}; 
        touchingBlock.isDropdownOpen = false;
        touchingBlock.options = {"mouse-pointer", "edge"};
        touchingBlock.selectedOptionIndex = 0;
        allBlocks.push_back(touchingBlock);
        currentY += destH + blockGap;
    }

    addBlock(BLOCK_SENSING_TOUCHING_COLOR, CAT_SENSING, "Sensing_3.png");
    addBlock(BLOCK_SENSING_COLOR_TOUCHING, CAT_SENSING, "Sensing_1.png");

    {
        SDL_Texture* tex = loadTex("Sensing_4.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem distanceToBlock;
        distanceToBlock.type = BLOCK_SENSING_DISTANCE_TO;
        distanceToBlock.category = CAT_SENSING;
        distanceToBlock.texture = tex; 
        distanceToBlock.rect = {startX, currentY, destW, destH}; 
        distanceToBlock.isHovered = false;
        distanceToBlock.hasDropdown = true;
        distanceToBlock.dropdownRect = {90, 5, 85, 30}; 
        distanceToBlock.isDropdownOpen = false;
        distanceToBlock.options = {"mouse-pointer"};
        distanceToBlock.selectedOptionIndex = 0;
        allBlocks.push_back(distanceToBlock);
        currentY += destH + blockGap;
    }

    addBlock(BLOCK_SENSING_ASK_AND_WAIT, CAT_SENSING, "Sensing_5.png");
    addBlock(BLOCK_SENSING_ANSWER, CAT_SENSING, "Sensing_6.png");

    {
        SDL_Texture* tex = loadTex("Sensing_7.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem keyPressedBlock;
        keyPressedBlock.type = BLOCK_SENSING_KEY_PRESSED;
        keyPressedBlock.category = CAT_SENSING;
        keyPressedBlock.texture = tex;
        keyPressedBlock.rect = {startX, currentY, destW, destH}; 
        keyPressedBlock.isHovered = false;
        keyPressedBlock.hasDropdown = true;
        keyPressedBlock.dropdownRect = {50, 5, 80, 30}; 
        keyPressedBlock.isDropdownOpen = false;
        keyPressedBlock.options = {"space", "up arrow", "down arrow"};
        keyPressedBlock.selectedOptionIndex = 0;
        allBlocks.push_back(keyPressedBlock);
        currentY += destH + blockGap;
    }

    addBlock(BLOCK_SENSING_MOUSE_DOWN, CAT_SENSING, "Sensing_8.png");
    addBlock(BLOCK_SENSING_MOUSE_X, CAT_SENSING, "Sensing_9.png");
    addBlock(BLOCK_SENSING_MOUSE_Y, CAT_SENSING, "Sensing_10.png");

    {
        SDL_Texture* tex = loadTex("Sensing_11.png", renderer);
        int origW, origH; SDL_QueryTexture(tex, NULL, NULL, &origW, &origH);
        int destW = (int)(origW * blockScale), destH = (int)(origH * blockScale);
        MenuItem dragModeBlock;
        dragModeBlock.type = BLOCK_SENSING_SET_DRAG_MODE;
        dragModeBlock.category = CAT_SENSING;
        dragModeBlock.texture = tex;
        dragModeBlock.rect = {startX, currentY, destW, destH}; 
        dragModeBlock.isHovered = false;
        dragModeBlock.hasDropdown = true;
        dragModeBlock.dropdownRect = {130, 5, 95, 30}; 
        dragModeBlock.isDropdownOpen = false;
        dragModeBlock.options = {"draggable", "not draggable"};
        dragModeBlock.selectedOptionIndex = 0;
        allBlocks.push_back(dragModeBlock);
        currentY += destH + blockGap;
    }

    addBlock(BLOCK_SENSING_TIMER, CAT_SENSING, "Sensing_12.png");
    addBlock(BLOCK_SENSING_RESET_TIMER, CAT_SENSING, "Sensing_13.png");

    //OPERATOR BLOCKS
    currentY = startY;
    addBlock(BLOCK_OPERATOR_ADD, CAT_OPERATORS, "Operators_plus.png");
    addBlock(BLOCK_OPERATOR_SUBTRACT, CAT_OPERATORS, "Operators_minus.png");
    addBlock(BLOCK_OPERATOR_MULTIPLY, CAT_OPERATORS, "Operators_multiply.png");
    addBlock(BLOCK_OPERATOR_DIVIDE, CAT_OPERATORS, "Operators_devision.png");
    addBlock(BLOCK_OPERATOR_GREATER_THAN, CAT_OPERATORS, "Operators_greater than.png");
    addBlock(BLOCK_OPERATOR_LESS_THAN, CAT_OPERATORS, "Operators_less than.png");
    addBlock(BLOCK_OPERATOR_EQUAL, CAT_OPERATORS, "Operators_equals.png");
    addBlock(BLOCK_OPERATOR_AND, CAT_OPERATORS, "Operators_and.png");
    addBlock(BLOCK_OPERATOR_OR, CAT_OPERATORS, "Operators_or.png");
    addBlock(BLOCK_OPERATOR_NOT, CAT_OPERATORS, "Operators_not.png");
    addBlock(BLOCK_OPERATOR_LENGTH_OF, CAT_OPERATORS, "Operators_LengthOfString.png");
    addBlock(BLOCK_OPERATOR_JOIN_STRING, CAT_OPERATORS, "Operators_joinToString.png");
    addBlock(BLOCK_OPERATOR_LETTER_OF, CAT_OPERATORS, "Operators_LetterOfString.png");
}