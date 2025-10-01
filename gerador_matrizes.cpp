#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <chrono>

using namespace std;

/**
 * Programa Auxiliar - Gerador de Matrizes
 * 
 * Este programa gera duas matrizes quadradas de dimensão especificada
 * e as salva em arquivos de texto para posterior uso nos programas
 * de multiplicação.
 * 
 * Uso: ./gerador_matrizes <dimensao>
 * 
 * Saída: 
 * - matriz_a_<dimensao>.txt
 * - matriz_b_<dimensao>.txt
 */

void gerarMatriz(const string& nomeArquivo, int dimensao) {
    ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao criar arquivo: " << nomeArquivo << endl;
        exit(1);
    }
    
    // Configurar gerador de números aleatórios
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(1.0, 100.0);
    
    // Escrever dimensão no início do arquivo
    arquivo << dimensao << endl;
    
    // Gerar e escrever matriz
    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            arquivo << fixed << setprecision(2) << dis(gen);
            if (j < dimensao - 1) {
                arquivo << " ";
            }
        }
        arquivo << endl;
    }
    
    arquivo.close();
    cout << "Matriz " << dimensao << "x" << dimensao << " salva em: " << nomeArquivo << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <dimensao>" << endl;
        cout << "Exemplo: " << argv[0] << " 100" << endl;
        return 1;
    }
    
    int dimensao = atoi(argv[1]);
    
    if (dimensao <= 0) {
        cerr << "Erro: A dimensão deve ser um número positivo." << endl;
        return 1;
    }
    
    cout << "Gerando matrizes " << dimensao << "x" << dimensao << "..." << endl;
    
    auto inicio = chrono::high_resolution_clock::now();
    
    // Gerar matriz A
    string arquivoA = "matriz_a_" + to_string(dimensao) + ".txt";
    gerarMatriz(arquivoA, dimensao);
    
    // Gerar matriz B
    string arquivoB = "matriz_b_" + to_string(dimensao) + ".txt";
    gerarMatriz(arquivoB, dimensao);
    
    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    
    cout << "Matrizes geradas com sucesso em " << duracao.count() << " ms" << endl;
    
    return 0;
}
