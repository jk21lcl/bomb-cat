#pragma once

#include "card.h"

class Detect : public Card
{
    public:
        Detect(Game* game);
        ~Detect() {}

        bool Use() override;
};