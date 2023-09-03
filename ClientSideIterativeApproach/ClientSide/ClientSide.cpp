#include <stdio.h>
#include <stdlib.h>
#include <String>
#include <pthread.h>
#include<iostream>
#include <windows.h>
#include<winsock.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct sockaddr_in server_addr;  // the address we want to connect to
int client_socket;

int main() {

    int nRet = 0;
    char buffer[1024];
    int n;

    //to provide env for socket programming in windows
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
        cout << endl << "WSA failed!";
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "WSA successfull!";
    }

    //create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (client_socket < 0) {
        cout << endl << "The client socket is not opened!";
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "The client socket opened successfully!";
    }

    //Connect
    memset(&server_addr, 0, sizeof(server_addr)); //to set all the socket structures with null values
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    nRet = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (client_socket == SOCKET_ERROR) {
        cout << endl << "Connection failed!";
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "Connection has been established!";
    }

    while (true)
    {
        memset(&buffer, 0, sizeof(buffer));
        cout << endl << "YOU: ";
        string data;
        getline(cin, data);
        
        strcpy_s(buffer, data.c_str());  //c_str -- converts string to char array with null terminator
        if (data == "exit")
        {
            //send to the client that server has closed the connection
            send(client_socket, buffer, strlen(buffer), 0);
            break;
        }

        send(client_socket, buffer, strlen(buffer), 0);
        
        memset(&buffer, 0, sizeof(buffer));
        n = recv(client_socket, buffer, sizeof(buffer), 0);

        if (!strcmp(buffer, "exit"))
        {
            cout << "Server has quit the session" << endl;
            break;
        }

        buffer[n] = '\0';
        cout << endl << "SERVER: " << buffer << endl;
    }
        

    //close
    closesocket(client_socket);
    cout << endl << "Socket closed" ;

    return 0;
}
