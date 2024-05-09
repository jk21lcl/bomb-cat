#pragma once

#include "card.h"

class Shuffle : public Card
{
    public:
        Shuffle(Game* game);
        ~Shuffle() {}

        bool Use() override;
};