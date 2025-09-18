# DockerNet

Sistema distribuído em C++ utilizando threads e containers Docker para processamento paralelo de dados, com comunicação mestre-escravo via HTTP.

## Tecnologias Utilizadas
- **C++17** ou superior
- **Docker** e **docker-compose** para orquestração de containers
- **RESTful API** com comunicação via HTTP
- **Threads** para processamento paralelo (std::thread, std::async)
- **cpp-httplib** ou **Boost.Beast** para a comunicação HTTP

## Arquitetura
A arquitetura do sistema é baseada em uma abordagem **mestre-escravo**, com o Mestre controlando a distribuição de tarefas para dois **Escravos** responsáveis pelo processamento dos dados. O Cliente envia um arquivo de texto e recebe o resultado processado em formato JSON.

### Estrutura:
- **Cliente**: Envia o arquivo `.txt` e exibe os resultados. (Interface gráfica ou linha de comando).
- **Mestre**: Recebe as requisições e gerencia o processamento em paralelo.
- **Escravos**: Processam os dados em dois endpoints: `/letras` (contagem de letras) e `/numeros` (contagem de números).
  
## Como Rodar o Projeto

### 1. Clonar o Repositório
```bash
git clone https://github.com/SEU-USUARIO/DockerNet.git
cd DockerNet
```

### 2. Construir as Imagens Docker
```bash
docker-compose build

```

### 3. Subir os Containers
```bash
docker-compose up

```

### 4. Alternativa: Compilar localmente
```bash
mkdir build
cd build
cmake ..
make

# Rodar em terminais separados:
# ./build/bin/slave_letras
# ./build/bin/slave_numeros  
# ./build/bin/master
# ./build/bin/client
```

### 5. Testar o Sistema
O cliente pode ser executado localmente e se comunicará com o Mestre via HTTP.

## Como Funciona
- O **Cliente** envia um arquivo `.txt` contendo letras e números para o **Mestre**.
- O **Mestre** distribui as tarefas para os **Escravos** em threads paralelas.
- O **Mestre** aguarda as respostas dos Escravos, consolida os dados e envia a resposta final de volta ao Cliente em formato **JSON**.

## Contribuições
Sinta-se à vontade para contribuir para o projeto! Para começar, basta fazer um fork, criar uma branch para a sua feature e enviar um pull request.

