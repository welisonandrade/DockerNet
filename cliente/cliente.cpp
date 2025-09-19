#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "httplib.h"

// Função para ler todo o conteúdo de um arquivo
std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo: " + path);
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <caminho_do_arquivo.txt>" << std::endl;
        return 1;
    }

    try {
        std::string file_path = argv[1];
        std::string file_content = read_file(file_path);

        // Cliente se conecta ao Mestre que está em localhost:8080
        httplib::Client cli("localhost", 8080);
        cli.set_connection_timeout(5, 0); // 5 segundos de timeout

        std::cout << "Enviando conteudo do arquivo para o servidor Mestre..." << std::endl;

        // Envia a requisição HTTP para o Mestre
        auto res = cli.Post("/processar", file_content, "text/plain");

        if (res) {
            if (res->status == 200) {
                std::cout << "\n>> Resultado recebido do Mestre:\n" << res->body << std::endl;
            } else {
                std::cerr << "O servidor retornou um erro: " << res->status << std::endl;
            }
        } else {
            auto err = res.error();
            std::cerr << "Erro na comunicacao HTTP: " << httplib::to_string(err) << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Ocorreu um erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}