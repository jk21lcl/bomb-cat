#include "game.h"

Dismantle::Dismantle(Game* game) : Card(game)
{
    name_ = "Dismantle";
    type_ = dismantle;
}