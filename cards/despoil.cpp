#include "game.h"

Despoil::Despoil(Game* game) : Card(game)
{
    name_ = "Despoil";
    type_ = despoil;
}

bool Despoil::Use()
{
    string message = "Input your target:";
    int choice = game_->PrivateSendInt(message, 1, game_->GetNumPlayers());
    while (true)
    {
        if (choice - 1 == game_->GetCurPlayerId())
        {
            string error_message_1 = "You can not choose yourself.";
            choice = game_->PrivateSendInt(error_message_1, 1, game_->GetNumPlayers());
            continue;
        }

        Player* target_player = game_->GetPlayers()[choice - 1];
        SOCKET target_client = game_->GetClients()[choice - 1];
        if (target_player->GetState() == out)
        {
            string error_message_2 = "You can not choose player that is out.";
            choice = game_->PrivateSendInt(error_message_2, 1, game_->GetNumPlayers());
            continue;
        }

        game_->PrivateSend("You use despoil. Target: " + to_string(choice));
        game_->OthersSend(game_->GetCurPlayer()->GetName() + " uses despoil. Target: " + to_string(choice));

        int num_target_cards = target_player->GetNumCards();
        if (num_target_cards == 0)
        {
            game_->PrivateSend("You get nothing.");
            break;
        }
        else 
        {
            int target = rand() % num_target_cards;
            Card* card = target_player->GetCards()[target];
            game_->GetCurPlayer()->AddCard(card);
            target_player->RemoveCard(target);
            game_->PrivateSend("You get a " + card->GetName() + ".", light_blue);
            Message send_message_2(no_response, "Your " + card->GetName() + " is despoiled.", light_yellow);
            send_message_2.Send(target_client);
            game_->ShowPlayerCards(choice - 1);
            break;
        }
    }
    return false;
}