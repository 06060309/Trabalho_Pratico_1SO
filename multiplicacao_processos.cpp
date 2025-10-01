#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <cstring>

using namespace std;

class MatrizProcessos {
private:
    vector<vector<double>> dados;
    int dimensao;

public:
    MatrizProcessos(int dim) : dimensao(dim) {
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
    
    void copiarDeMemoriaCompartilhada(double* memCompartilhada) {
        for (int i = 0; i < dimensao; i++) {
            for (int j = 0; j < dimensao; j++) {
                dados[i][j] = memCompartilhada[i * dimensao + j];
            }
        }
    }
    
    void multiplicarComProcessos(const MatrizProcessos& a, const MatrizProcessos& b, int numProcessos) {
        if (a.dimensao != b.dimensao || a.dimensao != dimensao) {
            cerr << "Erro: Dimensões incompatíveis para multiplicação" << endl;
            return;
        }
        
        // Criar memória compartilhada para o resultado
        size_t tamanhoMemoria = dimensao * dimensao * sizeof(double);
        double* resultado_compartilhado = (double*)mmap(NULL, tamanhoMemoria, 
                                                       PROT_READ | PROT_WRITE, 
                                                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        
        if (resultado_compartilhado == MAP_FAILED) {
            cerr << "Erro ao criar memória compartilhada" << endl;
            return;
        }
        
        // Inicializar memória compartilhada
        memset(resultado_compartilhado, 0, tamanhoMemoria);
        
        vector<pid_t> processos;
        int linhasPorProcesso = dimensao / numProcessos;
        int linhasRestantes = dimensao % numProcessos;
        
        cout << "Distribuindo " << dimensao << " linhas entre " << numProcessos << " processos" << endl;
        cout << "Linhas por processo: " << linhasPorProcesso;
        if (linhasRestantes > 0) {
            cout << " (+" << linhasRestantes << " linhas extras para os primeiros processos)";
        }
        cout << endl;
        
        int linhaAtual = 0;
        
        // Criar processos filhos
        for (int p = 0; p < numProcessos; p++) {
            int linhaInicio = linhaAtual;
            int linhaFim = linhaInicio + linhasPorProcesso;
            
            // Distribuir linhas restantes entre os primeiros processos
            if (p < linhasRestantes) {
                linhaFim++;
            }
            
            cout << "Processo " << p << ": linhas " << linhaInicio << " a " << (linhaFim - 1) << endl;
            
            pid_t pid = fork();
            
            if (pid == 0) {
               
                for (int i = linhaInicio; i < linhaFim; i++) {
                    for (int j = 0; j < dimensao; j++) {
                        double soma = 0.0;
                        for (int k = 0; k < dimensao; k++) {
                            soma += a.dados[i][k] * b.dados[k][j];
                        }
                        resultado_compartilhado[i * dimensao + j] = soma;
                    }
                }
                exit(0); 
            } else if (pid > 0) {
                
                processos.push_back(pid);
            } else {
                cerr << "Erro ao criar processo filho" << endl;
                return;
            }
            
            linhaAtual = linhaFim;
        }
        
        
        for (pid_t pid : processos) {
            int status;
            waitpid(pid, &status, 0);
        }
        
        
        copiarDeMemoriaCompartilhada(resultado_compartilhado);
        
        
        munmap(resultado_compartilhado, tamanhoMemoria);
    }
    
    int getDimensao() const { return dimensao; }
    
    const vector<vector<double>>& getDados() const { return dados; }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <dimensao> <num_processos>" << endl;
        cout << "Exemplo: " << argv[0] << " 100 4" << endl;
        return 1;
    }
    
    int dimensao = atoi(argv[1]);
    int numProcessos = atoi(argv[2]);
    
    if (dimensao <= 0) {
        cerr << "Erro: A dimensão deve ser um número positivo." << endl;
        return 1;
    }
    
    if (numProcessos <= 0) {
        cerr << "Erro: O número de processos deve ser um número positivo." << endl;
        return 1;
    }
    
    // Verificar se o número de processos não excede o número de linhas
    if (numProcessos > dimensao) {
        cout << "Aviso: Número de processos (" << numProcessos 
             << ") maior que o número de linhas (" << dimensao 
             << "). Ajustando para " << dimensao << " processos." << endl;
        numProcessos = dimensao;
    }
    
    cout << "Iniciando multiplicação paralela (processos) de matrizes " 
         << dimensao << "x" << dimensao << " com " << numProcessos << " processos" << endl;
    
    // Criar matrizes
    MatrizProcessos matrizA(dimensao);
    MatrizProcessos matrizB(dimensao);
    MatrizProcessos resultado(dimensao);
    
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
    
    cout << "Iniciando multiplicação com processos..." << endl;
    auto inicio = chrono::high_resolution_clock::now();
    
    resultado.multiplicarComProcessos(matrizA, matrizB, numProcessos);
    
    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    
    // Salvar resultado
    string arquivoResultado = "resultado_processos_" + to_string(dimensao) + "_" + to_string(numProcessos) + ".txt";
    cout << "Salvando resultado em: " << arquivoResultado << endl;
    
    if (!resultado.salvarEmArquivo(arquivoResultado)) {
        return 1;
    }
    
    cout << "Multiplicação com processos concluída!" << endl;
    cout << "Tempo de execução: " << duracao.count() << " ms" << endl;
    cout << "Tempo de execução: " << fixed << setprecision(3) 
         << duracao.count() / 1000.0 << " segundos" << endl;
    
    return 0;
}
