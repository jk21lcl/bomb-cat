#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>

using namespace std;

enum MessageType
{
    no_response, // output the content
    response_int, // output the content and input an integer
    response_str, // output the content and input a string
    close // close the socket, no output
};

enum Color
{
    white,
    dark_red,
    light_red,
    dark_green,
    light_green,
    dark_yellow,
    light_yellow,
    dark_blue,
    light_blue,
    dark_pink,
    light_pink,
    dark_cyan,
    light_cyan
};

class Message
{
    public:
        // 1st encodes type, 2nd encodes color, 3rd/4th encodes low/high
        // tail bit $ as delimiter
        MessageType type;
        Color color;
        int low; // the lower bound of response integer
        int high; // the upper bound of response integer
        string content;

        Message(MessageType t = no_response, string con = "", Color c = white, int l = 0, int h = 0);
        ~Message() {}

        void Send(SOCKET socket) const;
};

void InputInteger(int m, int n, int* out);
void OutputColor(string content, Color color);

vector<Message> ReceiveMessage(SOCKET socket);