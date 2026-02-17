#include <iostream>
#include <SDL2/SDL.h>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;
using namespace std;
const int screen_width = 800;
const int screen_height = 600;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool initSDL();
void closeSDL();

struct block{
    string command;
 //   string category;
 //   double value1;
  //  double value2;
//    string text;
  //  bool boolean; //it will probabely be needed in events
};
struct sprite{
    string name;
    int x , y;
    int size;
    int direction;
    int index;
    bool visible;
    string customPath;
    vector<block> blocks;
};
bool saveToFile(const string filename , const string content){
    ofstream file(filename);
    if (!file.is_open()){
        return false;
    }
    file << content;
    file.close();
    return true;
}
string readFromFile(const string filename){
    ifstream file(filename);
    if(!file.is_open()){
        return "";
    }
    string content , line;
    while (getline(file , line)){
        content += line;
        content += "\n";
    }
    file.close();
    return content;
}

string serialize(const vector<sprite>& sprites){
    stringstream ss;
    for(size_t i = 0 ; i < sprites.size() ; i++){
        const sprite& s = sprites[i];
        ss << "$SPRITE" << "\n";
        ss << "name = "<< s.name<< "\n";
        ss << "xPosition = "<<s.x << "\n";
        ss << "yPosirion = "<<s.y <<"\n";
        ss << "size = "<<s.size << "\n";
        ss << "direction = "<<s.direction<<"\n";
        ss << "index = "<<s.index <<"\n";
        ss <<"visible= "<<(s.visible ? "true" : "false") <<"\n";
        ss << "customPath : "<< s.customPath << "\n";
        ss << "$Blocks" << "\n";
        for (size_t j = 0 ; j < s.blocks.size() ; j++){
            ss << s.blocks[j].command << "\n"; //<< s.blocks[j].size1 <<  "\t" << s.blocks[j].size2 << "\n";
        }
        ss << "$ENDSPRITE\n";
    }
    return ss.str();
}
vector<sprite> deserialize(const string& filename){
    vector<sprite> sprites;
    string content = readFromFile(filename);
    if(content.empty()){
        return sprites;
    }
    stringstream ss(content);
    string line;
    sprite currentSprite;
    bool readingSprite = false;
    bool readingBlocks = false;
    while(getline(ss , line)){
        if(line.empty()) continue;
        if(line == "$SPRITE"){
            if(readingSprite){
                sprites.push_back(currentSprite);
            }
            currentSprite = sprite();
            readingSprite = true;
            readingBlocks = false;
            continue;
        }
        else if (line == "$BLOCKS"){
            readingBlocks = true;
            continue;
        }
        else if (line == "$ENDSPRITE"){
            if(readingSprite){
                sprites.push_back(currentSprite);
            }
            readingSprite = false;
            readingBlocks = false;
            continue;
        }
        if(!readingSprite) continue;
        if(!readingBlocks){
            size_t equalspos = line.find('=');
            if(equalspos != string::npos){
                string key = line.substr(0,equalspos);
                string value = line.substr(equalspos+1);
                if(key == "name"){
                    currentSprite.name = value;
                }
                else if (key == "x") {
                    currentSprite.x = stoi(value);  // string to int
                }
                else if (key == "y") {
                    currentSprite.y = stoi(value);
                }
                else if (key == "size") {
                    currentSprite.size = stoi(value);
                }
                else if (key == "direction") {
                    currentSprite.direction = stoi(value);
                }
                else if (key == "index") {
                    currentSprite.index = stoi(value);
                }
                else if (key == "visible") {
                    currentSprite.visible = (value == "true");
                }
                else if (key == "customPath") {
                    currentSprite.customPath = value;
                }
            }
        }
        else{
            if(!line.empty()){
                block b;
                b.command = line;
                currentSprite.blocks.push_back(b);
            }
        }
    }
    if(readingSprite){
        sprites.push_back(currentSprite);
    }
    return sprites;
}

int main(int argc, char* argv[]) {


}

