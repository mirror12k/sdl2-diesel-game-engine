

#include "graphics.hpp"
#include "entity.hpp"
#include "context.hpp"

namespace diesel
{



class graphic_entity : public entity
{
private:
    named_sprite sprite;
public:
    graphic_entity(string filename, int x, int y);

    bool is_drawn () const;

    virtual void draw(drawing_context* ctx);

    void move(int deltax, int deltay);
};



}


