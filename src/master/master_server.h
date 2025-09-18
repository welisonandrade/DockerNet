#ifndef MASTER_SERVER_H
#define MASTER_SERVER_H

#include <string>
#include <future>
#include <vector>
#include <memory>

struct SlaveResult {
    bool success;
    std::string response;
    std::string error;
};

class MasterServer {
public:
    static SlaveResult checkSlaveHealth(const std::string& host, int port);
    static SlaveResult sendToSlave(const std::string& host, int port, const std::string& endpoint, const std::string& data);
    static std::string processTextFile(const std::string& fileContent);
    static std::string combineResults(const SlaveResult& lettersResult, const SlaveResult& numbersResult);
};

#endif // MASTER_SERVER_H
