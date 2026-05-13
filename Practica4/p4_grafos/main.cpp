#include "graph_coloring.h"

#include <iostream>
#include <string>
using namespace std;

/*
Instrucciones de compilación: g++ -std=c++17 main.cpp graph_coloring.cpp -o graph_coloring
*/


/**
 * @brief Función principal que carga un grafo, realiza coloreo voraz y ejecuta algoritmos de backtracking y branch-and-bound.
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos de línea de comandos (espera ruta de archivo de grafo y opcionalmente colores máximos).
 * @return 0 en éxito, 1 en error.
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <graph_file> [max_colors_for_backtracking]" << endl;
        cout << "Example: " << argv[0] << " data/map_coloring_example.txt 3" << endl;
        return 0;
    }

    string file_path = argv[1];
    Graph graph = parse_graph_file(file_path);

    if (graph.num_vertices <= 0) {
        cout << "Failed to load graph. Please check the file format." << endl;
        return 1;
    }

    int edge_count = 0;
    for (const auto& neighbors : graph.adj) {
        edge_count += static_cast<int>(neighbors.size());
    }
    edge_count /= 2;

    cout << "Loaded graph with " << graph.num_vertices << " vertices and " << edge_count << " edges." << endl;

    vector<int> greedy_colors = greedy_coloring(graph);
    int greedy_used = count_used_colors(greedy_colors);
    cout << "Greedy coloring uses " << greedy_used << " colors." << endl;

    if (argc >= 3) {
        int max_colors = stoi(argv[2]);
        ColoringResult backtracking_result = backtracking_color_with_k(graph, max_colors);
        print_coloring_solution(backtracking_result, "Backtracking with max K=" + to_string(max_colors));
    }

    ColoringResult bnb_result = branch_and_bound_min_colors(graph);
    print_coloring_solution(bnb_result, "Branch and Bound Optimal Coloring");

    return 0;
}

