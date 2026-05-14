# =================================================================
# Makefile para la Práctica 4 de Algorítmica
# Ingeniería de Software - Universidad de Granada
# =================================================================

# Compilador y flags
CXX = g++
CXXFLAGS = -O3 -std=c++17 -I.

# Regla por defecto: compilar todo
all: seleccion_proyectos graph_coloring

# Ejercicio 2.1: Selección de Proyectos
# Se compila desde el archivo único que contiene ambos métodos
seleccion_proyectos: seleccion_proyectos.cpp
	$(CXX) $(CXXFLAGS) seleccion_proyectos.cpp -o seleccion_proyectos

# Ejercicio 2.2: Coloreo de Grafos
# Requiere la unión de main y la lógica de la librería
graph_coloring: main.cpp graph_coloring.cpp graph_coloring.h
	$(CXX) $(CXXFLAGS) main.cpp graph_coloring.cpp -o graph_coloring

# Limpieza de ejecutables
clean:
	rm -f seleccion_proyectos graph_coloring
