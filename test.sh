#!/bin/bash

echo "=== Testando Sistema Distribuído ==="

# Mata processos anteriores se existirem
pkill -f slave_letras
pkill -f slave_numeros  
pkill -f master

sleep 2

echo "1. Iniciando Escravo Letras..."
./build/bin/slave_letras &
SLAVE_LETRAS_PID=$!

echo "2. Iniciando Escravo Numeros..."
./build/bin/slave_numeros &
SLAVE_NUMEROS_PID=$!

echo "3. Aguardando escravos iniciarem..."
sleep 3

echo "4. Testando escravos..."
curl -s http://localhost:8081/health
echo ""
curl -s http://localhost:8082/health  
echo ""

echo "5. Iniciando Mestre..."
./build/bin/master &
MASTER_PID=$!

echo "6. Aguardando mestre iniciar..."
sleep 3

echo "7. Testando mestre..."
curl -s http://localhost:8080/health
echo ""

echo "8. Testando processamento completo..."
curl -X POST http://localhost:8080/process \
  -H "Content-Type: text/plain" \
  -d "Hello World 123 ABC"
echo ""

echo "9. Limpando processos..."
kill $SLAVE_LETRAS_PID $SLAVE_NUMEROS_PID $MASTER_PID

echo "=== Teste concluído ==="
