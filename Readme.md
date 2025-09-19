# Sistema Distribuído Mestre-Escravo com C++ e Docker

##  Descrição do Projeto

[cite_start]Este projeto implementa um sistema distribuído em C++ puro, utilizando uma arquitetura mestre-escravo para processar arquivos de texto de forma concorrente[cite: 3]. [cite_start]O sistema é conteinerizado com Docker, facilitando a implantação e a execução dos diferentes componentes[cite: 23].

[cite_start]O objetivo é receber um arquivo `.txt` do cliente, contar o número total de letras e dígitos numéricos de forma paralela usando dois serviços escravos, e retornar o resultado consolidado[cite: 3, 15, 17, 18].

##  Arquitetura

O sistema é dividido em quatro componentes principais:

* [cite_start]**Cliente**: Uma aplicação de linha de comando que roda na máquina do usuário[cite: 7, 24]. [cite_start]Sua única função é ler um arquivo de texto, enviar seu conteúdo para o Mestre através de uma requisição HTTP (REST) e exibir o resultado final[cite: 6, 8].

* [cite_start]**Mestre (Contêiner 1)**: É o orquestrador do sistema[cite: 3].
    * [cite_start]Recebe as requisições do cliente[cite: 11].
    * [cite_start]Dispara duas threads em paralelo para se comunicar com os escravos[cite: 12, 13].
    * [cite_start]Antes de delegar a tarefa, verifica se cada escravo está online e disponível através de um endpoint `/health`[cite: 14].
    * [cite_start]Após receber as respostas, ele as combina em um único objeto JSON e o retorna ao cliente[cite: 15].

* **Escravo 1 (Contêiner 2)**: Um serviço especializado em contar letras.
    * [cite_start]Expõe um endpoint `/letras` que recebe um texto e retorna a quantidade total de letras (A-Z, a-z)[cite: 17].

* **Escravo 2 (Contêiner 3)**: Um serviço especializado em contar números.
    * [cite_start]Expõe um endpoint `/numeros` que recebe um texto e retorna a quantidade total de dígitos numéricos (0-9)[cite: 18].

## ️ Tecnologias Utilizadas

* [cite_start]**Linguagem**: C++17 [cite: 20]
* [cite_start]**Comunicação entre Processos**: REST HTTP com a biblioteca [cpp-httplib](https://github.com/yhirose/cpp-httplib)[cite: 21].
* [cite_start]**Concorrência**: Threads paralelas gerenciadas com `std::async` (`<future>`)[cite: 22].
* [cite_start]**Conteinerização**: Docker e Docker Compose para orquestração dos serviços[cite: 23].

##  Pré-requisitos

Para executar este projeto no seu ambiente (testado em Fedora 42), você precisará de:

* [Docker Engine](https://docs.docker.com/engine/install/fedora/) e o plugin Docker Compose.
* Compilador C++ (g++) e ferramentas de desenvolvimento.
    ```bash
    sudo dnf groupinstall "Development Tools"
    sudo dnf install gcc-c++
    ```
* [Git](https://git-scm.com/) para clonar o repositório.

##  Como Compilar e Executar

Siga os passos abaixo para colocar o sistema no ar.

**1. Clone o Repositório**
```bash

  git clone <https://github.com/welisonandrade/DockerNet>
  cd <DockerNet>