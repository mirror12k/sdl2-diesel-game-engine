
#pragma once


#include "sprite.hpp"


//#include <iostream>
//
//using std::cout;
//using std::endl;

#include <map>
using std::map;

#include <list>
using std::list;

#include "exception.hpp"


namespace diesel
{




class graphics_exception : public generic_exception
{
public:
    graphics_exception(const string& reason);
};







class graphics_context
{
private:
    SDL_Window* window;
    SDL_Surface* screen;
    SDL_Renderer* renderer;

    string window_title;
    int window_width, window_height;

    map<string, SDL_Texture*> loaded_textures;
    map<string, TTF_Font*> loaded_fonts;
    list<texture_reference*> referenced_textures;

public:
    graphics_context(const string& window_title, int window_width, int window_height);
    ~graphics_context();

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

    int garbage_collect_texture_references();

};



}



