
#pragma once



#include <iostream>

using std::cout;
using std::endl;

#include <map>
using std::map;

#include <list>
using std::list;

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






class drawing_context
{
private:
    SDL_Window* window;
    SDL_Surface* screen;
    SDL_Renderer* renderer;

    char* window_title;
    int window_width, window_height;

    map<string, SDL_Texture*> loaded_textures;
    map<string, TTF_Font*> loaded_fonts;
    list<texture_reference*> referenced_textures;

public:
    drawing_context(char* window_title, int window_width, int window_height);
    ~drawing_context();

    void start_graphics();
    void end_graphics();

    void clear();
    void present();

    SDL_Surface* load_surface(const string& filename);
    SDL_Texture* load_texture(const string& filename);
    SDL_Texture* surface_to_texture(SDL_Surface* surf);

    SDL_Texture* get_texture(const string& filename);

    void set_texture_alpha(SDL_Texture* tex, uint8_t alpha);

    void draw_texture(SDL_Texture* tex, SDL_Rect* dst);
    void draw_sub_texture(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dst);

    void load_named_sprite(named_sprite* sprite);

    void draw_sprite(graphic_sprite* sprite);
    void draw_sprite_alpha(graphic_sprite* sprite, uint8_t alpha);
    void draw_sprite_offset(graphic_sprite* sprite, int offsetx, int offsety);
    void draw_sprite_rect(graphic_sprite* sprite, int offsetx, int offsety);
    void draw_sprite_tile(graphic_sprite* sprite, SDL_Rect* dest, int tile_x, int tile_y);

    TTF_Font* load_ttf_font(const string& filename);
    TTF_Font* get_font(const string& filename);

    void load_named_font(named_font* font);
    void load_rendered_text_sprite(rendered_text_sprite* sprite);

    void render_font_text(referenced_sprite* sprite, named_font* font, const string& text, const SDL_Color& color = {0, 0, 0});

    texture_reference* create_texture_reference(SDL_Texture* tex, const SDL_Rect& rect);

};



}



