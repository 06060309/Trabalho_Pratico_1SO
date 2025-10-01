#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

class Matriz {
private:
    vector<vector<double>> dados;
    int dimensao;

public:
    Matriz(int dim) : dimensao(dim) {
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
    
    void multiplicarSequencial(const Matriz& a, const Matriz& b) {
        if (a.dimensao != b.dimensao || a.dimensao != dimensao) {
            cerr << "Erro: Dimensões incompatíveis para multiplicação" << endl;
            return;
        }
        
        // Algoritmo clássico de multiplicação de matrizes O(n³)
        for (int i = 0; i < dimensao; i++) {
            for (int j = 0; j < dimensao; j++) {
                dados[i][j] = 0.0;
                for (int k = 0; k < dimensao; k++) {
                    dados[i][j] += a.dados[i][k] * b.dados[k][j];
                }
            }
        }
    }
    
    int getDimensao() const { return dimensao; }
};

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
    
    cout << "Iniciando multiplicação sequencial de matrizes " << dimensao << "x" << dimensao << endl;
    
    // Criar matrizes
    Matriz matrizA(dimensao);
    Matriz matrizB(dimensao);
    Matriz resultado(dimensao);
    
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
    cout << "Iniciando multiplicação..." << endl;
    auto inicio = chrono::high_resolution_clock::now();
    
    resultado.multiplicarSequencial(matrizA, matrizB);
    
    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    
    // Salvar resultado
    string arquivoResultado = "resultado_sequencial_" + to_string(dimensao) + ".txt";
    cout << "Salvando resultado em: " << arquivoResultado << endl;
    
    if (!resultado.salvarEmArquivo(arquivoResultado)) {
        return 1;
    }
    
    cout << "Multiplicação sequencial concluída!" << endl;
    cout << "Tempo de execução: " << duracao.count() << " ms" << endl;
    cout << "Tempo de execução: " << fixed << setprecision(3) 
         << duracao.count() / 1000.0 << " segundos" << endl;
    
    return 0;
}
