#include "game.h"

Game::Game(int num_players, vector<SOCKET> clients)
{
    clients_ = clients;
    num_players_ = num_players;
    cur_player_id_ = -1;
    num_pile_ = 0;
    Initialize();
}

Game::~Game()
{
    for (Card* card : pile_)
        delete card;
}

void Game::AddPlayer(Player* player)
{
    players_.push_back(player);
}

void Game::Initialize()
{
    for (int i = 0; i < num_players_ - 1; i++)
        DynamicAddCard<Bomb>();
    for (int i = 0; i < 5 * num_players_; i++)
    {
        int type = 1 + rand() % (NUM_CARD_TYPE - 1);
        switch (type)
        {
            case 1:
                DynamicAddCard<Dismantle>();
                break;
            case 2:
                DynamicAddCard<Rest>();
                break;
            default:
                cout << "IMPOSSIBLE!!!" << endl;
        }
    }
    random_shuffle(pile_.begin(), pile_.end());
}

void Game::ShowStatus() const
{
    string message;
    message += "Alive players:\r\n  ";
    for (Player* player : players_)
        if (player->GetState() == in)
            message += player->GetName() + " ";
    message += "\r\n";
    message += "Your cards:\r\n  "; 
    vector<Card*> cards = cur_player_->GetCards();
    for (int i = 1; i <= cur_player_->GetNumCards(); i++)
        message += to_string(i) + ": " + cards[i - 1]->GetName() + " ";
    PrivateSend(message);
}

void Game::Input()
{
    vector<Card*> cards = cur_player_->GetCards();
    PrivateSend("Input the id of the card you use. Enter 0 to directly take a card from the top of the pile.");
    while (true)
    {
        Send_Message(response_int, cur_client_, "", 0, cur_player_->GetNumCards());
        int choice = stoi(Receive_Message(cur_client_));
        if (choice == 0)
        {
            Card* card = pile_.back();
            pile_.pop_back();
            num_pile_--;
            Broadcast(cur_player_->GetName() + " get a " + card->GetName() + ".");
            if (card->GetCardType() == bomb)
            {
                if (DismantleBomb())
                    ReplaceBomb(card);
                else 
                {
                    delete card;
                    cur_player_->SetOut();
                }
            }
            else
                cur_player_->AddCard(card);
            break;
        }
        else
        {
            if (cards[choice - 1]->GetCardType() == dismantle)
            {
                PrivateSend("You can not directly use Dismantle.");
                continue;
            }
            Broadcast(cur_player_->GetName() + " uses " + cards[choice - 1]->GetName() + ".");
            if (cur_player_->UseCard(choice - 1))
                break;
        }
    }
}

void Game::Start()
{
    Broadcast("Game Start!");
    for (int i = 0; i < num_players_; i++)
    {
        string message = "You are " + players_[i]->GetName() + ".";
        Send_Message(no_response, clients_[i], message);
    }
    int round = 0;
    while (CheckGameContinue())
    {
        round++;
        Broadcast("\r\nRound: " + to_string(round) + "\r\n");
        UpdateCurPlayer();
        Broadcast("It's " + cur_player_->GetName() + "'s turn. There are "+ to_string(num_pile_) + " cards remain.");
        ShowStatus();
        Input();
    }
    for (Player* player : players_)
        if (player->GetState() == in)
            Broadcast(player->GetName() + " wins!");
    for (SOCKET client_socket : clients_)
        Send_Message(close, client_socket, "");
}

void Game::UpdateCurPlayer()
{
    do
    {
        cur_player_id_ = (cur_player_id_ + 1) % num_players_;
        cur_player_ = players_[cur_player_id_];
    }
    while (cur_player_->GetState() == out);
    cur_client_ = clients_[cur_player_id_];
}

bool Game::CheckGameContinue() const
{
    int num_in_players = 0;
    for (Player* player : players_)
        if (player->GetState() == in)
            num_in_players++;
    if (num_in_players > 1)
        return true;
    return false;
}

bool Game::DismantleBomb()
{
    PrivateSend("You need to dismantle the bomb. Enter the id of the card you want to use. Enter 0 to give up.");
    while (true)
    {
        Send_Message(response_int, cur_client_, "", 0, cur_player_->GetNumCards());
        int choice = stoi(Receive_Message(cur_client_));
        if (choice == 0)
            return false;
        Card* card = cur_player_->GetCards()[choice - 1];
        if (card->GetCardType() == dismantle)
        {
            cur_player_->DeleteCard(choice - 1);
            return true;
        }
        else 
            PrivateSend("You can not use this card to dismantle the bomb.");
    }
}

void Game::ReplaceBomb(Card* card)
{
    PrivateSend("You can put the bomb back to the pile. Enter the position you want to put it at. 0 for the top.");
    Send_Message(response_int, cur_client_, "", 0, num_pile_);
    int choice = stoi(Receive_Message(cur_client_));
    pile_.emplace(pile_.end() - choice, card);
    num_pile_++;
}

void Game::Broadcast(string content) const
{
    for (SOCKET client_socket : clients_)
        Send_Message(no_response, client_socket, content);
}

void Game::PrivateSend(string content) const
{
    Send_Message(no_response, cur_client_, content);
}