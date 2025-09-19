#include <iostream>
#include <cctype>
#include "httplib.h"

int main() {
    httplib::Server svr;

    // Endpoint de verificação de disponibilidade
    svr.Get("/health", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("OK", "text/plain");
    });

    // Endpoint para contar números
    svr.Post("/numeros", [](const httplib::Request &req, httplib::Response &res) {
        std::string body = req.body;
        long long count = 0;
        for (char c : body) {
            if (isdigit(c)) {
                count++;
            }
        }
        res.set_content(std::to_string(count), "text/plain");
    });

    std::cout << "Servidor Escravo 2 (Numeros) rodando na porta 8082..." << std::endl;
    svr.listen("0.0.0.0", 8082);

    return 0;
}