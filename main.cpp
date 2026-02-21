#include <iostream>
#include <cmath>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <vector>
#include <cstring>

using namespace std;
struct Value {
    enum Type { NUM, STR, BOOL } type;
    union {
        double num;
        bool b;
    };
    std::string str;

    Value() : type(NUM), num(0) {}
    Value(double d) : type(NUM), num(d) {}
    Value(bool b) : type(BOOL), b(b) {}
    Value(const std::string& s) : type(STR), str(s) {}
};
enum GraphicEffect {
    EFFECT_COLOR,
    EFFECT_FISHEYE,
    EFFECT_WHIRL,
    EFFECT_PIXELATE,
    EFFECT_MOSAIC,
    EFFECT_BRIGHTNESS,
    EFFECT_GHOST,
    NUM_EFFECTS
};
struct Sprite {
    double x, y;
    double direction;
    double size;
    bool visible;
    bool draggable;
    int currentCostume;
    vector<SDL_Texture*> costumes;
    float volume;                 
    float pitch;                 
    float pan; 

    string bubbleText;
    Uint32 bubbleExpireTime;
    bool bubbleIsThink;
    float effects[NUM_EFFECTS];

    //INITIALIZE
    Sprite() : x(0), y(0), direction(90), size(100), visible(true), draggable(false),
               currentCostume(0), bubbleText(""), bubbleExpireTime(0), bubbleIsThink(false), volume(100), pitch(0), pan(0) {
        for (int i = 0; i < NUM_EFFECTS; ++i) effects[i] = 0.0f;
    }
};
struct Stage {
    int currentBackdrop;
    std::vector<SDL_Texture*> backdrops;

    Stage() : currentBackdrop(0) {}
};
struct Runtime {
    Sprite currentSprite;
    Stage stage;
    int mouseX, mouseY;
    bool mouseDown;
    bool keyPressed[SDL_NUM_SCANCODES];
    Uint32 timerStart;
    std::string answer;

    Runtime() : mouseX(0), mouseY(0), mouseDown(false), timerStart(SDL_GetTicks()) {
        memset(keyPressed, 0, sizeof(keyPressed));
    }

    double timerValue() const {
        return (SDL_GetTicks() - timerStart) / 1000.0;
    }
};
extern Runtime g_runtime;


void motion_move_steps(Runtime& rt, double steps) {
    double rad = rt.currentSprite.direction * M_PI / 180.0;
    double dx = steps * sin(rad);
    double dy = -steps * cos(rad);
    rt.currentSprite.x += dx;
    rt.currentSprite.y += dy;
}
void motion_turn_right(Runtime& rt, double degrees) {
    rt.currentSprite.direction += degrees;
}
void motion_turn_left(Runtime& rt, double degrees) {
    rt.currentSprite.direction -= degrees;
}
void motion_goto_xy(Runtime& rt, double x, double y) {
    rt.currentSprite.x = x;
    rt.currentSprite.y = y;
}
void motion_goto(Runtime& rt, const string& target) {
    if (target == "random") {
        rt.currentSprite.x = (rand() % 480) - 240;
        rt.currentSprite.y = (rand() % 360) - 180;
    } else if (target == "mouse-pointer") {
        rt.currentSprite.x = rt.mouseX;
        rt.currentSprite.y = rt.mouseY;
    }
}
void motion_change_x(Runtime& rt, double dx) {
    rt.currentSprite.x += dx;
}
void motion_change_y(Runtime& rt, double dy) {
    rt.currentSprite.y += dy;
}
void motion_set_x(Runtime& rt, double x) {
    rt.currentSprite.x = x;
}

void motion_set_y(Runtime& rt, double y) {
    rt.currentSprite.y = y;
}

void motion_point_direction(Runtime& rt, double dir) {
    rt.currentSprite.direction = dir;
}
void motion_point_towards(Runtime& rt, const string& target) {
    double tx = 0, ty = 0;
    if (target == "mouse-pointer") {
        tx = rt.mouseX;
        ty = rt.mouseY;
    } else {
        return;
    }
    double dx = tx - rt.currentSprite.x;
    double dy = ty - rt.currentSprite.y;
    double angle = atan2(dy, dx) * 180.0 / M_PI;
    rt.currentSprite.direction = 90.0 - angle;
}
void motion_if_on_edge_bounce(Runtime& rt) {
    double x = rt.currentSprite.x;
    double y = rt.currentSprite.y;
    bool bounced = false;
    if (x < -240) {
        x = -240;
        rt.currentSprite.direction = 180 - rt.currentSprite.direction;
        bounced = true;
    } else if (x > 240) {
        x = 240;
        rt.currentSprite.direction = 180 - rt.currentSprite.direction;
        bounced = true;
    }
    if (y < -180) {
        y = -180;
        rt.currentSprite.direction = -rt.currentSprite.direction;
        bounced = true;
    } else if (y > 180) {
        y = 180;
        rt.currentSprite.direction = -rt.currentSprite.direction;
        bounced = true;
    }
    if (bounced) {
        rt.currentSprite.x = x;
        rt.currentSprite.y = y;
        while (rt.currentSprite.direction > 180) rt.currentSprite.direction -= 360;
        while (rt.currentSprite.direction < -180) rt.currentSprite.direction += 360;
    }
}



