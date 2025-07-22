#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")

class SimpleClient {
private:
    SOCKET socket_;
    std::string host_;
    int port_;
    bool connected_;

public:
    SimpleClient(const std::string& host = "127.0.0.1", int port = 8080) 
        : socket_(INVALID_SOCKET), host_(host), port_(port), connected_(false) {}

    ~SimpleClient() {
        Disconnect();
    }

    bool Connect() {
        // Initialize Winsock
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            std::cerr << "Failed to initialize Winsock" << std::endl;
            return false;
        }

        // Create socket
        socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket_ == INVALID_SOCKET) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        // Connect to server
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port_);
        server_addr.sin_addr.s_addr = inet_addr(host_.c_str());

        if (connect(socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
            std::cerr << "Failed to connect to server" << std::endl;
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
            return false;
        }

        connected_ = true;
        std::cout << "Connected to server at " << host_ << ":" << port_ << std::endl;
        return true;
    }

    void Disconnect() {
        if (socket_ != INVALID_SOCKET) {
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
        }
        connected_ = false;
    }

    struct LatencyResult {
        bool success;
        uint64_t latency_ns;
        std::string response;
        std::string error_message;
    };

    LatencyResult SendRequest(const std::string& request, const std::string& expected_response = "") {
        LatencyResult result = {false, 0, "", ""};
        
        if (!connected_) {
            result.error_message = "Not connected to server";
            return result;
        }

        // Get high-resolution start time
        auto start_time = std::chrono::high_resolution_clock::now();

        // Send request
        if (!SendData(request)) {
            result.error_message = "Failed to send request";
            return result;
        }

        // Receive response
        std::string response;
        if (!ReceiveData(response)) {
            result.error_message = "Failed to receive response";
            return result;
        }

        // Calculate latency
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        result.latency_ns = duration.count();

        // Check if response matches expected
        if (!expected_response.empty() && response != expected_response) {
            result.error_message = "Response mismatch. Expected: " + expected_response + ", Got: " + response;
            return result;
        }

        result.success = true;
        result.response = response;
        return result;
    }

    void RunBatchTest(const std::string& request, const std::string& expected_response, int request_count) {
        std::cout << "\n=== Ultra-Low Latency Client ===" << std::endl;
        std::cout << "Testing sub-millisecond response times\n" << std::endl;

        std::cout << "Client Configuration:" << std::endl;
        std::cout << "  Server: " << host_ << ":" << port_ << std::endl;
        std::cout << "  Request: \"" << request << "\"" << std::endl;
        std::cout << "  Expected Response: \"" << expected_response << "\"" << std::endl;
        std::cout << "  Test Type: Batch" << std::endl;
        std::cout << "  Requests: " << request_count << std::endl;
        std::cout << "  Concurrent Connections: 1\n" << std::endl;

        if (!Connect()) {
            std::cerr << "Failed to connect to server" << std::endl;
            return;
        }

        std::cout << "Running batch test with " << request_count << " requests...\n" << std::endl;

        std::vector<LatencyResult> results;
        results.reserve(request_count);

        auto test_start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < request_count; ++i) {
            results.push_back(SendRequest(request, expected_response));
        }

        auto test_end = std::chrono::high_resolution_clock::now();
        auto test_duration = std::chrono::duration_cast<std::chrono::milliseconds>(test_end - test_start);

        // Calculate statistics
        int successful = 0;
        int failed = 0;
        std::vector<uint64_t> latencies;

        for (const auto& result : results) {
            if (result.success) {
                successful++;
                latencies.push_back(result.latency_ns);
            } else {
                failed++;
            }
        }

        // Print results
        std::cout << "=== Test Results ===" << std::endl;
        std::cout << "Test Duration: " << test_duration.count() << " ms" << std::endl;
        std::cout << "Total Requests: " << request_count << std::endl;
        std::cout << "Successful: " << successful << std::endl;
        std::cout << "Failed: " << failed << std::endl;
        std::cout << "Success Rate: " << std::fixed << std::setprecision(2) 
                  << (successful * 100.0 / request_count) << "%\n" << std::endl;

        if (!latencies.empty()) {
            std::sort(latencies.begin(), latencies.end());
            
            uint64_t min_latency = latencies.front();
            uint64_t max_latency = latencies.back();
            uint64_t total_latency = 0;
            for (uint64_t latency : latencies) {
                total_latency += latency;
            }
            uint64_t avg_latency = total_latency / latencies.size();

            std::cout << "Latency Statistics:" << std::endl;
            std::cout << "  Min: " << std::fixed << std::setprecision(3) << (min_latency / 1000.0) << " μs" << std::endl;
            std::cout << "  Max: " << std::fixed << std::setprecision(3) << (max_latency / 1000.0) << " μs" << std::endl;
            std::cout << "  Avg: " << std::fixed << std::setprecision(3) << (avg_latency / 1000.0) << " μs" << std::endl;
            std::cout << "  P50: " << std::fixed << std::setprecision(3) << (latencies[latencies.size() * 50 / 100] / 1000.0) << " μs" << std::endl;
            std::cout << "  P95: " << std::fixed << std::setprecision(3) << (latencies[latencies.size() * 95 / 100] / 1000.0) << " μs" << std::endl;
            std::cout << "  P99: " << std::fixed << std::setprecision(3) << (latencies[latencies.size() * 99 / 100] / 1000.0) << " μs" << std::endl;
            std::cout << "  P99.9: " << std::fixed << std::setprecision(3) << (latencies[latencies.size() * 999 / 1000] / 1000.0) << " μs\n" << std::endl;

            std::cout << "Performance Metrics:" << std::endl;
            double throughput = (successful * 1000.0) / test_duration.count();
            std::cout << "  Throughput: " << std::fixed << std::setprecision(1) << throughput << " req/sec\n" << std::endl;

            if (min_latency < 1000000) { // Less than 1ms
                std::cout << "🎯 ACHIEVED SUB-MILLISECOND LATENCY!" << std::endl;
                std::cout << "  Best latency: " << std::fixed << std::setprecision(3) << (min_latency / 1000.0) << " μs" << std::endl;
            }
        }

        std::cout << "Test completed." << std::endl;
        Disconnect();
    }

private:
    bool SendData(const std::string& data) {
        int total_sent = 0;
        int data_size = static_cast<int>(data.length());

        while (total_sent < data_size) {
            int sent = send(socket_, data.c_str() + total_sent, data_size - total_sent, 0);
            if (sent == SOCKET_ERROR) {
                return false;
            }
            total_sent += sent;
        }

        return true;
    }

    bool ReceiveData(std::string& data) {
        char buffer[1024];
        data.clear();

        while (true) {
            int received = recv(socket_, buffer, sizeof(buffer), 0);
            if (received == SOCKET_ERROR) {
                return false;
            }
            if (received == 0) {
                break; // Connection closed
            }
            data.append(buffer, received);
            
            // Simple protocol: assume response is complete when we receive it
            break;
        }

        return true;
    }
};

int main(int argc, char* argv[]) {
    std::string request = "test";
    std::string expected_response = "respuesta";
    int request_count = 5;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--request" && i + 1 < argc) {
            request = argv[++i];
        } else if (arg == "--expected" && i + 1 < argc) {
            expected_response = argv[++i];
        } else if (arg == "--requests" && i + 1 < argc) {
            request_count = std::stoi(argv[++i]);
        }
    }

    SimpleClient client;
    client.RunBatchTest(request, expected_response, request_count);

    return 0;
} 