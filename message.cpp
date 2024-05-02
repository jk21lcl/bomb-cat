#include "message.h"

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

void Send_Message(MessageType type, SOCKET socket, string content, int low, int high)
{
    string head;
    head.push_back((int)type + '0');
    if (type == response_int)
    {
        head.push_back(low + '0');
        head.push_back(high + '0');
    }
    string message = head + content;
    if (send(socket, message.c_str(), message.length(), 0) == SOCKET_ERROR)
    {
        std::cerr << "Error: Failed to send message" << std::endl;
        exit(1);
    }
    Sleep(10); // wait for 10 millisecond to avoid concatenation of messages
}

string Receive_Message(SOCKET socket)
{
    char message[1024];
    int bytes_received = recv(socket, message, sizeof(message), 0);
    if (bytes_received <= 0)
    {
        std::cerr << "Error: Failed to receive message" << std::endl;
        exit(1);
    }
    return string(message, bytes_received);
    Sleep(10);
}