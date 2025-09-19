# Exemplo de Uso do Sistema

## 1. Compilar o Projeto
```bash
mkdir build
cd build
cmake ..
make
```

## 2. Executar os Serviços

### Terminal 1 - Escravo Letras
```bash
./build/bin/slave_letras
```
Saída esperada:
```
Iniciando servidor Slave Letras na porta 8081...
Aguardando requisições...
```

### Terminal 2 - Escravo Numeros  
```bash
./build/bin/slave_numeros
```
Saída esperada:
```
Iniciando servidor Slave Numeros na porta 8082...
Aguardando requisições...
```

### Terminal 3 - Mestre
```bash
./build/bin/master
```
Saída esperada:
```
Starting Master server on port 8080...
Available endpoints:
  GET  /health - Health check
  POST /process - Process text content
  POST /upload - Upload and process file content
```

### Terminal 4 - Cliente
```bash
./build/bin/client
```

## 3. Testar o Sistema

### Modo Interativo
```bash
./build/bin/client
```
Escolha opção 1 e digite um texto como: "Hello World 123 ABC"

### Com Arquivo
```bash
./build/bin/client test_files/sample1.txt
```

## 4. Resultado Esperado
```json
{
  "status": "success",
  "letters_result": {"letras": 10},
  "numbers_result": {"numeros": 3}
}
```

## 5. Usar Docker (Alternativa)
```bash
docker-compose build
docker-compose up
```
