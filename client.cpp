#include "message.h"

// return true if game ends
bool ResponseMessage(SOCKET socket)
{
    vector<Message> messages = ReceiveMessage(socket);
    for (Message message : messages)
        switch (message.type)
        {
            case no_response:
                OutputColor(message.content, message.color);
                break;
            case response_int:
            {
                OutputColor(message.content, message.color);
                int input;
                InputInteger(message.low, message.high, &input);
                Message(no_response, to_string(input)).Send(socket);
                break;
            }
            case response_str:
            {
                OutputColor(message.content, message.color);
                string input;
                cin >> input;
                Message(no_response, input).Send(socket);
                break;
            }
            case close:
                return true;
            default:
                cout << "IMPOSSIBLE!" << endl;
                break;
        }
    return false;
}

int main() {
    cout << "Welcome to the client!" << endl;
    // client setup
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        std::cout << "Error: WSAStartup failed" << std::endl;
        return 1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET)
    {
        std::cout << "Error: Could not create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345); // Receiver's port
    server_address.sin_addr.s_addr = inet_addr("183.172.168.143");

    // register
    if (connect(client_socket, (sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        std::cout << "Error: Could not connect to server" << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // response for game
    while (true)
    {
        if (ResponseMessage(client_socket))
            break;
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
