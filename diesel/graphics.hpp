
#pragma once


#include <map>
using std::map;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace diesel
{


class named_sprite
{
public:
    SDL_Texture* texture = nullptr;
    const char* filename;

    SDL_Rect sprite_rect, rect;

    int tiles_x, tiles_y;
    int current_frame_x = 0, current_frame_y = 0;

    named_sprite(const char* filename, int w, int h, int tiles_x=1, int tiles_y=1);

    void step_frame();
};




class drawing_context
{
private:
    SDL_Window* window;
    SDL_Surface* screen;
    SDL_Renderer* renderer;

    char* window_title;
    int window_width, window_height;

    map<const char*, SDL_Texture*> loaded_textures;

public:
    drawing_context(char* window_title, int window_width, int window_height);
    ~drawing_context();

    void start_graphics();
    void end_graphics();

    void clear();
    void present();

    SDL_Surface* load_surface(const char* filename);
    SDL_Texture* load_texture(const char* filename);
    SDL_Texture* surface_to_texture(SDL_Surface* surf);

    SDL_Texture* get_texture(const char* filename);

    void draw_texture(SDL_Texture* tex, SDL_Rect* dst);
    void draw_sub_texture(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dst);

    void draw_sprite(named_sprite* sprite);

};



}



