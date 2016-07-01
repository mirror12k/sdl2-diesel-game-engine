

#include <stdio.h>

#include "diesel/context.hpp"
#include "diesel/graphic_entity.hpp"
#include "diesel/game.hpp"
#include "diesel/dynamic_loading.hpp"
#include "diesel/scene.hpp"

#include "diesel/service.hpp"


class test_entity : public diesel::graphic_entity
{
private:
    int velocity_x = 4, velocity_y = 4;
public:
    test_entity() : diesel::graphic_entity("tetris_grey_block.png", 20, 20)
    {}

    void update (diesel::update_context* ctx)
    {
        if (this->velocity_x > 0)
        {
            this->sprite.rect.x += this->velocity_x;
            if (this->sprite.rect.x >= 800)
                this->velocity_x = -this->velocity_x;
        }
        else
        {
            this->sprite.rect.x += this->velocity_x;
            if (this->sprite.rect.x <= 0)
                this->velocity_x = -this->velocity_x;
        }

        if (this->velocity_y > 0)
        {
            this->sprite.rect.y += this->velocity_y;
            if (this->sprite.rect.y >= 640)
                this->velocity_y = -this->velocity_y;
        }
        else
        {
            this->sprite.rect.y += this->velocity_y;
            if (this->sprite.rect.y <= 0)
                this->velocity_y = -this->velocity_y;
        }
    }

    virtual void load_dynamic(const diesel::dynamic_value& args)
    {
        this->sprite.rect.x = args.at<int>("x");
        this->sprite.rect.y = args.at<int>("y");
    }
};



class test_service : public diesel::service
{
public:
    test_service() : diesel::service("service::test_service")
    {}
    void test_print(int val)
    {
        printf("lol: %d\n", val);
    }
};


class test_user : public diesel::dynamicly_loadable_entity
{
private:
    int counter = 0;
public:
    void update(diesel::update_context* ctx)
    {
        test_service* srv = ctx->get_service<test_service>("service::test_service");
        srv->test_print(this->counter++);
    }
};


class test_physics : public diesel::iterating_entity_service<diesel::graphic_entity>
{
public:
    int step = 0;
    int vx = 4, vy = 4;

    test_physics()
    : diesel::iterating_entity_service<diesel::graphic_entity>("service::test_physics")
    {}
    void update(diesel::update_context* ctx)
    {
        this->diesel::iterating_entity_service<diesel::graphic_entity>::update(ctx);
        if (++this->step == 30)
        {
            this->vx = -this->vx;
            this->vy = -this->vy;
            this->step = 0;
        }
    }
    void process_entity(diesel::update_context* ctx, diesel::graphic_entity* ent)
    {
        ent->move(this->vx, this->vy);
    }
};


class test_shaker : public diesel::graphic_entity
{
public:
    test_shaker() : diesel::graphic_entity("tetris_grey_block.png", 20, 20)
    {}

    void on_added(diesel::update_context* ctx)
    {
        test_physics* srv = ctx->get_service<test_physics>("service::test_physics");
        srv->register_entity(this);
    }
    void on_removed(diesel::update_context* ctx)
    {
        test_physics* srv = ctx->get_service<test_physics>("service::test_physics");
        srv->unregister_entity(this);
    }

    virtual void load_dynamic(const diesel::dynamic_value& args)
    {
        this->sprite.rect.x = args.at<int>("x");
        this->sprite.rect.y = args.at<int>("y");
    }
};




int main ()
{
    printf("hello world!\n");

//    diesel::update_context ents;
//
//    ents.add_entity(new diesel::entity());
//
//    diesel::entity* ent = new test_entity();
//    ents.add_entity(ent);
//    test_entity* ent2 = new test_entity();
//    ents.add_entity(ent2);
//    ents.update();
//
//    printf("removing entity!\n");
//    ents.remove_entity(ent);
//    ents.update();
//
//    ents.update();
//
//    diesel::drawing_context win("lol test", 1200, 640);
//    win.start_graphics();
//
////    SDL_Texture* tex = win.get_texture("tetris_grey_block.png");
////    printf("tex: %x\n", tex);
////    tex = win.get_texture("tetris_grey_block.png");
////    printf("tex: %x\n", tex);
//
//    win.clear();
////    SDL_Rect rect;
////    rect.w = 20;
////    rect.h = 20;
////    rect.x = 100;
////    rect.y = 100;
////    win.draw_texture(tex, &rect);
//
//
////    diesel::named_sprite sprite("tetris_grey_block.png", 20, 20);
////    sprite.rect.x = 100;
////    sprite.rect.y = 300;
////    win.draw_sprite(&sprite);
//
//
////    diesel::named_sprite spritesheet("tetris_blocks.png", 20, 20, 8);
////    spritesheet.rect.x = 200;
////    spritesheet.rect.y = 200;
////    win.draw_sprite(&spritesheet);
////    spritesheet.step_frame();
////    spritesheet.rect.x = 240;
////    win.draw_sprite(&spritesheet);
////    spritesheet.step_frame();
////    spritesheet.step_frame();
////    spritesheet.rect.x = 280;
////    win.draw_sprite(&spritesheet);
//
//    ents.draw(&win);
//
//    ent2->move(15, 40);
//    ents.draw(&win);
//
//    ent2->move(-30, 40);
//    ents.draw(&win);
//
//    win.present();
//
//    SDL_Delay(1000);
//    SDL_Delay(1000);


    diesel::dynamic_loader loader;
    loader.register_class<test_entity>("test_entity");
    loader.register_class<test_service>("test_service");
    loader.register_class<test_user>("test_user");

    loader.register_class<test_physics>("test_physics");
    loader.register_class<test_shaker>("test_shaker");
//    loader.register_class("test_entity", new diesel::dynamic_class_instantiator<test_entity>());
//    loader.register_class("test_service", new diesel::dynamic_class_instantiator<test_service>());
//    loader.register_class("test_user", new diesel::dynamic_class_instantiator<test_user>());

//    diesel::entity* ent = loader.load("asdf", diesel::dynamic_object_value());
//    printf("got ent: %x\n", ent);

    diesel::dynamic_value dval(15);

    dval.as_int();

//
//    diesel::file_scene scn("asdf", "qwerty");
//    scn.load_file(".gitignore");

    diesel::game game ("lol test", 800, 640, 30);
    game.load_scene(new diesel::file_scene("load_sprites.lst", "load_entities.ent", &loader));
    game.run();

    return 0;
}




