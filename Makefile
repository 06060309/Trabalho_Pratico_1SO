# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
THREADFLAGS = -pthread

# Arquivos fonte
SOURCES = gerador_matrizes.cpp multiplicacao_sequencial.cpp multiplicacao_threads.cpp multiplicacao_processos.cpp

# Executáveis
TARGETS = gerador_matrizes multiplicacao_sequencial multiplicacao_threads multiplicacao_processos

# Regra padrão
all: $(TARGETS)

# Compilação individual
gerador_matrizes: gerador_matrizes.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

multiplicacao_sequencial: multiplicacao_sequencial.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

multiplicacao_threads: multiplicacao_threads.cpp
	$(CXX) $(CXXFLAGS) $(THREADFLAGS) -o $@ $<

multiplicacao_processos: multiplicacao_processos.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
	rm -f matriz_*.txt
	rm -f resultado_*.txt
	rm -f resultados_*.csv
	rm -f *.png

distclean: clean
	rm -f *.png *.csv

test: all
	@echo "Executando teste de corretude..."
	./verificar_corretude.sh

# Executar experimentos
experimentos: all
	@echo "Executando Experimento E1..."
	./experimento_e1.sh
	@echo "Executando Experimento E2..."
	./experimento_e2_rapido.sh
	@echo "Gerando análises..."
	python3 analisar_resultados.py