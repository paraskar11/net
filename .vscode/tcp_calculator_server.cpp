// TCP Calculator Server in C++ (Windows)
#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>

#pragma comment(lib, "ws2_32.lib") // Link with Ws2_32.lib

double evaluateExpression(const std::string &expression) {
    std::istringstream iss(expression);
    double num1, num2;
    char op;
    iss >> num1 >> op >> num2; // Read the first number, operator, and second number

    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': 
            if (num2 != 0) return num1 / num2;
            else throw std::runtime_error("Division by zero!");
        default: throw std::runtime_error("Invalid operator!");
    }
}

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);
    char buffer[1024];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // Setup the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080); // Port 8080

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Start listening for connections
    if (listen(server_socket, 1) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Calculator Server is waiting for a connection on port 8080..." << std::endl;

    // Accept a connection
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Process client requests
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break; // Break on error or disconnect

        std::string expression(buffer);
        try {
            double result = evaluateExpression(expression);
            std::ostringstream oss;
            oss << result;
            std::string resultStr = oss.str();
            send(client_socket, resultStr.c_str(), resultStr.length(), 0);
        } catch (const std::exception &e) {
            std::string errorMsg = e.what();
            send(client_socket, errorMsg.c_str(), errorMsg.length(), 0);
        }
    }

    // Clean up
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
