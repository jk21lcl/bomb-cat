#pragma once

#include "card.h"

class Dismantle : public Card
{
    public:
        Dismantle(Game* game);
        ~Dismantle() {}

        bool Use() override { return false; }
};