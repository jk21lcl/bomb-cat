#include "game.h"

Detect::Detect(Game* game) : Card(game)
{
    name_ = "Detect";
    type_ = detect;
}

bool Detect::Use()
{
    game_->PrivateSend("You use detect.", light_cyan);
    game_->OthersSend(game_->GetCurPlayer()->GetName() + " uses detect.", light_cyan);
    int num_pile = game_->GetNumPile();
    vector<Card*> cards = game_->GetPile();
    string message = "The top 3 cards are:\r\n  ";
    for (int i = 0; i < min(3, num_pile); i++)
        message += cards[num_pile - 1 - i]->GetName() + " ";
    game_->PrivateSend(message);
    return false;
}