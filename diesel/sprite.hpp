
#pragma once



#include <string>
using std::string;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>



namespace diesel
{



class graphic_sprite
{
public:
    SDL_Texture* texture = nullptr;
    SDL_Rect sprite_rect, rect;
};


class named_sprite : public graphic_sprite
{
public:
    string filename;

    int tiles_x, tiles_y;
    int current_frame_x = 0, current_frame_y = 0;

    named_sprite(const string& filename, int w, int h, int tiles_x=1, int tiles_y=1);

    void step_frame();
    void set_frame(int frame);
};



class named_font
{
public:
    TTF_Font* font = nullptr;
    string filename;

    named_font(const string& filename);
};


class reference_counted
{
public:
    uint reference_count = 0;

    int reference();
    int unreference();
};


class texture_reference : public reference_counted
{
public:
    SDL_Texture* texture;
    SDL_Rect sprite_rect;

    texture_reference(SDL_Texture* texture, const SDL_Rect& sprite_rect);
};

class referenced_sprite : public graphic_sprite
{
public:
    texture_reference* source = nullptr;

//    int tiles_x, tiles_y;
//    int current_frame_x = 0, current_frame_y = 0;

    referenced_sprite(const referenced_sprite& other);
    referenced_sprite();
    referenced_sprite(texture_reference* ref);

    ~referenced_sprite();
    referenced_sprite& operator=(const referenced_sprite& other);

    void reference(texture_reference* source);
    void unreference();

//    void step_frame();
//    void set_frame(int frame);

};



class rendered_text_sprite : public referenced_sprite
{
public:
    named_font font;
    string text;

    rendered_text_sprite(const string& font_filename, const string& text="");

    // setting the text variable directly won't un-reference any old rendered texture
    void set_text(const string& text);
};



}




