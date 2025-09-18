#include <iostream>
#include <thread>
#include <chrono>
#include "../../include/httplib.h"
#include "slave_numeros.h"

int main() {
    httplib::Server server;
    
    // Health check endpoint
    server.Get("/health", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content_json("{\"status\": \"ok\", \"service\": \"slave_numeros\"}");
    });
    
    // Numbers counting endpoint
    server.Post("/numeros", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Received request to count numbers" << std::endl;
        std::cout << "Input text: " << req.body << std::endl;
        
        std::string result = SlaveNumeros::processRequest(req.body);
        res.set_content_json(result);
        
        std::cout << "Response: " << result << std::endl;
    });
    
    std::cout << "Starting Slave Numeros server on port 8082..." << std::endl;
    
    if (!server.listen("0.0.0.0", 8082)) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    
    return 0;
}
