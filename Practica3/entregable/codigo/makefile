#Makefile para la Práctica 3: Algoritmos Greedy
CXX = g++
CXXFLAGS = -O3 -Wall

#Objetivos principales
all: problema3_1 nearest_neighbor nearest_insertion

#Compilación del Problema 1
problema3_1: problema3_1.cpp
	$(CXX) $(CXXFLAGS) problema3_1.cpp -o problema3_1

#Compilación del Problema 2: Vecino más cercano
nearest_neighbor: nearest_neighbor.cpp data_loader.cpp plot_tour.cpp
	$(CXX) $(CXXFLAGS) nearest_neighbor.cpp data_loader.cpp plot_tour.cpp -o nearest_neighbor

#Compilación del Problema 2: Inserción más económica
nearest_insertion: nearest_insertion.cpp data_loader.cpp plot_tour.cpp
	$(CXX) $(CXXFLAGS) nearest_insertion.cpp data_loader.cpp plot_tour.cpp -o nearest_insertion

# Limpieza
clean:
	rm -f problema3_1 nearest_neighbor nearest_insertion *.png *.tmp.dat *.tmp.plt
