// TCP File Transfer Server in C++ (Windows)
#include <iostream>
#include <fstream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib") // Link with Ws2_32.lib

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);
    char buffer[1024];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    // Setup the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080); // Port 8080

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Bind failed" << endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Start listening for connections
    if (listen(server_socket, 1) == SOCKET_ERROR) {
        cerr << "Listen failed" << endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    cout << "File Transfer Server is waiting for a connection on port 8080..." << endl;

    // Accept a connection
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket == INVALID_SOCKET) {
        cerr << "Accept failed" << endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Open a file to save the received data
    ofstream outfile("received_file.txt", ios::binary);
    if (!outfile.is_open()) {
        cerr << "Failed to open file for writing" << endl;
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Receive the file data from the client
    int bytesRead;
    while ((bytesRead = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        outfile.write(buffer, bytesRead);
    }

    cout << "File received successfully!" << endl;

    // Clean up
    outfile.close();
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
