#!/bin/bash

# Experimento E1: Sequencial vs. Paralelo

echo "=== EXPERIMENTO E1: SEQUENCIAL VS. PARALELO ==="
echo "Comparando tempos de execução para diferentes tamanhos de matriz"
echo

TAMANHOS=(100 200 400 800 1600)
NUM_THREADS=4
NUM_PROCESSOS=4
ARQUIVO_RESULTADOS="resultados_e1.csv"

# Criar cabeçalho do arquivo CSV
echo "Tamanho,Sequencial_ms,Threads_ms,Processos_ms" > $ARQUIVO_RESULTADOS

echo "Configuração:"
echo "- Número de threads: $NUM_THREADS"
echo "- Número de processos: $NUM_PROCESSOS"
echo "- Tamanhos de matriz: ${TAMANHOS[@]}"
echo "- Arquivo de resultados: $ARQUIVO_RESULTADOS"
echo

for tamanho in "${TAMANHOS[@]}"; do
    echo "----------------------------------------"
    echo "Testando matriz ${tamanho}x${tamanho}"
    echo "----------------------------------------"

    if [ ! -f "matriz_a_${tamanho}.txt" ] || [ ! -f "matriz_b_${tamanho}.txt" ]; then
        echo "Gerando matrizes ${tamanho}x${tamanho}..."
        ./gerador_matrizes $tamanho
        echo
    fi
    
    # Teste Sequencial
    echo "Executando versão sequencial..."
    resultado_seq=$(./multiplicacao_sequencial $tamanho | grep "Tempo de execução:" | head -1 | awk '{print $4}')
    echo "Tempo sequencial: ${resultado_seq} ms"
    
    # Teste com Threads
    echo "Executando versão com threads..."
    resultado_threads=$(./multiplicacao_threads $tamanho $NUM_THREADS | grep "Tempo de execução:" | head -1 | awk '{print $4}')
    echo "Tempo threads: ${resultado_threads} ms"
    
    # Teste com Processos
    echo "Executando versão com processos..."
    resultado_processos=$(./multiplicacao_processos $tamanho $NUM_PROCESSOS | grep "Tempo de execução:" | head -1 | awk '{print $4}')
    echo "Tempo processos: ${resultado_processos} ms"
    
    # Salvar resultados no CSV
    echo "${tamanho},${resultado_seq},${resultado_threads},${resultado_processos}" >> $ARQUIVO_RESULTADOS
    
    # Calcular speedup
    if [ "$resultado_seq" != "0" ]; then
        speedup_threads=$(echo "scale=2; $resultado_seq / $resultado_threads" | bc -l)
        speedup_processos=$(echo "scale=2; $resultado_seq / $resultado_processos" | bc -l)
        echo "Speedup threads: ${speedup_threads}x"
        echo "Speedup processos: ${speedup_processos}x"
    fi
    
    echo
done

echo "=== EXPERIMENTO E1 CONCLUÍDO ==="
echo "Resultados salvos em: $ARQUIVO_RESULTADOS"
echo

echo "RESUMO DOS RESULTADOS:"
echo "======================"
cat $ARQUIVO_RESULTADOS
