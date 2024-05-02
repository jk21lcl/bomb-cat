#include "player.h"

Player::Player(string name, Game* game)
{
    name_ = name;
    game_ = game;
    state_ = in;
    num_cards_ = 0;
    Initialize();
}

bool Player::UseCard(int id)
{
    bool skip = cards_[id]->Use();
    delete cards_[id];
    cards_.erase(cards_.begin() + id);
    num_cards_--;
    return skip;
}

void Player::AddCard(Card* card)
{
    cards_.push_back(card);
    num_cards_++;
}

void Player::DeleteCard(int id)
{
    delete cards_[id];
    cards_.erase(cards_.begin() + id);
    num_cards_--;
}

void Player::SetOut()
{
    state_ = out;
    for (Card* card : cards_)
        delete card;
    cards_.clear();
    num_cards_ = 0;
}

void Player::Initialize()
{
    DynamicAddCard<Dismantle>();
    for (int i = 0; i < 4; i++)
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
}