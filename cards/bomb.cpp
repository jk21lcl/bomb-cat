#include "bomb.h"

Bomb::Bomb(Game* game) : Card(game)
{
    name_ = "Bomb";
    type_ = bomb;
}