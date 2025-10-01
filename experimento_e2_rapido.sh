#!/bin/bash

# Experimento E2 Simplificado: Impacto do número de threads/processos

echo "=== EXPERIMENTO E2: IMPACTO DO NÚMERO DE THREADS/PROCESSOS ==="
echo "Analisando o impacto do valor P (número de threads/processos) no desempenho"
echo

TAMANHO_TESTE=800
ARQUIVO_RESULTADOS_E2="resultados_e2.csv"

echo "Usando matriz ${TAMANHO_TESTE}x${TAMANHO_TESTE}"
echo

# Obter tempo sequencial de referência
echo "Obtendo tempo sequencial de referência..."
tempo_seq=$(./multiplicacao_sequencial $TAMANHO_TESTE | grep "Tempo de execução:" | head -1 | awk '{print $4}')
echo "Tempo sequencial de referência: ${tempo_seq} ms"
echo

# Valores de P para testar
VALORES_P=(1 2 4 6 8)

# Criar cabeçalho do arquivo CSV
echo "P,Threads_ms,Processos_ms,Speedup_Threads,Speedup_Processos" > $ARQUIVO_RESULTADOS_E2

for p in "${VALORES_P[@]}"; do
    echo "----------------------------------------"
    echo "Testando com P = $p"
    echo "----------------------------------------"
    
    # Teste com Threads
    echo "Executando versão com $p threads..."
    tempo_threads=$(./multiplicacao_threads $TAMANHO_TESTE $p | grep "Tempo de execução:" | head -1 | awk '{print $4}')
    echo "Tempo threads: ${tempo_threads} ms"
    
    # Teste com Processos
    echo "Executando versão com $p processos..."
    tempo_processos=$(./multiplicacao_processos $TAMANHO_TESTE $p | grep "Tempo de execução:" | head -1 | awk '{print $4}')
    echo "Tempo processos: ${tempo_processos} ms"
    
    # Calcular speedup em relação ao sequencial
    if [ "$tempo_seq" != "0" ] && [ "$tempo_threads" != "0" ] && [ "$tempo_processos" != "0" ]; then
        speedup_threads=$(echo "scale=3; $tempo_seq / $tempo_threads" | bc -l)
        speedup_processos=$(echo "scale=3; $tempo_seq / $tempo_processos" | bc -l)
        echo "Speedup threads: ${speedup_threads}x"
        echo "Speedup processos: ${speedup_processos}x"
    else
        speedup_threads="N/A"
        speedup_processos="N/A"
    fi
    
    # Salvar resultados no CSV
    echo "${p},${tempo_threads},${tempo_processos},${speedup_threads},${speedup_processos}" >> $ARQUIVO_RESULTADOS_E2
    
    echo
done

echo "=== EXPERIMENTO E2 CONCLUÍDO ==="
echo "Tamanho de matriz usado: ${TAMANHO_TESTE}x${TAMANHO_TESTE}"
echo "Tempo sequencial de referência: ${tempo_seq} ms"
echo "Resultados salvos em: $ARQUIVO_RESULTADOS_E2"
echo

# Mostrar resumo dos resultados
echo "RESUMO DOS RESULTADOS:"
echo "======================"
cat $ARQUIVO_RESULTADOS_E2

# Encontrar o melhor P para threads e processos
echo
echo "ANÁLISE DOS RESULTADOS:"
echo "======================="

# Melhor P para threads (ignorar linhas com N/A)
melhor_p_threads=$(tail -n +2 $ARQUIVO_RESULTADOS_E2 | grep -v "N/A" | sort -t',' -k4 -nr | head -1 | cut -d',' -f1)
melhor_speedup_threads=$(tail -n +2 $ARQUIVO_RESULTADOS_E2 | grep -v "N/A" | sort -t',' -k4 -nr | head -1 | cut -d',' -f4)

# Melhor P para processos (ignorar linhas com N/A)
melhor_p_processos=$(tail -n +2 $ARQUIVO_RESULTADOS_E2 | grep -v "N/A" | sort -t',' -k5 -nr | head -1 | cut -d',' -f1)
melhor_speedup_processos=$(tail -n +2 $ARQUIVO_RESULTADOS_E2 | grep -v "N/A" | sort -t',' -k5 -nr | head -1 | cut -d',' -f5)

echo "Melhor P para threads: $melhor_p_threads (speedup: ${melhor_speedup_threads}x)"
echo "Melhor P para processos: $melhor_p_processos (speedup: ${melhor_speedup_processos}x)"
