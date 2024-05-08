#pragma once

#include "card.h"

class Despoil : public Card
{
    public:
        Despoil(Game* game);
        ~Despoil() {}

        bool Use() override;
};