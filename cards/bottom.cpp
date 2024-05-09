#include "game.h"

Bottom::Bottom(Game* game) : Card(game)
{
    name_ = "Bottom";
    type_ = bottom;
}

bool Bottom::Use()
{
    game_->PrivateSend("You use bottom.", light_cyan);
    game_->OthersSend(game_->GetCurPlayer()->GetName() + " uses bottom.", light_cyan);
    game_->PlayerGetCard(0);
    return true;
}