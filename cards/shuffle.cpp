#include "game.h"

Shuffle::Shuffle(Game* game) : Card(game)
{
    name_ = "Shuffle";
    type_ = CardType::shuffle;
}

bool Shuffle::Use()
{
    game_->ShuffleDeck();
    game_->PrivateSend("You use shuffle. The pile has been shuffled.", light_cyan);
    game_->OthersSend(game_->GetCurPlayer()->GetName() + " uses shuffle. The pile has been shuffled.", light_cyan);
    return false;
}