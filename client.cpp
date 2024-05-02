#include "message.h"

// return true if game ends
bool ResponseMessage(SOCKET socket)
{
    string message = Receive_Message(socket);
    MessageType type = (MessageType)(message[0] - '0');
    switch (type)
    {
        case no_response:
            cout << message.substr(1) << endl;
            return false;
        case response_int:
        {
            int low = message[1] - '0';
            int high = message[2] - '0';
            int input;
            InputInteger(low, high, &input);
            Send_Message(no_response, socket, to_string(input));
            return false;
        }
        case close:
            return true;
        default:
            cout << "IMPOSSIBLE!" << endl;
            return false;
    }
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
    server_address.sin_addr.s_addr = inet_addr("183.172.174.94");

    // register
    if (connect(client_socket, (sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        std::cout << "Error: Could not connect to server" << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    cout << "Enter your name:" << endl;
    string name;
    cin >> name;
    Send_Message(no_response, client_socket, name);

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
