#include "game.h"

vector<SOCKET> clients;
vector<string> names;

bool CheckNameValid(string name)
{
    for (string existing_name : names)
        if (name == existing_name)
            return false;
    return true;
}

int main()
{
    srand(time(nullptr));
    cout << "Welcome to the server!" << endl;

    // server setup
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        std::cout << "Error: WSAStartup failed" << std::endl;
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET)
    {
        std::cout << "Error: Could not create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345); // Receiver's port

    if (bind(server_socket, (sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        std::cout << "Error: Could not bind to port" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 1) == SOCKET_ERROR)
    {
        std::cout << "Error: Could not listen on socket" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening..." << std::endl;

    // register
    int num_players;
    cout << "Input the number of players:" << endl;
    InputInteger(2, 10, &num_players);

    int num_registered_players = 0;
    while (num_registered_players < num_players)
    {
        SOCKET client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET)
        {
            std::cout << "Error: Could not accept connection" << std::endl;
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }
        cout << "client " << client_socket << " registers." << endl;

        // add name
        Message(response_str, "Input your name:").Send(client_socket);
        while (true)
        {
            string name = ReceiveMessage(client_socket)[0].content;
            if (CheckNameValid(name))
            {
                names.push_back(name);
                break;
            }
            Message(response_str, "The name exists. Please input again.").Send(client_socket);
        }

        Message(no_response, "Waiting for other players to enter...").Send(client_socket);
        clients.push_back(client_socket);
        num_registered_players++;
    }

    // game start
    Game game(num_players, clients);
    for (int i = 0; i < num_players; i++)
    {
        Player* player = new Player(names[i], &game);
        game.AddPlayer(player);
    }
    game.Start();

    for (SOCKET client_socket : clients)
        closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}