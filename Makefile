# Compilador
CXX = g++
# Flags de compilação
CXXFLAGS = -Wall -std=c++11

# Nome do executável
TARGET = programa

# Arquivos objeto gerados
OBJS = main.o resultados.o vnd.o grasp.o

# Regra principal: compila o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Regras para compilar cada objeto
main.o: main.cpp resultados.h vnd.h grasp.h
	$(CXX) $(CXXFLAGS) -c main.cpp

resultados.o: resultados.cpp resultados.h
	$(CXX) $(CXXFLAGS) -c resultados.cpp

vnd.o: vnd.cpp vnd.h
	$(CXX) $(CXXFLAGS) -c vnd.cpp

grasp.o: grasp.cpp grasp.h
	$(CXX) $(CXXFLAGS) -c grasp.cpp

# Limpeza dos arquivos gerados
clean:
	rm -f *.o $(TARGET)