//lookFunctions
void looks_say(Runtime& rt, const std::string& text, double seconds) {
    rt.currentSprite.bubbleText = text;
    rt.currentSprite.bubbleIsThink = false;
    if (seconds > 0) {
        rt.currentSprite.bubbleExpireTime = SDL_GetTicks() + (Uint32)(seconds * 1000);
    } else {
        rt.currentSprite.bubbleExpireTime = 0;
    }
}
void looks_say_for_seconds(Runtime& rt, const std::string& text, double seconds) {
    looks_say(rt, text, seconds);
}
void looks_think(Runtime& rt, const std::string& text, double seconds) {
    rt.currentSprite.bubbleText = text;
    rt.currentSprite.bubbleIsThink = true;
    if (seconds > 0) {
        rt.currentSprite.bubbleExpireTime = SDL_GetTicks() + (Uint32)(seconds * 1000);
    } else {
        rt.currentSprite.bubbleExpireTime = 0;
    }
}
void looks_think_for_seconds(Runtime& rt, const std::string& text, double seconds) {
    looks_think(rt, text, seconds);
}
void looks_switch_costume(Runtime& rt, const std::string& costumeName) {
    int idx = atoi(costumeName.c_str()) - 1; //BC scratch starts it from 1 not 0
    if (idx >= 0 && idx < (int)rt.currentSprite.costumes.size()) {
        rt.currentSprite.currentCostume = idx;
    }
}
void looks_next_costume(Runtime& rt) {
    if (!rt.currentSprite.costumes.empty()) {
        rt.currentSprite.currentCostume = (rt.currentSprite.currentCostume + 1) % rt.currentSprite.costumes.size();
    }
}
void looks_switch_backdrop(Runtime& rt, const std::string& backdropName) {
    int idx = atoi(backdropName.c_str()) - 1;
    if (idx >= 0 && idx < (int)rt.stage.backdrops.size()) {
        rt.stage.currentBackdrop = idx;
    }
}

void looks_next_backdrop(Runtime& rt) {
    if (!rt.stage.backdrops.empty()) {
        rt.stage.currentBackdrop = (rt.stage.currentBackdrop + 1) % rt.stage.backdrops.size();
    }
}

void looks_change_size(Runtime& rt, double delta) {
    rt.currentSprite.size += delta;
    if (rt.currentSprite.size < 0) rt.currentSprite.size = 0;
}
void looks_set_size(Runtime& rt, double size) {
    rt.currentSprite.size = size;
    if (rt.currentSprite.size < 0) rt.currentSprite.size = 0;
}

void looks_change_effect(Runtime& rt, GraphicEffect effect, double delta) {
    if (effect >= 0 && effect < NUM_EFFECTS) {
        rt.currentSprite.effects[effect] += delta;
    }
}

void looks_set_effect(Runtime& rt, GraphicEffect effect, double value) {
    if (effect >= 0 && effect < NUM_EFFECTS) {
        rt.currentSprite.effects[effect] = value;
    }
}

void looks_clear_effects(Runtime& rt) {
    for (int i = 0; i < NUM_EFFECTS; ++i) {
        rt.currentSprite.effects[i] = 0;
    }
}

void looks_show(Runtime& rt) {
    rt.currentSprite.visible = true;
}

void looks_hide(Runtime& rt) {
    rt.currentSprite.visible = false;
}


Value looks_size(Runtime& rt) {
    return Value(rt.currentSprite.size);
}

Value looks_costume_number_name(Runtime& rt, const std::string& mode) {
    if (mode == "number") {
        return Value((double)(rt.currentSprite.currentCostume + 1));
    } else {
        return Value("");
    }
}

Value looks_backdrop_number_name(Runtime& rt, const std::string& mode) {
    if (mode == "number") {
        return Value((double)(rt.stage.currentBackdrop + 1));
    } else {
        return Value("");
    }
}
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
