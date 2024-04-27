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

bool Player::DismantleBomb()
{
    cout << "You need to dismantle the bomb. Enter the id of the card you want to use. Enter 0 to give up." << endl;
    while (true)
    {
        int choice;
        InputInteger(0, num_cards_, &choice);
        if (choice == 0)
            return false;
        Card* card = cards_[choice - 1];
        if (card->GetCardType() == dismantle)
        {
            delete card;
            cards_.erase(cards_.begin() + choice - 1);
            num_cards_--;
            return true;
        }
        else 
            cout << "You can not use this card to dismantle the bomb." << endl;
    }
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

void InputInteger(int m, int n, int* out)
{
    int input;
    while (true)
    {
        cin >> input;
        if (input < m || input > n)
        {
            cout << "Input out of range. Please input again." << endl;
            continue;
        }
        break;
    }
    *out = input;
}