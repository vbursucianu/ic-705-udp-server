#include <iostream>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int BUFFER_SIZE = 4096;

int sock;  // Declare socket as a global variable for access in the signal handler

void signalHandler(int signum) {
    std::cerr << "Received signal: " << signum << ". Closing socket and exiting." << std::endl;
    close(sock);
    exit(signum);
}

void printHexPayload(const char* payload, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(payload[i])) << " ";
    }
    std::cout << std::dec << std::endl;
}

int main(int argc, char *argv[]) {
#if 0    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return -1;
    }
    int udpPort = std::stoi(argv[1]);
#else
    int udpPort = 50004;
#endif
    // Set up signal handler
    signal(SIGINT, signalHandler);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Bind to a specific UDP port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(udpPort);

    std::cout << "Binding to udp socket " << udpPort << std::endl;
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(sock);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    while (true) {
#if 0
        // Receive UDP packet
        ssize_t packet_size = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_size);
        if (packet_size == -1) {
            std::cerr << "Error receiving packet" << std::endl;
            close(sock);
            return -1;
        }

        // Print information
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
        std::cout << "Source IP: " << ip_str << std::endl;
        std::cout << "Source Port: " << ntohs(client_addr.sin_port) << std::endl;
        std::cout << "Payload Size: " << packet_size << " bytes" << std::endl;
        std::cout << "Payload (Hex): ";
        printHexPayload(buffer, packet_size);
        std::cout << std::endl;
#else
        const char* message = "Hello, Wireshark!";
        sendto(sock, message, strlen(message), 0, (struct sockaddr*)&client_addr, addr_size);

        std::cout << "Sent message: " << message << std::endl;

        // Simulate some delay between transmissions
        sleep(1);
#endif
    }

    return 0;
}
