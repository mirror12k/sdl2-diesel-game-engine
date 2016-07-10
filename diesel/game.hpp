
#pragma once

#include "update_context.hpp"
#include "scene.hpp"



namespace diesel
{


class game
{
private:
    update_context* entity_data;
    graphics_context graphics;

    bool running = false;
    int fps;
    int frame_delay;

    scene* loading_scene = nullptr;

public:
    game(char* window_title, int window_width, int window_height, int fps);

    void check_input();

    void run();
    void load_scene(scene* scene);
    void start_scene(scene* scene);

    void set_running(bool running);

};



}



