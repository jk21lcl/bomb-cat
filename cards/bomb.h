#pragma once

#include "card.h"

class Bomb : public Card
{
    public:
        Bomb(Game* game);
        ~Bomb() {}

        bool Use() override { return false; }
};