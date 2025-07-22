#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

class SimpleServer {
private:
    SOCKET listen_socket_;
    std::atomic<bool> running_;
    std::vector<std::thread> worker_threads_;
    int port_;

public:
    SimpleServer(int port = 8080) : listen_socket_(INVALID_SOCKET), running_(false), port_(port) {}

    ~SimpleServer() {
        Stop();
    }

    bool Start() {
        // Initialize Winsock
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            std::cerr << "Failed to initialize Winsock" << std::endl;
            return false;
        }

        // Create socket
        listen_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listen_socket_ == INVALID_SOCKET) {
            std::cerr << "Failed to create socket" << std::endl;
            WSACleanup();
            return false;
        }

        // Set socket options
        BOOL reuse = TRUE;
        if (setsockopt(listen_socket_, SOL_SOCKET, SO_REUSEADDR, 
                      reinterpret_cast<char*>(&reuse), sizeof(reuse)) == SOCKET_ERROR) {
            std::cerr << "Failed to set SO_REUSEADDR" << std::endl;
            closesocket(listen_socket_);
            WSACleanup();
            return false;
        }

        // Bind socket
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port_);
        server_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(listen_socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
            std::cerr << "Failed to bind socket" << std::endl;
            closesocket(listen_socket_);
            WSACleanup();
            return false;
        }

        // Listen for connections
        if (listen(listen_socket_, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Failed to listen" << std::endl;
            closesocket(listen_socket_);
            WSACleanup();
            return false;
        }

        running_ = true;
        std::cout << "Simple server started on port " << port_ << std::endl;

        // Start worker threads
        int thread_count = std::thread::hardware_concurrency();
        for (int i = 0; i < thread_count; ++i) {
            worker_threads_.emplace_back(&SimpleServer::WorkerThread, this);
        }

        return true;
    }

    void Stop() {
        if (!running_) return;

        running_ = false;

        // Close listen socket to wake up accept calls
        if (listen_socket_ != INVALID_SOCKET) {
            closesocket(listen_socket_);
            listen_socket_ = INVALID_SOCKET;
        }

        // Wait for worker threads
        for (auto& thread : worker_threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        WSACleanup();
        std::cout << "Server stopped" << std::endl;
    }

private:
    void WorkerThread() {
        while (running_) {
            sockaddr_in client_addr;
            int addr_len = sizeof(client_addr);

            SOCKET client_socket = accept(listen_socket_, reinterpret_cast<sockaddr*>(&client_addr), &addr_len);
            if (client_socket == INVALID_SOCKET) {
                if (running_) {
                    std::cerr << "Accept failed" << std::endl;
                }
                break;
            }

            // Handle client in this thread
            HandleClient(client_socket);
        }
    }

    void HandleClient(SOCKET client_socket) {
        char buffer[1024];
        
        while (running_) {
            // Receive data
            int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (received <= 0) {
                break;
            }

            buffer[received] = '\0';
            std::string request(buffer);

            // Generate response
            std::string response = "respuesta";

            // Send response
            int sent = send(client_socket, response.c_str(), response.length(), 0);
            if (sent <= 0) {
                break;
            }
        }

        closesocket(client_socket);
    }
};

int main() {
    SimpleServer server(8080);
    
    if (!server.Start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    std::cout << "Server running. Press Enter to stop..." << std::endl;
    std::cin.get();

    server.Stop();
    return 0;
} 