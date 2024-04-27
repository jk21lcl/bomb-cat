#include "rest.h"

Rest::Rest(Game* game) : Card(game)
{
    name_ = "Rest";
    type_ = rest;
}

bool Rest::Use()
{
    return true;
}