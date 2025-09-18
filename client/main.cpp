#include <iostream>
#include <string>
#include "../include/httplib.h"
#include "client_app.h"

void showUsage() {
    std::cout << "Uso: ./client [opções]" << std::endl;
    std::cout << "Opções:" << std::endl;
    std::cout << "  -h, --host HOST    Host do servidor mestre (padrão: localhost)" << std::endl;
    std::cout << "  -p, --port PORT    Porta do servidor mestre (padrão: 8080)" << std::endl;
    std::cout << "  -f, --file FILE    Processar arquivo específico" << std::endl;
    std::cout << "  -t, --text TEXT    Processar texto específico" << std::endl;
    std::cout << "  -i, --interactive  Modo interativo (padrão)" << std::endl;
    std::cout << "  --help             Mostrar esta ajuda" << std::endl;
    std::cout << std::endl;
    std::cout << "Exemplos:" << std::endl;
    std::cout << "  ./client                           # Modo interativo" << std::endl;
    std::cout << "  ./client -f arquivo.txt            # Processar arquivo" << std::endl;
    std::cout << "  ./client -t \"Hello World 123\"      # Processar texto" << std::endl;
    std::cout << "  ./client -h 192.168.1.100 -p 8080 # Conectar a servidor específico" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string masterHost = "localhost";
    int masterPort = 8080;
    std::string filename;
    std::string text;
    bool interactive = true;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--host") {
            if (i + 1 < argc) {
                masterHost = argv[++i];
            } else {
                std::cerr << "Erro: --host requer um valor" << std::endl;
                return 1;
            }
        } else if (arg == "-p" || arg == "--port") {
            if (i + 1 < argc) {
                masterPort = std::stoi(argv[++i]);
            } else {
                std::cerr << "Erro: --port requer um valor" << std::endl;
                return 1;
            }
        } else if (arg == "-f" || arg == "--file") {
            if (i + 1 < argc) {
                filename = argv[++i];
                interactive = false;
            } else {
                std::cerr << "Erro: --file requer um valor" << std::endl;
                return 1;
            }
        } else if (arg == "-t" || arg == "--text") {
            if (i + 1 < argc) {
                text = argv[++i];
                interactive = false;
            } else {
                std::cerr << "Erro: --text requer um valor" << std::endl;
                return 1;
            }
        } else if (arg == "-i" || arg == "--interactive") {
            interactive = true;
        } else if (arg == "--help") {
            showUsage();
            return 0;
        } else {
            std::cerr << "Argumento desconhecido: " << arg << std::endl;
            showUsage();
            return 1;
        }
    }
    
    try {
        if (interactive) {
            // Modo interativo
            ClientApp::runInteractiveMode(masterHost, masterPort);
        } else if (!filename.empty()) {
            // Processar arquivo específico
            std::cout << "Processando arquivo: " << filename << std::endl;
            std::string fileContent = ClientApp::readTextFile(filename);
            std::cout << "Arquivo carregado com sucesso (" << fileContent.length() << " caracteres)" << std::endl;
            
            std::string result = ClientApp::sendToMaster(masterHost, masterPort, "/upload", fileContent);
            ClientApp::displayResults(result);
            
        } else if (!text.empty()) {
            // Processar texto específico
            std::cout << "Processando texto: " << text << std::endl;
            std::string result = ClientApp::sendToMaster(masterHost, masterPort, "/process", text);
            ClientApp::displayResults(result);
            
        } else {
            // Modo interativo por padrão
            ClientApp::runInteractiveMode(masterHost, masterPort);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
