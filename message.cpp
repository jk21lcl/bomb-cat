#include "message.h"

Message::Message(MessageType t, string con, Color c, int l, int h)
{
    type = t;
    content = con;
    color = c;
    low = l;
    high = h;
}

void Message::Send(SOCKET socket) const
{
    string head;
    head.push_back((int)type + '0');
    head.push_back((int)color + '0');
    head.push_back(low + '0');
    head.push_back(high + '0');
    string message = head + content;
    message.push_back('$');
    if (send(socket, message.c_str(), message.length(), 0) == SOCKET_ERROR)
    {
        std::cout << "Error: Failed to send message" << std::endl;
        exit(1);
    }
}

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

void OutputColor(string content, Color color)
{
    switch (color)
    {
        case white:
            break;
        case dark_red:
            cout << "\e[0;31m";
            break;
        case light_red:
            cout << "\e[1;31m";
            break;
        case dark_green:
            cout << "\e[0;32m";
            break;
        case light_green:
            cout << "\e[1;32m";
            break;
        case dark_yellow:
            cout << "\e[0;33m";
            break;
        case light_yellow:
            cout << "\e[1;33m";
            break;
        case dark_blue:
            cout << "\e[0;34m";
            break;
        case light_blue:
            cout << "\e[1;34m";
            break;
        case dark_pink:
            cout << "\e[0;35m";
            break;
        case light_pink:
            cout << "\e[1;35m";
            break;
        case dark_cyan:
            cout << "\e[0;36m";
            break;
        case light_cyan:
            cout << "\e[1;36m";
            break;
        default:
            cout << "IMPOSSBLE!" << endl;
    }
    cout << content << "\e[0m" << endl;
}

vector<Message> ReceiveMessage(SOCKET socket)
{
    char message[1024];
    int bytes_received = recv(socket, message, sizeof(message), 0);
    if (bytes_received <= 0)
    {
        std::cout << "Error: Failed to receive message" << std::endl;
        exit(1);
    }
    string raw_message(message, bytes_received);

    vector<string> result;
    size_t start = 0;
    size_t end = 0;
    while ((end = raw_message.find('$', start)) != string::npos)
    {
        result.push_back(raw_message.substr(start, end - start));
        start = end + 1;
    }

    vector<Message> messages;
    for (string message : result)
    {
        MessageType type = MessageType(message[0] - '0');
        Color color = Color(message[1] - '0');
        int low = message[2] - '0';
        int high = message[3] - '0';
        string content = message.substr(4);
        messages.push_back(Message(type, content, color, low, high));
    }
    return messages;
}