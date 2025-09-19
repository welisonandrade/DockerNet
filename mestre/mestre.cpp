#include <iostream>
#include <thread>
#include <future>
#include "httplib.h"
#include "json.hpp"

// Usar o namespace para o json
using json = nlohmann::json;

// Função para comunicar com um escravo
std::string contact_slave(const std::string& host, int port, const std::string& endpoint, const std::string& body) {
    httplib::Client cli(host, port);

    // 1. Verificar se o escravo está disponível
    auto health_res = cli.Get("/health");
    if (!health_res || health_res->status != 200) {
        std::cerr << "Erro: Escravo em " << host << ":" << port << " nao esta disponivel." << std::endl;
        return "-1"; // Retorna um valor de erro
    }

    // 2. Enviar a carga de trabalho
    auto res = cli.Post(endpoint, body, "text/plain");
    if (res && res->status == 200) {
        return res->body;
    }
    return "-1"; // Retorna um valor de erro
}


int main() {
    httplib::Server svr;

    // Endpoint que recebe o arquivo do cliente
    svr.Post("/processar", [](const httplib::Request& req, httplib::Response& res) {
        std::string file_content = req.body;

        // Disparar duas tarefas em paralelo usando std::async
        auto future_letras = std::async(std::launch::async, contact_slave, "escravo1", 8081, "/letras", file_content);
        auto future_numeros = std::async(std::launch::async, contact_slave, "escravo2", 8082, "/numeros", file_content);

        // Esperar e obter os resultados
        std::string letras_count_str = future_letras.get();
        std::string numeros_count_str = future_numeros.get();

        // Converter resultados para inteiros
        int letras_count = std::stoi(letras_count_str);
        int numeros_count = std::stoi(numeros_count_str);

        // Combinar a resposta em um JSON
        json result_json;
        result_json["letras"] = letras_count;
        result_json["numeros"] = numeros_count;

        res.set_content(result_json.dump(4), "application/json");
    });

    std::cout << "Servidor Mestre rodando na porta 8080..." << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}