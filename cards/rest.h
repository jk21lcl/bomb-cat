#pragma once

#include "card.h"

class Rest : public Card
{
    public:
        Rest(Game* game);
        ~Rest() {}

        bool Use() override;
};