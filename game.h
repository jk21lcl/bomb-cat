#pragma once

#include "player.h"

class Game
{
    public:
        Game(int num_players, vector<SOCKET> clients);
        ~Game();

        void AddPlayer(Player* player);
        void Start();
    
    private:
        vector<SOCKET> clients_;
        SOCKET cur_client_;
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
        bool DismantleBomb();
        void ReplaceBomb(Card* card);
        void Broadcast(string content) const; // send no-response message to all players
        void PrivateSend(string content) const; // send no-response message to current player
        void OthersSend(string content) const; // send no-response message to other players

        template<typename T>
        void DynamicAddCard()
        {
            Card* card = new T(this);
            pile_.push_back(card);
            num_pile_++;
        }
};