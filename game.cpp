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
            case 3:
                DynamicAddCard<Despoil>();
                break;
            case 4:
                DynamicAddCard<Shuffle>();
                break;
            case 5:
                DynamicAddCard<Detect>();
                break;
            case 6:
                DynamicAddCard<Bottom>();
                break;
            default:
                cout << "IMPOSSIBLE!!!" << endl;
        }
    }
    ShuffleDeck();
}

void Game::ShowStatus() const
{
    Broadcast("There are "+ to_string(num_pile_) + " cards remain.");
    string message;
    message += "Alive players:\r\n";
    for (Player* player : players_)
        if (player->GetState() == in)
            message += "  " + player->GetName() + ": " + to_string(player->GetNumCards()) + " cards";

    Broadcast(message, dark_green);
    for (int i = 0; i < num_players_; i++)
        if (i != cur_player_id_)
            ShowPlayerCards(i);
}

void Game::Input()
{
    while (true)
    {
        vector<Card*> cards = cur_player_->GetCards();
        ShowPlayerCards(cur_player_id_);
        string message = "Input the id of the card you use. Enter 0 to directly take a card from the top of the pile.";
        int choice = PrivateSendInt(message, 0, cur_player_->GetNumCards());
        if (choice == 0)
        {
            PlayerGetCard(num_pile_ - 1);
            break;
        }
        else
        {
            if (cards[choice - 1]->GetCardType() == dismantle)
            {
                PrivateSend("You can not directly use Dismantle.");
                continue;
            }
            if (cur_player_->UseCard(choice - 1))
                break;
        }
    }
}

void Game::Start()
{
    Broadcast("\r\nGame Start!\r\n");
    string message = "Players:  ";
    for (int i = 0; i < num_players_; i++)
        message += players_[i]->GetName() + " ";
    Broadcast(message);
    int round = 0;
    while (CheckGameContinue())
    {
        round++;
        Broadcast("\r\nRound: " + to_string(round) + "\r\n", light_pink);
        UpdateCurPlayer();
        OthersSend("It's " + cur_player_->GetName() + "'s turn.");
        PrivateSend("It's your turn.");
        ShowStatus();
        Input();
    }
    for (Player* player : players_)
        if (player->GetState() == in)
            Broadcast(player->GetName() + " wins!");
    for (SOCKET client_socket : clients_)
    {
        Message(close).Send(client_socket);
    }
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
    string message = "You need to dismantle the bomb. Enter the id of the card you want to use. Enter 0 to give up.";
    int choice = PrivateSendInt(message, 0, cur_player_->GetNumCards());
    while (true)
    {
        if (choice == 0)
            return false;
        Card* card = cur_player_->GetCards()[choice - 1];
        if (card->GetCardType() == dismantle)
        {
            cur_player_->DeleteCard(choice - 1);
            return true;
        }
        else
        {
            string error_message = "You can not use this card to dismantle the bomb.";
            choice = PrivateSendInt(error_message, 0, cur_player_->GetNumCards());
        }
    }
}

void Game::ReplaceBomb(Card* card)
{
    string message = "You can put the bomb back to the pile. Enter the position you want to put it at. 0 for the top.";
    int choice = PrivateSendInt(message, 0, num_pile_);
    pile_.emplace(pile_.end() - choice, card);
    num_pile_++;
}

void Game::Broadcast(string content, Color color) const
{
    Message message(no_response, content, color);
    for (SOCKET client_socket : clients_)
        message.Send(client_socket);
}

void Game::PrivateSend(string content, Color color) const
{
    Message(no_response, content, color).Send(cur_client_);
}

int Game::PrivateSendInt(string content, int low, int high, Color color) const
{
    Message(response_int, content, color, low, high).Send(cur_client_);
    return stoi(ReceiveMessage(cur_client_)[0].content);
}

void Game::OthersSend(string content, Color color) const
{
    Message message(no_response, content, color);
    for (SOCKET client_socket : clients_)
        if (client_socket != cur_client_)
            message.Send(client_socket);
}

void Game::ShowPlayerCards(int id) const
{
    Player* player = players_[id];
    SOCKET client = clients_[id];
    string message;
    message += "\r\nYour cards:\r\n  "; 
    vector<Card*> cards = player->GetCards();
    for (int i = 1; i <= player->GetNumCards(); i++)
        message += to_string(i) + ": " + cards[i - 1]->GetName() + " ";
    Message(no_response, message).Send(client);
}

void Game::ShuffleDeck()
{
    random_shuffle(pile_.begin(), pile_.end());
}

void Game::PlayerGetCard(int id)
{
    Card* card = pile_[id];
    pile_.erase(pile_.begin() + id);
    num_pile_--;
    if (card->GetCardType() == bomb)
    {
        PrivateSend("You get a bomb.", light_red);
        OthersSend(cur_player_->GetName() + " gets a bomb.", light_red);
        if (DismantleBomb())
        {
            Broadcast(cur_player_->GetName() + " dismantles the bomb.", light_green);
            ReplaceBomb(card);
        }
        else 
        {
            delete card;
            cur_player_->SetOut();
            Broadcast("The bomb explodes. " + cur_player_->GetName() + " is out.", dark_red);
        }
    }
    else
    {
        PrivateSend("You get a " + card->GetName() + ".", light_blue);
        OthersSend(cur_player_->GetName() + " gets a card.", light_blue);
        cur_player_->AddCard(card);
    }
}