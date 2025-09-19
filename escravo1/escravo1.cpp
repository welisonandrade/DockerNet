#include <iostream>
#include <cctype>
#include "httplib.h"

int main() {
    httplib::Server svr;

    // Endpoint de verificação de disponibilidade
    svr.Get("/health", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("OK", "text/plain");
    });

    // Endpoint para contar letras
    svr.Post("/letras", [](const httplib::Request &req, httplib::Response &res) {
        std::string body = req.body;
        long long count = 0;
        for (char c : body) {
            if (isalpha(c)) {
                count++;
            }
        }
        res.set_content(std::to_string(count), "text/plain");
    });

    std::cout << "Servidor Escravo 1 (Letras) rodando na porta 8081..." << std::endl;
    svr.listen("0.0.0.0", 8081);

    return 0;
}