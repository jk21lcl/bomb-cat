#pragma once

#include "player.h"

class Game
{
    public:
        Game(int num_players);
        ~Game();

        void AddPlayer(Player* player);
        void Start();
    
    private:
        int num_players_;
        int cur_player_id_;
        Player* cur_player_;
        vector<Player*> players_;
        int num_pile_;
        vector<Card*> pile_;

        // initialize the pile, n-1 bombs and 5n random cards except bomb
        void Initialize();
        void ShowStatus() const;
        void Input();
        void UpdateCurPlayer();
        bool CheckGameContinue() const;
        void ReplaceBomb(Card* card);

        template<typename T>
        void DynamicAddCard()
        {
            Card* card = new T(this);
            pile_.push_back(card);
            num_pile_++;
        }
};