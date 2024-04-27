#include "game.h"

Game::Game(int num_players)
{
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
    cout << "There are " << num_pile_ << " cards remain." << endl;
    cout << "Alive players:" << endl;
    for (Player* player : players_)
        if (player->GetState() == in)
            cout << player->GetName() << " ";
    cout << endl;
    cout << "Your cards:" << endl;
    vector<Card*> cards = cur_player_->GetCards();
    for (int i = 1; i <= cur_player_->GetNumCards(); i++)
        cout << i << ": " << cards[i - 1]->GetName() << " ";
    cout << endl;
}

void Game::Input()
{
    vector<Card*> cards = cur_player_->GetCards();
    cout << "Input the id of the card you use. Enter 0 to directly take a card from the top of the pile." << endl;
    while (true)
    {
        int choice;
        InputInteger(0, cur_player_->GetNumCards(), &choice);
        if (choice == 0)
        {
            Card* card = pile_.back();
            pile_.pop_back();
            num_pile_--;
            cout << "You get a " << card->GetName() << "." << endl;
            if (card->GetCardType() == bomb)
            {
                if (cur_player_->DismantleBomb())
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
                cout << "You can not directly use Dismantle." << endl;
                continue;
            }
            if (cur_player_->UseCard(choice - 1))
                break;
        }
    }
}

void Game::Start()
{
    int round = 0;
    while (CheckGameContinue())
    {
        round++;
        cout << "Round: " << round << endl;
        UpdateCurPlayer();
        cout << "It's " << cur_player_->GetName() << "'s turn." << endl;
        ShowStatus();
        Input();
    }
    for (Player* player : players_)
        if (player->GetState() == in)
            cout << player->GetName() << " wins!" << endl;
}

void Game::UpdateCurPlayer()
{
    do
    {
        cur_player_id_ = (cur_player_id_ + 1) % num_players_;
        cur_player_ = players_[cur_player_id_];
    }
    while (cur_player_->GetState() == out);
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

void Game::ReplaceBomb(Card* card)
{
    cout << "You can put the bomb back to the pile. Enter the position you want to put it at. 0 for the top." << endl;
    int choice;
    InputInteger(0, num_pile_, &choice);
    pile_.emplace(pile_.end() - choice, card);
    num_pile_++;
}