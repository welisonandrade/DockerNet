# Usar uma imagem base com compilador C++
FROM gcc:latest

# Definir o diretório de trabalho
WORKDIR /app

# Copiar o código-fonte e a biblioteca http
COPY escravo2.cpp httplib.h ./

# Compilar o código C++17
RUN g++ -std=c++17 escravo2.cpp -o escravo2_server

# Expor a porta
EXPOSE 8082

# Comando para iniciar o servidor
CMD ["./escravo2_server"]