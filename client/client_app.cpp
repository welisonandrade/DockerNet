#include "client_app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

std::string ClientApp::readTextFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}

std::string ClientApp::sendToMaster(const std::string& host, int port, const std::string& endpoint, const std::string& data) {
    // Simple HTTP client implementation
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Socket creation failed");
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (host == "localhost") {
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    } else if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address: " + host);
    }
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Connection failed to " + host + ":" + std::to_string(port));
    }
    
    // Send HTTP POST request
    std::string request = "POST " + endpoint + " HTTP/1.1\r\n"
                         "Host: " + host + "\r\n"
                         "Content-Type: text/plain\r\n"
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
    
    // Extract JSON from response
    size_t json_start = response.find("\r\n\r\n");
    if (json_start != std::string::npos) {
        return response.substr(json_start + 4);
    }
    
    return response;
}

void ClientApp::displayResults(const std::string& jsonResponse) {
    std::cout << "\n=== RESULTADO DO PROCESSAMENTO ===" << std::endl;
    std::cout << jsonResponse << std::endl;
    std::cout << "====================================\n" << std::endl;
}

void ClientApp::runInteractiveMode(const std::string& masterHost, int masterPort) {
    std::cout << "=== CLIENTE DO SISTEMA DISTRIBUÍDO ===" << std::endl;
    std::cout << "Conectando ao Mestre em " << masterHost << ":" << masterPort << std::endl;
    std::cout << "Digite 'sair' para encerrar o programa\n" << std::endl;
    
    while (true) {
        std::cout << "Escolha uma opção:" << std::endl;
        std::cout << "1. Enviar arquivo de texto" << std::endl;
        std::cout << "2. Enviar texto diretamente" << std::endl;
        std::cout << "3. Verificar status do mestre" << std::endl;
        std::cout << "4. Sair" << std::endl;
        std::cout << "Opção: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            std::cout << "Digite o caminho do arquivo: ";
            std::string filename;
            std::getline(std::cin, filename);
            
            if (filename == "sair") {
                break;
            }
            
            try {
                std::string fileContent = readTextFile(filename);
                std::cout << "Arquivo carregado com sucesso (" << fileContent.length() << " caracteres)" << std::endl;
                
                std::string result = sendToMaster(masterHost, masterPort, "/upload", fileContent);
                displayResults(result);
                
            } catch (const std::exception& e) {
                std::cout << "Erro: " << e.what() << std::endl;
            }
            
        } else if (choice == "2") {
            std::cout << "Digite o texto a ser processado:" << std::endl;
            std::string text;
            std::getline(std::cin, text);
            
            if (text == "sair") {
                break;
            }
            
            try {
                std::string result = sendToMaster(masterHost, masterPort, "/process", text);
                displayResults(result);
                
            } catch (const std::exception& e) {
                std::cout << "Erro: " << e.what() << std::endl;
            }
            
        } else if (choice == "3") {
            try {
                std::string result = sendToMaster(masterHost, masterPort, "/health", "");
                std::cout << "Status do Mestre: " << result << std::endl;
                
            } catch (const std::exception& e) {
                std::cout << "Erro ao verificar status: " << e.what() << std::endl;
            }
            
        } else if (choice == "4" || choice == "sair") {
            break;
            
        } else {
            std::cout << "Opção inválida. Tente novamente." << std::endl;
        }
    }
    
    std::cout << "Encerrando cliente..." << std::endl;
}
