

#include "game.hpp"

namespace diesel
{



game::game(char* window_title, int window_width, int window_height, int fps)
: graphics(window_title, window_width, window_height), fps(fps)
{
    this->entity_data = new update_context();
    this->frame_delay = 1000 / fps;
}


void game::check_input()
{
    const uint8_t* keys = this->entity_data->get_keyboard_state();
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

        this->entity_data->update();
        this->graphics.clear();
        this->entity_data->draw(&this->graphics);
        this->graphics.present();

        if ((this->loading_scene != nullptr) && (this->loading_scene->is_ready()))
        {
            printf("starting scene\n");
            this->start_scene(this->loading_scene);
            this->loading_scene = nullptr;
        }

        SDL_Delay(this->frame_delay);
    }
}

void game::load_scene(scene* scene)
{
    this->loading_scene = scene;
    update_context* new_entities = new update_context();
    scene->set_update_context(new_entities);
    this->entity_data->add_entity(scene);
}

void game::start_scene(scene* scene)
{
    update_context* new_entities = scene->get_update_context();
    this->entity_data->remove_all();
    delete this->entity_data;
    this->entity_data = new_entities;
}


void game::set_running(bool running)
{
    this->running = running;
}


}



