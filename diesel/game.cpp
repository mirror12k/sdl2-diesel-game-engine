

#include "game.hpp"

namespace diesel
{



game::game(char* window_title, int window_width, int window_height, int fps)
: graphics(window_title, window_width, window_height), fps(fps)
{
    this->frame_delay = 1000 / fps;
}


void game::check_input()
{
    const uint8_t* keys = this->entity_data.get_keyboard_state();
    if (keys[SDL_SCANCODE_ESCAPE])
    {
        this->running = false;
    }
}


void game::run()
{
    this->graphics.start_graphics();
    this->running = true;

    while (this->running)
    {
        SDL_PumpEvents();
        this->check_input();

        this->entity_data.update();
        this->graphics.clear();
        this->entity_data.draw(&this->graphics);
        this->graphics.present();

        SDL_Delay(this->frame_delay);
    }
}

void game::load_scene(scene* scene)
{
    this->entity_data.add_entity(scene);
//    scene->load_graphics(&this->graphics);
//    scene->load_entities(&this->entity_data);
}


void game::set_running(bool running)
{
    this->running = running;
}


}



