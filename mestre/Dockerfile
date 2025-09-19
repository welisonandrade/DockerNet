# Usar uma imagem base com compilador C++
FROM gcc:latest

# Definir o diretório de trabalho
WORKDIR /app

# Copiar os fontes e as bibliotecas
COPY mestre.cpp httplib.h json.hpp ./

# Compilar o código, linkando a biblioteca de threads (pthread)
RUN g++ -std=c++17 mestre.cpp -o mestre_server -lpthread

# Expor a porta
EXPOSE 8080

# Comando para iniciar o servidor
CMD ["./mestre_server"]