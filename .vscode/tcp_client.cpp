// TCP Hello Client in C++ (Windows)
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link with Ws2_32.lib

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // Setup the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Receive message from the server
    char buffer[1024] = {0};
    recv(sock, buffer, sizeof(buffer), 0);
    std::cout << "Server says: " << buffer << std::endl;

    // Send a response to the server
    const char* hiMsg = "Hi from Client!";
    send(sock, hiMsg, strlen(hiMsg), 0);

    // Clean up
    closesocket(sock);
    WSACleanup();

    return 0;
}