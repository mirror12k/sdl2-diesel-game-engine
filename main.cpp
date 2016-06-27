

#include <stdio.h>

#include "diesel/entity.hpp"


class test_entity : public diesel::entity
{
public:
    void update ()
    {
        printf("lol hello\n");
    }
};


int main ()
{
    printf("hello world!\n");

    diesel::entity_pool pool;

    pool.add_entity(new diesel::entity());

    diesel::entity* ent = new test_entity();
    pool.add_entity(ent);
    pool.add_entity(new test_entity());
    pool.update();

    printf("removing entity!\n");
    pool.remove_entity(ent);
    pool.update();


    return 0;
}




