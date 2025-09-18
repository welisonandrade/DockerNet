#include "master_server.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

SlaveResult MasterServer::checkSlaveHealth(const std::string& host, int port) {
    SlaveResult result;
    result.success = false;
    
    // Simple HTTP client implementation
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        result.error = "Socket creation failed";
        return result;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        result.error = "Invalid address";
        close(sock);
        return result;
    }
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        result.error = "Connection failed";
        close(sock);
        return result;
    }
    
    // Send HTTP GET request
    std::string request = "GET /health HTTP/1.1\r\n"
                         "Host: " + host + "\r\n"
                         "Connection: close\r\n\r\n";
    
    send(sock, request.c_str(), request.length(), 0);
    
    // Read response
    char buffer[4096];
    std::string response;
    int bytes_read;
    while ((bytes_read = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_read] = '\0';
        response += buffer;
    }
    
    close(sock);
    
    // Simple response parsing
    if (response.find("200 OK") != std::string::npos) {
        result.success = true;
        result.response = response;
    } else {
        result.error = "Health check failed";
    }
    
    return result;
}

SlaveResult MasterServer::sendToSlave(const std::string& host, int port, const std::string& endpoint, const std::string& data) {
    SlaveResult result;
    result.success = false;
    
    // First check health
    SlaveResult healthResult = checkSlaveHealth(host, port);
    if (!healthResult.success) {
        result.error = "Slave not available: " + healthResult.error;
        return result;
    }
    
    // Simple HTTP client implementation
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        result.error = "Socket creation failed";
        return result;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        result.error = "Invalid address";
        close(sock);
        return result;
    }
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        result.error = "Connection failed";
        close(sock);
        return result;
    }
    
    // Send HTTP POST request
    std::string request = "POST " + endpoint + " HTTP/1.1\r\n"
                         "Host: " + host + "\r\n"
                         "Content-Type: application/json\r\n"
                         "Content-Length: " + std::to_string(data.length()) + "\r\n"
                         "Connection: close\r\n\r\n" + data;
    
    send(sock, request.c_str(), request.length(), 0);
    
    // Read response
    char buffer[4096];
    std::string response;
    int bytes_read;
    while ((bytes_read = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_read] = '\0';
        response += buffer;
    }
    
    close(sock);
    
    // Simple response parsing
    if (response.find("200 OK") != std::string::npos) {
        result.success = true;
        // Extract JSON from response
        size_t json_start = response.find("\r\n\r\n");
        if (json_start != std::string::npos) {
            result.response = response.substr(json_start + 4);
        } else {
            result.response = response;
        }
    } else {
        result.error = "Request failed";
    }
    
    return result;
}

std::string MasterServer::processTextFile(const std::string& fileContent) {
    std::cout << "Processing text file with " << fileContent.length() << " characters" << std::endl;
    
    // Start two threads to communicate with slaves in parallel
    auto lettersFuture = std::async(std::launch::async, [&fileContent]() {
        return MasterServer::sendToSlave("slave_letras", 8081, "/letras", fileContent);
    });
    
    auto numbersFuture = std::async(std::launch::async, [&fileContent]() {
        return MasterServer::sendToSlave("slave_numeros", 8082, "/numeros", fileContent);
    });
    
    // Wait for both results
    SlaveResult lettersResult = lettersFuture.get();
    SlaveResult numbersResult = numbersFuture.get();
    
    return combineResults(lettersResult, numbersResult);
}

std::string MasterServer::combineResults(const SlaveResult& lettersResult, const SlaveResult& numbersResult) {
    std::string result = "{\n";
    
    if (lettersResult.success && numbersResult.success) {
        result += "  \"status\": \"success\",\n";
        result += "  \"letters_result\": " + lettersResult.response + ",\n";
        result += "  \"numbers_result\": " + numbersResult.response + "\n";
    } else {
        result += "  \"status\": \"error\",\n";
        if (!lettersResult.success) {
            result += "  \"letters_error\": \"" + lettersResult.error + "\",\n";
        } else {
            result += "  \"letters_result\": " + lettersResult.response + ",\n";
        }
        if (!numbersResult.success) {
            result += "  \"numbers_error\": \"" + numbersResult.error + "\"\n";
        } else {
            result += "  \"numbers_result\": " + numbersResult.response + "\n";
        }
    }
    
    result += "}";
    
    return result;
}
