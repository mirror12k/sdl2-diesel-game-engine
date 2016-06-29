
#pragma once

#include "context.hpp"



namespace diesel
{


class game
{
private:
    update_context entity_data;
    drawing_context graphics;

    bool running = false;
    int fps;
    int frame_delay;

public:
    game(char* window_title, int window_width, int window_height, int fps);

    void check_input();

    void run();

    void set_running(bool running);

};



}



