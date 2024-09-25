// TCP File Transfer Client in C++ (Windows)
#include <iostream>
#include <fstream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib") // Link with Ws2_32.lib

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    // Setup the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Connection to server failed" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Open the file to be sent
    ifstream infile("file_to_send.txt", ios::binary);
    if (!infile.is_open()) {
        cerr << "Failed to open file" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Read the file and send it to the server
    while (!infile.eof()) {
        infile.read(buffer, sizeof(buffer));
        int bytesToSend = infile.gcount();
        send(sock, buffer, bytesToSend, 0);
    }

    cout << "File sent successfully!" << endl;

    // Clean up
    infile.close();
    closesocket(sock);
    WSACleanup();

    return 0;
}
