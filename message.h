#pragma once

#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

// the first char encodes the type of message
enum MessageType
{
    no_response, // output the content
    response_int, // the 2nd/3rd char encodes the lower/upper bound, no output
    close // close the socket, no output
};

void InputInteger(int m, int n, int* out);

void Send_Message(MessageType type, SOCKET socket, string content, int low = 0, int high = 0);
string Receive_Message(SOCKET socket);