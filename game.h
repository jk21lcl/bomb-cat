#pragma once

#include "player.h"

class Game
{
    public:
        Game(int num_players, vector<SOCKET> clients);
        ~Game();

        void AddPlayer(Player* player);
        void Start();
        void ShowPlayerCards(int id) const;

        void Broadcast(string content, Color color = white) const; // send no-response message to all players
        void PrivateSend(string content, Color color = white) const; // send no-response message to current player
        int PrivateSendInt(string content, int low, int high, Color color = white) const; // send response-int message to current player
        void OthersSend(string content, Color color = white) const; // send no-response message to other players

        int GetNumPlayers() const { return num_players_; }
        vector<Player*> GetPlayers() const { return players_; }
        vector<SOCKET> GetClients() const { return clients_; }
        int GetCurPlayerId() const { return cur_player_id_; }
        Player* GetCurPlayer() const { return cur_player_; }
        SOCKET GetCurClient() const { return cur_client_; }

    private:
        int num_players_;
        vector<Player*> players_;
        vector<SOCKET> clients_;

        int cur_player_id_;
        Player* cur_player_;
        SOCKET cur_client_;

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

        template<typename T>
        void DynamicAddCard()
        {
            Card* card = new T(this);
            pile_.push_back(card);
            num_pile_++;
        }
};