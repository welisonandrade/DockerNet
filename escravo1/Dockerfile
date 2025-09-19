# Usar uma imagem base com compilador C++
FROM gcc:latest

# Definir o diretório de trabalho
WORKDIR /app

# Copiar o código-fonte e a biblioteca http
COPY escravo1.cpp httplib.h ./

# Compilar o código C++17
RUN g++ -std=c++17 escravo1.cpp -o escravo1_server

# Expor a porta que o servidor vai usar
EXPOSE 8081

# Comando para iniciar o servidor quando o container for executado
CMD ["./escravo1_server"]