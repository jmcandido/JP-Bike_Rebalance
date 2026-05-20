
CXX = g++

CXXFLAGS = -Wall -std=c++11 -O3

ifeq ($(OS),Windows_NT)
SOCKET_LIBS = -lws2_32
STATIC_LIBS = -static-libgcc -static-libstdc++
else
SOCKET_LIBS =
STATIC_LIBS =
endif

# Nome do executável
TARGET = programa
WEB_TARGET = servidor

OBJS = main.o resultados.o vnd.o grasp.o
CORE_OBJS = resultados.o vnd.o grasp.o
WEB_OBJS = server.o $(CORE_OBJS)

all: $(TARGET) $(WEB_TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(WEB_TARGET): $(WEB_OBJS)
	$(CXX) $(CXXFLAGS) -o $(WEB_TARGET) $(WEB_OBJS) $(SOCKET_LIBS) $(STATIC_LIBS)

# Regras para compilar cada objeto
main.o: main.cpp resultados.h vnd.h grasp.h
	$(CXX) $(CXXFLAGS) -c main.cpp

resultados.o: resultados.cpp resultados.h
	$(CXX) $(CXXFLAGS) -c resultados.cpp

vnd.o: vnd.cpp vnd.h
	$(CXX) $(CXXFLAGS) -c vnd.cpp

grasp.o: grasp.cpp grasp.h
	$(CXX) $(CXXFLAGS) -c grasp.cpp

server.o: server.cpp resultados.h vnd.h grasp.h
	$(CXX) $(CXXFLAGS) -c server.cpp

# Limpeza dos arquivos gerados
clean:
	rm -f *.o $(TARGET) $(WEB_TARGET)
