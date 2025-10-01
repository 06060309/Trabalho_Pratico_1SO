#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <mutex>

using namespace std;

/**
 * Programa Paralelo com Threads - Multiplicação de Matrizes
 * 
 * Este programa realiza a multiplicação de duas matrizes usando threads,
 * onde cada thread é responsável por calcular uma parte das linhas
 * da matriz resultado.
 * 
 * Uso: ./multiplicacao_threads <dimensao> <num_threads>
 * 
 * Entrada: 
 * - matriz_a_<dimensao>.txt
 * - matriz_b_<dimensao>.txt
 * 
 * Saída:
 * - resultado_threads_<dimensao>_<num_threads>.txt
 */

class MatrizThreads {
private:
    vector<vector<double>> dados;
    int dimensao;

public:
    MatrizThreads(int dim) : dimensao(dim) {
        dados.resize(dim, vector<double>(dim, 0.0));
    }
    
    bool carregarDeArquivo(const string& nomeArquivo) {
        ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            cerr << "Erro ao abrir arquivo: " << nomeArquivo << endl;
            return false;
        }
        
        int dimArquivo;
        arquivo >> dimArquivo;
        
        if (dimArquivo != dimensao) {
            cerr << "Erro: Dimensão do arquivo (" << dimArquivo 
                 << ") não corresponde à esperada (" << dimensao << ")" << endl;
            return false;
        }
        
        for (int i = 0; i < dimensao; i++) {
            for (int j = 0; j < dimensao; j++) {
                arquivo >> dados[i][j];
            }
        }
        
        arquivo.close();
        return true;
    }
    
    bool salvarEmArquivo(const string& nomeArquivo) const {
        ofstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            cerr << "Erro ao criar arquivo: " << nomeArquivo << endl;
            return false;
        }
        
        arquivo << dimensao << endl;
        
        for (int i = 0; i < dimensao; i++) {
            for (int j = 0; j < dimensao; j++) {
                arquivo << fixed << setprecision(2) << dados[i][j];
                if (j < dimensao - 1) {
                    arquivo << " ";
                }
            }
            arquivo << endl;
        }
        
        arquivo.close();
        return true;
    }
    
    // Função executada por cada thread para calcular uma faixa de linhas
    static void calcularLinhas(const MatrizThreads& a, const MatrizThreads& b, 
                              MatrizThreads& resultado, int linhaInicio, int linhaFim) {
        int dim = a.dimensao;
        
        for (int i = linhaInicio; i < linhaFim; i++) {
            for (int j = 0; j < dim; j++) {
                resultado.dados[i][j] = 0.0;
                for (int k = 0; k < dim; k++) {
                    resultado.dados[i][j] += a.dados[i][k] * b.dados[k][j];
                }
            }
        }
    }
    
    void multiplicarComThreads(const MatrizThreads& a, const MatrizThreads& b, int numThreads) {
        if (a.dimensao != b.dimensao || a.dimensao != dimensao) {
            cerr << "Erro: Dimensões incompatíveis para multiplicação" << endl;
            return;
        }
        
        vector<thread> threads;
        int linhasPorThread = dimensao / numThreads;
        int linhasRestantes = dimensao % numThreads;
        
        cout << "Distribuindo " << dimensao << " linhas entre " << numThreads << " threads" << endl;
        cout << "Linhas por thread: " << linhasPorThread;
        if (linhasRestantes > 0) {
            cout << " (+" << linhasRestantes << " linhas extras para as primeiras threads)";
        }
        cout << endl;
        
        int linhaAtual = 0;
        
        // Criar e iniciar threads
        for (int t = 0; t < numThreads; t++) {
            int linhaInicio = linhaAtual;
            int linhaFim = linhaInicio + linhasPorThread;
            
            // Distribuir linhas restantes entre as primeiras threads
            if (t < linhasRestantes) {
                linhaFim++;
            }
            
            cout << "Thread " << t << ": linhas " << linhaInicio << " a " << (linhaFim - 1) << endl;
            
            threads.emplace_back(calcularLinhas, ref(a), ref(b), ref(*this), linhaInicio, linhaFim);
            
            linhaAtual = linhaFim;
        }
        
        // Aguardar conclusão de todas as threads
        for (auto& t : threads) {
            t.join();
        }
    }
    
    int getDimensao() const { return dimensao; }
    
    // Permitir acesso aos dados para as threads
    friend void calcularLinhas(const MatrizThreads& a, const MatrizThreads& b, 
                              MatrizThreads& resultado, int linhaInicio, int linhaFim);
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <dimensao> <num_threads>" << endl;
        cout << "Exemplo: " << argv[0] << " 100 4" << endl;
        return 1;
    }
    
    int dimensao = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    
    if (dimensao <= 0) {
        cerr << "Erro: A dimensão deve ser um número positivo." << endl;
        return 1;
    }
    
    if (numThreads <= 0) {
        cerr << "Erro: O número de threads deve ser um número positivo." << endl;
        return 1;
    }
    
    // Verificar se o número de threads não excede o número de linhas
    if (numThreads > dimensao) {
        cout << "Aviso: Número de threads (" << numThreads 
             << ") maior que o número de linhas (" << dimensao 
             << "). Ajustando para " << dimensao << " threads." << endl;
        numThreads = dimensao;
    }
    
    cout << "Iniciando multiplicação paralela (threads) de matrizes " 
         << dimensao << "x" << dimensao << " com " << numThreads << " threads" << endl;
    
    // Criar matrizes
    MatrizThreads matrizA(dimensao);
    MatrizThreads matrizB(dimensao);
    MatrizThreads resultado(dimensao);
    
    // Carregar matrizes dos arquivos
    string arquivoA = "matriz_a_" + to_string(dimensao) + ".txt";
    string arquivoB = "matriz_b_" + to_string(dimensao) + ".txt";
    
    cout << "Carregando matriz A de: " << arquivoA << endl;
    if (!matrizA.carregarDeArquivo(arquivoA)) {
        return 1;
    }
    
    cout << "Carregando matriz B de: " << arquivoB << endl;
    if (!matrizB.carregarDeArquivo(arquivoB)) {
        return 1;
    }
    
    // Medir tempo de execução da multiplicação
    cout << "Iniciando multiplicação com threads..." << endl;
    auto inicio = chrono::high_resolution_clock::now();
    
    resultado.multiplicarComThreads(matrizA, matrizB, numThreads);
    
    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    
    // Salvar resultado
    string arquivoResultado = "resultado_threads_" + to_string(dimensao) + "_" + to_string(numThreads) + ".txt";
    cout << "Salvando resultado em: " << arquivoResultado << endl;
    
    if (!resultado.salvarEmArquivo(arquivoResultado)) {
        return 1;
    }
    
    cout << "Multiplicação com threads concluída!" << endl;
    cout << "Tempo de execução: " << duracao.count() << " ms" << endl;
    cout << "Tempo de execução: " << fixed << setprecision(3) 
         << duracao.count() / 1000.0 << " segundos" << endl;
    
    return 0;
}
