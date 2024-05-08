#include "game.h"

Rest::Rest(Game* game) : Card(game)
{
    name_ = "Rest";
    type_ = rest;
}

bool Rest::Use()
{
    game_->PrivateSend("You use rest.");
    game_->OthersSend(game_->GetCurPlayer()->GetName() + "uses rest.");
    return true;
}