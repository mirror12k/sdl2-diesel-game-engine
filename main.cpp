

#include <stdio.h>

#include "diesel/context.hpp"


class test_entity : public diesel::graphic_entity
{
public:
    void update (diesel::update_context* ctx)
    {
        printf("lol hello\n");
    }
};


int main ()
{
    printf("hello world!\n");

    diesel::update_context ents;

    ents.add_entity(new diesel::entity());

    diesel::entity* ent = new test_entity();
    ents.add_entity(ent);
    ents.add_entity(new test_entity());
    ents.update();

    printf("removing entity!\n");
    ents.remove_entity(ent);
    ents.update();

    ents.update();

    diesel::drawing_context win("lol test", 1200, 640);

    win.start_graphics();
    win.clear();
    win.present();

    SDL_Delay(1000);
    SDL_Delay(1000);

    return 0;
}




