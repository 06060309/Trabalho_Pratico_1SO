#!/bin/bash

# Compara os resultados da multiplicação sequencial, threads e processos

echo "=== VERIFICAÇÃO DE CORRETUDE ==="
echo "Verificando se todos os programas produzem resultados idênticos"
echo

TAMANHO=50  
NUM_THREADS=2
NUM_PROCESSOS=2

echo "Gerando matrizes ${TAMANHO}x${TAMANHO} para teste..."
./gerador_matrizes $TAMANHO

echo "Executando multiplicação sequencial..."
./multiplicacao_sequencial $TAMANHO

echo "Executando multiplicação com threads..."
./multiplicacao_threads $TAMANHO $NUM_THREADS

echo "Executando multiplicação com processos..."
./multiplicacao_processos $TAMANHO $NUM_PROCESSOS

echo
echo "Comparando resultados..."

# Comparar arquivos de resultado
ARQUIVO_SEQ="resultado_sequencial_${TAMANHO}.txt"
ARQUIVO_THREADS="resultado_threads_${TAMANHO}_${NUM_THREADS}.txt"
ARQUIVO_PROCESSOS="resultado_processos_${TAMANHO}_${NUM_PROCESSOS}.txt"

if [ -f "$ARQUIVO_SEQ" ] && [ -f "$ARQUIVO_THREADS" ] && [ -f "$ARQUIVO_PROCESSOS" ]; then
    echo "Comparando sequencial vs threads..."
    if diff -q "$ARQUIVO_SEQ" "$ARQUIVO_THREADS" > /dev/null; then
        echo "Sequencial e Threads: IDÊNTICOS"
    else
        echo "Sequencial e Threads: DIFERENTES"
        echo "Primeiras diferenças:"
        diff "$ARQUIVO_SEQ" "$ARQUIVO_THREADS" | head -10
    fi
    
    echo "Comparando sequencial vs processos..."
    if diff -q "$ARQUIVO_SEQ" "$ARQUIVO_PROCESSOS" > /dev/null; then
        echo "Sequencial e Processos: IDÊNTICOS"
    else
        echo "Sequencial e Processos: DIFERENTES"
        echo "Primeiras diferenças:"
        diff "$ARQUIVO_SEQ" "$ARQUIVO_PROCESSOS" | head -10
    fi
    
    echo "Comparando threads vs processos..."
    if diff -q "$ARQUIVO_THREADS" "$ARQUIVO_PROCESSOS" > /dev/null; then
        echo "Threads e Processos: IDÊNTICOS"
    else
        echo "Threads e Processos: DIFERENTES"
        echo "Primeiras diferenças:"
        diff "$ARQUIVO_THREADS" "$ARQUIVO_PROCESSOS" | head -10
    fi
else
    echo "Erro: Nem todos os arquivos de resultado foram gerados"
fi

echo
echo "=== VERIFICAÇÃO CONCLUÍDA ==="
