#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        return 1;
    }

    const char* server_ip = argv[1];
    int port = std::stoi(argv[2]);

    // Создание TCP-сокета
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Настройка адреса сервера
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sockfd);
        return 1;
    }

    // Подключение к серверу
    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return 1;
    }

    std::cout << "Connected to the echo server." << std::endl;

    // Ввод сообщения для отправки на сервер
    std::string message;
    std::cout << "Enter message to send: ";
    std::getline(std::cin, message);

    // Отправка сообщения на сервер
    if (send(sockfd, message.c_str(), message.length(), 0) < 0) {
        perror("Send failed");
        close(sockfd);
        return 1;
    }

    // Получение ответа от сервера
    char buffer[BUFFER_SIZE];
    int n = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        return 1;
    }

    buffer[n] = '\0'; // Завершение строки
    std::cout << "Echo from server: " << buffer << std::endl;

    // Закрытие сокета
    close(sockfd);
    return 0;
}
