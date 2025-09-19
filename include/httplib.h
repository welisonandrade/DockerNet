/*
 * httplib.h
 * 
 * A C++11 single-file header-only HTTP library.
 * 
 * This is a simplified version of cpp-httplib for educational purposes.
 * For production use, consider using the original cpp-httplib library.
 */

#ifndef HTTPLIB_H
#define HTTPLIB_H

#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

namespace httplib {

class Request {
public:
    std::string method;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;
    
    std::string get_param_value(const std::string& name) const {
        auto it = headers.find(name);
        return (it != headers.end()) ? it->second : "";
    }
};

class Response {
public:
    int status = 200;
    std::map<std::string, std::string> headers;
    std::string body;
    
    void set_content(const std::string& content, const std::string& content_type = "text/plain") {
        body = content;
        headers["Content-Type"] = content_type;
        headers["Content-Length"] = std::to_string(content.length());
    }
    
    void set_content_json(const std::string& json_content) {
        set_content(json_content, "application/json");
    }
};

class Server {
public:
    using Handler = std::function<void(const Request&, Response&)>;
    
    Server() : port_(0), running_(false) {}
    
    void Get(const std::string& pattern, Handler handler) {
        get_handlers_[pattern] = handler;
    }
    
    void Post(const std::string& pattern, Handler handler) {
        post_handlers_[pattern] = handler;
    }
    
    bool listen(const std::string& host, int port) {
        port_ = port;
        
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == 0) {
            std::cerr << "Socket creation failed" << std::endl;
            return false;
        }
        
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            std::cerr << "setsockopt failed" << std::endl;
            return false;
        }
        
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Bind failed" << std::endl;
            return false;
        }
        
        if (::listen(server_fd, 3) < 0) {
            std::cerr << "Listen failed" << std::endl;
            return false;
        }
        
        std::cout << "Server listening on " << host << ":" << port << std::endl;
        
        running_ = true;
        while (running_) {
            int addrlen = sizeof(address);
            int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
            
            if (client_socket < 0) {
                continue;
            }
            
            std::thread(&Server::handle_client, this, client_socket).detach();
        }
        
        return true;
    }
    
    void stop() {
        running_ = false;
    }
    
private:
    int port_;
    bool running_;
    std::map<std::string, Handler> get_handlers_;
    std::map<std::string, Handler> post_handlers_;
    
    void handle_client(int client_socket) {
        char buffer[4096] = {0};
        int valread = read(client_socket, buffer, 4096);
        
        if (valread > 0) {
            std::string request_str(buffer);
            Request req = parse_request(request_str);
            Response res;
            
            if (req.method == "GET") {
                auto it = get_handlers_.find(req.path);
                if (it != get_handlers_.end()) {
                    it->second(req, res);
                } else {
                    res.status = 404;
                    res.set_content("Not Found");
                }
            } else if (req.method == "POST") {
                auto it = post_handlers_.find(req.path);
                if (it != post_handlers_.end()) {
                    it->second(req, res);
                } else {
                    res.status = 404;
                    res.set_content("Not Found");
                }
            }
            
            send_response(client_socket, res);
        }
        
        close(client_socket);
    }
    
    Request parse_request(const std::string& request_str) {
        Request req;
        std::istringstream stream(request_str);
        std::string line;
        
        // Parse first line
        if (std::getline(stream, line)) {
            std::istringstream line_stream(line);
            line_stream >> req.method >> req.path;
        }
        
        // Parse headers
        while (std::getline(stream, line) && line != "\r" && !line.empty()) {
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                std::string key = line.substr(0, colon_pos);
                std::string value = line.substr(colon_pos + 1);
                // Remove leading whitespace and \r
                value.erase(0, value.find_first_not_of(" \t\r"));
                value.erase(value.find_last_not_of("\r") + 1);
                req.headers[key] = value;
            }
        }
        
        // Parse body (simplified)
        std::string body_line;
        while (std::getline(stream, body_line)) {
            req.body += body_line;
            if (!stream.eof()) {
                req.body += "\n";
            }
        }
        
        return req;
    }
    
    void send_response(int client_socket, const Response& res) {
        std::string response_str = "HTTP/1.1 " + std::to_string(res.status) + " OK\r\n";
        
        for (const auto& header : res.headers) {
            response_str += header.first + ": " + header.second + "\r\n";
        }
        
        response_str += "\r\n" + res.body;
        
        send(client_socket, response_str.c_str(), response_str.length(), 0);
    }
};

} // namespace httplib

#endif // HTTPLIB_H
