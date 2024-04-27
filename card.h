#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

enum CardType
{
    bomb,
    dismantle,
    rest
};

class Game;

class Card
{
    public:
        Card(Game* game) : game_(game) {}
        virtual ~Card() {}

        string GetName() const { return name_; }
        CardType GetCardType() const { return type_; }
        virtual bool Use() = 0; // return whether skip this round
    
    protected:
        Game* game_;
        string name_;
        CardType type_;
};