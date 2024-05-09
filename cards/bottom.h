#pragma once

#include "card.h"

class Bottom : public Card
{
    public:
        Bottom(Game* game);
        ~Bottom() {}

        bool Use() override;
};