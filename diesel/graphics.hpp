



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


}



