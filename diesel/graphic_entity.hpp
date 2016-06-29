

#include "graphics.hpp"
#include "entity.hpp"
#include "context.hpp"

#include "dynamic_loading.hpp"

namespace diesel
{



class graphic_entity : public dynamicly_loadable_entity
{
protected:
    named_sprite sprite;
public:
    graphic_entity(string filename, int x, int y);

    bool is_drawn () const;

    virtual void draw(drawing_context* ctx);

    void move(int deltax, int deltay);
};



}


