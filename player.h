#pragma once

#include "cards.h"

const int NUM_CARD_TYPE = 3;

enum PlayerState
{
    in,
    out
};

class Player
{
    public:
        Player(string name, Game* game);
        ~Player() {}

        string GetName() const { return name_; }
        PlayerState GetState() const { return state_; }
        int GetNumCards() const { return num_cards_; }
        vector<Card*> GetCards() const { return cards_; }
        void SetOut();

        bool UseCard(int id);
        void AddCard(Card* card);
        bool DismantleBomb();
    
    private:
        Game* game_;
        string name_;
        PlayerState state_;
        int num_cards_;
        vector<Card*> cards_;

        // initialize the cards, 1 dismantle and 4 random cards except bomb
        void Initialize();

        template<typename T>
        void DynamicAddCard()
        {
            Card* card = new T(game_);
            cards_.push_back(card);
            num_cards_++;
        }
};

void InputInteger(int m, int n, int* out);