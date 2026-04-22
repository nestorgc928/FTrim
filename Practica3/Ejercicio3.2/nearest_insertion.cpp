#include "nearest_insertion.h"
#include "data_loader.h"
#include "plot_tour.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>

using namespace std;

pair<vector<int>, int> heuristic_nearest_insertion(
    const vector<vector<int>>& distance_matrix
) {
    /*
     * TODO: Implement the Nearest Insertion heuristic.
     * Return: A pair containing the optimized tour and the total cost.
     */
    int n = distance_matrix.size();
    vector<int> tour;
    int total_cost = 0;

    //Si el grafo no contiene ciudades lo devolvemos vacio
    if (n==0) return {tour, total_cost};

    //Si hay 3 ciudades o menos el tour consiste en unirlas todas
    if (n <= 3) {
        for (int i=0; i<n; ++i) {
            tour.push_back(i);
        }

        for (int i=0; i<n; ++i) {
            total_cost += distance_matrix[tour[i]][tour[(i+1)%n]];
        }

        return {tour,total_cost};
    }

    //vector para saber si las ciudades ya estan metidas en la ruta
    vector<bool> visitado(n,false);

    //Como el algoritmo nos dice que partimos de un subtour de 3 ciudades, metemos las 3 primeras
    for (int i=0; i<3;++i) {
        tour.push_back(i);
        visitado[i] = true;
    }

    //En el bucle principal, iteramos hasta que todas las ciudades estan en la ruta
    while (tour.size() < n) {
        int mejor_k = -1; //Sera la ciudad 'k' que elegimos para insertar
        int minima_distancia_al_tour = numeric_limits<int>::max();

        //Primer paso: buscamos la ciudad no visitada mas cercana a cualquier ciudad de la ruta
        for (int i=0; i<n; ++i) {
            if (!visitado[i]) {
                int dist_min = numeric_limits<int>::max();
                //Miramos la distancia entre 'i' y los nodos del tour
                for (int c = 0; c < n; ++c) {
                    if (distance_matrix[i][c] < dist_min) {
                        dist_min = distance_matrix[i][c];
                    }
                }
                //si esta ciudad esta mas cerca del tour que las anteriores la guardamos
                if (dist_min < minima_distancia_al_tour) {
                    minima_distancia_al_tour = dist_min;
                    mejor_k = i;
                }
            }
        }

        //Segundo paso: ya sabemos que tenemos que insertar 'mejor_k', ahora vemos donde insertarlo
        int mejor_posicion = -1;
        int menor_incremento = numeric_limits<int>::max();
        int tamanio_tour = tour.size();

        //Probamos a insertarlo entre cada par de ciudades adyacentes de la ruta
        for (int p = 0; p < tamanio_tour; ++p) {
            int i = tour[p];
            int j = tour[(p+1)%tamanio_tour];

            //Usamos la formula que conocemos: lo que nos cuesta el desvío (ir a k y luego a j), menos el camino directo i->j que rompemos
            int incremento = distance_matrix[i][mejor_k] + distance_matrix[mejor_k][j] - distance_matrix[i][j];

            if (incremento < menor_incremento) {
                menor_incremento = incremento;
                mejor_posicion = p+1; //lo guardamos para insertar justo despues de la ciudad i
            }
        }

        //Insertamos la ciudad en el hueco correspondiente y la marcamos como visitada
        tour.insert(tour.begin()+mejor_posicion, mejor_k);
        visitado[mejor_k] = true;
    }

    //Al terminar, para conocer el coste total sumamos recorriendo todo
    for (int i = 0; i < n; ++i) {
        total_cost += distance_matrix[tour[i]][tour[(i+1)%n]];
    }

    return {tour, total_cost};
}

int main(int argc, char* argv[]) {
    // g++ nearest_insertion.cpp data_loader.cpp plot_tour.cpp -o nearest_insertion.bin -O3 && ./nearest_insertion.bin ./data/berlin52.tsp
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <path_to_tsp_file>" << endl;
        return 1;
    }

    string file_path = argv[1];
    pair<vector<Node>, vector<int>> parsed_data = parse_tsplib_file(file_path);
    vector<Node> nodes = parsed_data.first;
    vector<int> optimal_tour = parsed_data.second;

    if (nodes.empty()) {
        cout << "No nodes loaded. Please check the file path and format." << endl;
        return 1;
    }

    vector<vector<int>> dist_matrix = build_distance_matrix(nodes);
    cout << "Successfully loaded " << dist_matrix.size() << " nodes." << endl;
    string instance_name = get_instance_name(file_path);

    if (!optimal_tour.empty()) {
        int optimal_cost = compute_tour_cost(optimal_tour, dist_matrix);
        cout << "Optimal Tour Cost from file: " << optimal_cost << endl;

        string optimal_png_path = instance_name + "_optimal_tour.png";
        if (export_tour_to_png(nodes, optimal_tour, optimal_png_path, "Optimal Tour", "#1f77b4")) {
            cout << "Saved optimal tour visualization to " << optimal_png_path << endl;
        } else {
            cout << "Could not write optimal tour PNG (requires gnuplot)." << endl;
        }
    } else {
        cout << "No optimal tour found in input file." << endl;
    }

    pair<vector<int>, int> insertion_result = heuristic_nearest_insertion(dist_matrix);
    cout << "Nearest Insertion Cost: " << insertion_result.second << endl;

    if (!insertion_result.first.empty()) {
        string computed_png_path = instance_name + "_nearest_insertion_tour.png";
        if (export_tour_to_png(nodes, insertion_result.first, computed_png_path, "Nearest Insertion Tour", "#d62728")) {
            cout << "Saved nearest-insertion tour visualization to " << computed_png_path << endl;
        } else {
            cout << "Could not write nearest-insertion tour PNG (requires gnuplot)." << endl;
        }
    } else {
        cout << "No computed nearest-insertion tour to visualize." << endl;
    }

    return 0;
}
