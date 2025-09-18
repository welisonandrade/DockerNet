#ifndef CLIENT_APP_H
#define CLIENT_APP_H

#include <string>

class ClientApp {
public:
    static std::string readTextFile(const std::string& filename);
    static std::string sendToMaster(const std::string& host, int port, const std::string& endpoint, const std::string& data);
    static void displayResults(const std::string& jsonResponse);
    static void runInteractiveMode(const std::string& masterHost = "localhost", int masterPort = 8080);
};

#endif // CLIENT_APP_H
