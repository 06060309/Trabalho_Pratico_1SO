import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from matplotlib import rcParams

rcParams['font.family'] = 'DejaVu Sans'
rcParams['font.size'] = 10
rcParams['axes.labelsize'] = 12
rcParams['axes.titlesize'] = 14
rcParams['legend.fontsize'] = 10
rcParams['xtick.labelsize'] = 10
rcParams['ytick.labelsize'] = 10

def carregar_dados():
    """Carrega os dados dos experimentos E1 e E2"""
    try:
        # Carregar dados do Experimento E1
        df_e1 = pd.read_csv('resultados_e1.csv')
        print("Dados do Experimento E1:")
        print(df_e1)
        print()
        
        # Carregar dados do Experimento E2
        df_e2 = pd.read_csv('resultados_e2.csv')
        print("Dados do Experimento E2:")
        print(df_e2)
        print()
        
        return df_e1, df_e2
    except FileNotFoundError as e:
        print(f"Erro ao carregar dados: {e}")
        return None, None

def gerar_grafico_e1(df_e1):
    """Gera gráfico do Experimento E1: Sequencial vs Paralelo"""
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    # Gráfico 1: Tempo de Execução
    ax1.plot(df_e1['Tamanho'], df_e1['Sequencial_ms'], 'o-', label='Sequencial', linewidth=2, markersize=8)
    ax1.plot(df_e1['Tamanho'], df_e1['Threads_ms'], 's-', label='Threads (P=4)', linewidth=2, markersize=8)
    ax1.plot(df_e1['Tamanho'], df_e1['Processos_ms'], '^-', label='Processos (P=4)', linewidth=2, markersize=8)
    
    ax1.set_xlabel('Tamanho da Matriz (NxN)')
    ax1.set_ylabel('Tempo de Execução (ms)')
    ax1.set_title('Experimento E1: Tempo de Execução por Tamanho de Matriz')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    ax1.set_yscale('log') 
    
    # Gráfico 2: Speedup
    speedup_threads = df_e1['Sequencial_ms'] / df_e1['Threads_ms']
    speedup_processos = df_e1['Sequencial_ms'] / df_e1['Processos_ms']
    
    ax2.plot(df_e1['Tamanho'], speedup_threads, 's-', label='Speedup Threads', linewidth=2, markersize=8)
    ax2.plot(df_e1['Tamanho'], speedup_processos, '^-', label='Speedup Processos', linewidth=2, markersize=8)
    ax2.axhline(y=4, color='r', linestyle='--', alpha=0.7, label='Speedup Ideal (4x)')
    
    ax2.set_xlabel('Tamanho da Matriz (NxN)')
    ax2.set_ylabel('Speedup (x)')
    ax2.set_title('Experimento E1: Speedup por Tamanho de Matriz')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('grafico_experimento_e1.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    print("Gráfico do Experimento E1 salvo como: grafico_experimento_e1.png")

def gerar_grafico_e2(df_e2):
    """Gera gráfico do Experimento E2: Impacto do número de threads/processos"""
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    # Gráfico 1: Tempo de Execução vs P
    ax1.plot(df_e2['P'], df_e2['Threads_ms'], 'o-', label='Threads', linewidth=2, markersize=8)
    ax1.plot(df_e2['P'], df_e2['Processos_ms'], 's-', label='Processos', linewidth=2, markersize=8)
    
    ax1.set_xlabel('Número de Threads/Processos (P)')
    ax1.set_ylabel('Tempo de Execução (ms)')
    ax1.set_title('Experimento E2: Tempo de Execução vs Número de P')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    ax1.set_xticks(df_e2['P'])
    
    # Gráfico 2: Speedup vs P
    ax2.plot(df_e2['P'], df_e2['Speedup_Threads'], 'o-', label='Speedup Threads', linewidth=2, markersize=8)
    ax2.plot(df_e2['P'], df_e2['Speedup_Processos'], 's-', label='Speedup Processos', linewidth=2, markersize=8)
    ax2.plot(df_e2['P'], df_e2['P'], 'r--', alpha=0.7, label='Speedup Linear Ideal')
    
    ax2.set_xlabel('Número de Threads/Processos (P)')
    ax2.set_ylabel('Speedup (x)')
    ax2.set_title('Experimento E2: Speedup vs Número de P')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    ax2.set_xticks(df_e2['P'])
    
    plt.tight_layout()
    plt.savefig('grafico_experimento_e2.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    print("Gráfico do Experimento E2 salvo como: grafico_experimento_e2.png")

def gerar_tabela_comparativa(df_e1, df_e2):
    """Gera tabela comparativa dos resultados"""
    
    print("\n" + "="*80)
    print("ANÁLISE COMPARATIVA DOS RESULTADOS")
    print("="*80)
    
    # Análise do Experimento E1
    print("\nEXPERIMENTO E1 - SEQUENCIAL VS PARALELO:")
    print("-" * 50)
    
    for _, row in df_e1.iterrows():
        tamanho = int(row['Tamanho'])
        seq_time = row['Sequencial_ms']
        threads_time = row['Threads_ms']
        processos_time = row['Processos_ms']
        
        if threads_time > 0:
            speedup_threads = seq_time / threads_time
        else:
            speedup_threads = float('inf')
            
        speedup_processos = seq_time / processos_time
        
        print(f"Matriz {tamanho}x{tamanho}:")
        print(f"  Sequencial: {seq_time:6.0f} ms")
        print(f"  Threads:    {threads_time:6.0f} ms (speedup: {speedup_threads:.2f}x)")
        print(f"  Processos:  {processos_time:6.0f} ms (speedup: {speedup_processos:.2f}x)")
        print()
    
    # Análise do Experimento E2
    print("EXPERIMENTO E2 - IMPACTO DO NÚMERO DE P:")
    print("-" * 50)
    
    melhor_p_threads = df_e2.loc[df_e2['Speedup_Threads'].idxmax()]
    melhor_p_processos = df_e2.loc[df_e2['Speedup_Processos'].idxmax()]
    
    print(f"Melhor configuração para Threads:")
    print(f"  P = {melhor_p_threads['P']}, Speedup = {melhor_p_threads['Speedup_Threads']:.3f}x")
    print()
    print(f"Melhor configuração para Processos:")
    print(f"  P = {melhor_p_processos['P']}, Speedup = {melhor_p_processos['Speedup_Processos']:.3f}x")
    print()
    
    # Eficiência
    print("EFICIÊNCIA (Speedup/P):")
    print("-" * 30)
    for _, row in df_e2.iterrows():
        p = row['P']
        eff_threads = row['Speedup_Threads'] / p
        eff_processos = row['Speedup_Processos'] / p
        print(f"P = {p}: Threads = {eff_threads:.3f}, Processos = {eff_processos:.3f}")

def main():
    """Função principal"""
    print("Analisando resultados dos experimentos...")
    print()
    
    # Carregar dados
    df_e1, df_e2 = carregar_dados()
    
    if df_e1 is None or df_e2 is None:
        print("Erro: Não foi possível carregar os dados dos experimentos.")
        return
    
    print("Gerando gráficos...")
    gerar_grafico_e1(df_e1)
    gerar_grafico_e2(df_e2)

    gerar_tabela_comparativa(df_e1, df_e2)
    
    print("\nAnálise concluída!")
    print("Arquivos gerados:")
    print("- grafico_experimento_e1.png")
    print("- grafico_experimento_e2.png")

if __name__ == "__main__":
    main()
