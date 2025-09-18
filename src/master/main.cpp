#include <iostream>
#include <fstream>
#include <sstream>
#include <future>
#include "../../include/httplib.h"
#include "master_server.h"

std::string readFileContent(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}

int main() {
    httplib::Server server;
    
    // Health check endpoint
    server.Get("/health", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content_json("{\"status\": \"ok\", \"service\": \"master\"}");
    });
    
    // Main processing endpoint
    server.Post("/process", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Master received processing request" << std::endl;
        std::cout << "Request body: " << req.body << std::endl;
        
        try {
            std::string result = MasterServer::processTextFile(req.body);
            res.set_content_json(result);
            std::cout << "Processing completed successfully" << std::endl;
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content_json("{\"error\": \"" + std::string(e.what()) + "\"}");
            std::cerr << "Error processing request: " << e.what() << std::endl;
        }
    });
    
    // File upload endpoint
    server.Post("/upload", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Master received file upload request" << std::endl;
        
        try {
            std::string result = MasterServer::processTextFile(req.body);
            res.set_content_json(result);
            std::cout << "File processing completed successfully" << std::endl;
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content_json("{\"error\": \"" + std::string(e.what()) + "\"}");
            std::cerr << "Error processing file: " << e.what() << std::endl;
        }
    });
    
    std::cout << "Starting Master server on port 8080..." << std::endl;
    std::cout << "Available endpoints:" << std::endl;
    std::cout << "  GET  /health - Health check" << std::endl;
    std::cout << "  POST /process - Process text content" << std::endl;
    std::cout << "  POST /upload - Upload and process file content" << std::endl;
    
    if (!server.listen("0.0.0.0", 8080)) {
        std::cerr << "Failed to start Master server" << std::endl;
        return 1;
    }
    
    return 0;
}
