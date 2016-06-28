

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

    diesel::named_sprite sprite("tetris_grey_block.png", 20, 20);
    sprite.rect.x = 100;
    sprite.rect.y = 300;
//    SDL_Texture* tex = win.get_texture("tetris_grey_block.png");
//    printf("tex: %x\n", tex);
//    tex = win.get_texture("tetris_grey_block.png");
//    printf("tex: %x\n", tex);

    win.clear();
//    SDL_Rect rect;
//    rect.w = 20;
//    rect.h = 20;
//    rect.x = 100;
//    rect.y = 100;
//    win.draw_texture(tex, &rect);
    win.draw_sprite(&sprite);
    win.present();

    SDL_Delay(1000);
    SDL_Delay(1000);

    return 0;
}




