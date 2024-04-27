#include "game.h"

int main()
{
    srand(time(nullptr));

    int num_players;
    cout << "Input the number of players:" << endl;
    InputInteger(2, 10, &num_players);

    Game game(num_players);
    for (int i = 0; i < num_players; i++)
    {
        Player* player = new Player("player_" + to_string(i), &game);
        game.AddPlayer(player);
    }
    game.Start();

    return 0;
}