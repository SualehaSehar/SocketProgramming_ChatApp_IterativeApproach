#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include<iostream>
#include <windows.h>
#include<winsock.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")
using std::cout;
using std::endl;
using std::cin;
using std::string;

struct sockaddr_in server_addr,client_addr;
int server_socket, client_socket, client_addr_len, recv_size;
char buffer[1024];
int main() {

    //to provide env for socket programming in windows
    WSADATA ws;
    int n;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
        cout << endl << "WSA failed!";
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "WSA successfull!";
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0); // 0 USES UNDERLINK PROTOCOL (MEANS WHAT MY SYSTEM'S UNDERLINK FRAMEWORK IS USING)
    if (server_socket == INVALID_SOCKET) {
        cout << endl << "The socket is not opened!";
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "The socket opened successfully!";
    }

    //binding
    memset(&server_addr,0,sizeof(server_addr)); //to set all the socket structures with null values
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // my system is server so it will pick my system's ip address 

    int binding = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (binding < 0 ) {
        cout << endl << "Binding to local port failed!";
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "Binding to local port successfull!";
    }

    //listening
    int listening = listen(server_socket, 5); //more than 5 clients will go to waiting queue and thes 5 clients will be in active queue
    if (listening < 0) {
        cout << endl << "listening to local port failed!";
        exit(EXIT_FAILURE);
    }
    else {
        cout << endl << "Started listening to local port...";
    }

    for (int i = 0; i < 5; i++) {
        //accepting
        memset(&client_addr, 0, sizeof(client_addr));
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            cout << endl << "Connection failed!";
            continue;
        }
        else {
            cout << endl << "Connection has been established!";
        }

        while (true)
        {
            //writing

            memset(&buffer, 0, sizeof(buffer)); //clear the buffer
            n = recv(client_socket, buffer, sizeof(buffer), 0);

            if (!strcmp(buffer, "exit"))
            {
                cout << "Client has quit the session" << endl;
                break;
            }

            buffer[n] = '\0';
            cout << endl << "CLIENT: " << buffer << endl;

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
        }

        //close
        closesocket(client_socket);
        cout << endl << "Started listening to local port...";
    }
    closesocket(server_socket);
    cout << endl << "Socket closed" ;

    return 0;

    //string s = "Hello";
    //const char* msg = s.c_str();
    //send(client_socket, msg, strlen(msg), 0);
}
