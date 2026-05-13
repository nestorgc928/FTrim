#include "graph_coloring.h"

#include <queue>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <set>
#include <string>
using namespace std;

/**
 * @brief Analiza un archivo de grafo y construye una estructura Graph.
 * @param filepath Ruta al archivo de grafo.
 * @return Estructura Graph con vértices y lista de adyacencia.
 */
Graph parse_graph_file(const string& filepath) {
    Graph graph;
    ifstream file(filepath);

    if (!file.is_open()) {
        cerr << "Error: Could not open graph file " << filepath << endl;
        return graph;
    }

    string line;
    int num_vertices = -1;
    bool header_parsed = false;

    while (getline(file, line)) {
        // Remove comments and trim whitespace.
        size_t comment_pos = line.find_first_of("#//");
        if (comment_pos != string::npos) {
            line = line.substr(0, comment_pos);
        }

        istringstream iss(line);
        vector<string> tokens;
        string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            continue;
        }

        if (!header_parsed) {
            // First meaningful line contains the number of vertices.
            if (tokens.size() >= 1) {
                num_vertices = stoi(tokens[0]);
            }
            header_parsed = true;
            graph.num_vertices = num_vertices;
            graph.adj.assign(num_vertices, vector<int>());
            continue;
        }

        if (tokens.size() >= 2) {
            int u = stoi(tokens[0]);
            int v = stoi(tokens[1]);
            // Accept either 0-indexed or 1-indexed vertex identifiers.
            if (u >= 1 && u <= num_vertices && v >= 1 && v <= num_vertices) {
                u -= 1;
                v -= 1;
            }
            if (u < 0 || u >= num_vertices || v < 0 || v >= num_vertices) {
                cerr << "Warning: edge (" << tokens[0] << ", " << tokens[1] << ") is outside vertex range and will be ignored." << endl;
                continue;
            }
            if (u == v) {
                continue;
            }
            graph.adj[u].push_back(v);
            graph.adj[v].push_back(u);
        }
    }

    // Remove duplicate neighbors.
    for (auto& neighbors : graph.adj) {
        sort(neighbors.begin(), neighbors.end());
        neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());
    }

    return graph;
}

/**
 * @brief Comprueba si asignar un color a un vértice es válido (ningún vértice adyacente tiene el mismo color).
 * @param vertex El vértice a colorear.
 * @param color El color a asignar.
 * @param colors Asignaciones de colores actuales.
 * @param graph La estructura del grafo.
 * @return true si la asignación de color es válida, false en caso contrario.
 */
bool is_color_valid(
    int vertex,
    int color,
    const vector<int>& colors,
    const Graph& graph
) {
    for (int neighbor : graph.adj[vertex]) {
        if (colors[neighbor] == color) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Cuenta el número de colores únicos utilizados en el coloreo.
 * @param colors Vector de asignaciones de colores.
 * @return Número de colores distintos utilizados.
 */
int count_used_colors(const vector<int>& colors) {
    set<int> unique_colors;
    for (int c : colors) {
        if (c >= 0) {
            unique_colors.insert(c);
        }
    }
    return static_cast<int>(unique_colors.size());
}

/**
 * @brief Realiza un coloreo voraz del grafo.
 * @param graph El grafo a colorear.
 * @return Vector de asignaciones de colores (uno por vértice).
 */
vector<int> greedy_coloring(const Graph& graph) {
    int n = graph.num_vertices;
    vector<int> colors(n, -1);
    vector<int> order(n);

    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return graph.adj[a].size() > graph.adj[b].size();
    });

    for (int vertex : order) {
        vector<bool> forbidden(n, false);
        for (int neighbor : graph.adj[vertex]) {
            if (colors[neighbor] != -1) {
                forbidden[colors[neighbor]] = true;
            }
        }
        int color = 0;
        while (color < n && forbidden[color]) {
            color += 1;
        }
        colors[vertex] = color;
    }
    return colors;
}

bool es_valida_backtracking (const Graph& graph, int max_colors, int vertice, ColoringResult& result) {

    //Primero comprobamos si ya no quedan más vértices por colorear.
    if (vertice == graph.num_vertices) {
        result.success = true;
        result.colors_used = count_used_colors(result.colors);
        return true;
    }


    // Ahora implementamos el backtracking: Si el vértice se puede colorear, entonces continuamos avanzando en profundidad.
    // Si no se puede colorear,podamos la rama.
    for (int color = 0; color < max_colors; ++color) {
        result.nodes_generated++;
        if (is_color_valid(vertice, color, result.colors, graph)) {
            result.colors[vertice] = color;
            if (es_valida_backtracking(graph, max_colors, vertice + 1, result)) {
                return true;
            }
            else result.colors[vertice] = -1;
        }
        // Si el color es inválido o si no es posible colorear el vértice, podamos la rama
        else result.nodes_pruned += 1;
    }
    return false;
}


/**
 * @brief Intenta colorear el grafo usando backtracking con un máximo de max_colors.
 * @param graph El grafo a colorear.
 * @param max_colors Número máximo de colores permitidos.
 * @return ColoringResult con éxito, colores y estadísticas.
 */
ColoringResult backtracking_color_with_k(const Graph& graph, int max_colors) {
    ColoringResult result;
    result.colors.assign(graph.num_vertices, -1);
    // using at most `max_colors`. The algorithm should update `result.nodes_generated`,
    // `result.nodes_pruned` and `result.max_live_nodes`.

    result.nodes_generated=1;
    result.nodes_pruned=0;
    result.max_live_nodes = graph.num_vertices;

    es_valida_backtracking(graph, max_colors, 0, result);

    return result;
}

struct Nodo {
    vector<int> colores;
    int nivel;
    int max_colors;

    // Para poder meter los nodos en una cola de prioridad correctamente como se sugiere en el guión de prácticas,
    // necesitamos sobrecargar el operador > para que ordene los nodos según el color de índice más alto usado.

    bool operator>(const Nodo & nodo) const{
        return this->max_colors > nodo.max_colors;
    }
};

/**
 * @brief Encuentra el número mínimo de colores necesarios para colorear el grafo usando branch and bound.
 * @param graph El grafo a colorear.
 * @return ColoringResult con coloreo óptimo y estadísticas.
 */
ColoringResult branch_and_bound_min_colors(const Graph& graph) {
    ColoringResult result;
    result.colors.assign(graph.num_vertices, -1);
    // needed to color the graph. Use a greedy upper bound and a lower bound based on the
    // current partial assignment to prune the search.

    //Antes de nada, para establecer una cota inicial para el B&B, utilizamos la solución greedy proporcionada
    vector<int> sol_greedy=greedy_coloring(graph);
    int cota = count_used_colors(sol_greedy);

    // Ahora establecemos el result inicial conforme la solución greedy encontrada.
    result.success=true;
    result.colors=sol_greedy;
    result.colors_used=cota;
    result.nodes_generated=1;
    result.nodes_pruned=0;
    result.max_live_nodes=1;

    priority_queue<Nodo, vector<Nodo>, greater<Nodo>> cola;

    Nodo raiz;
    raiz.colores.assign(graph.num_vertices, -1);
    raiz.nivel=0;
    raiz.max_colors=-1;

    cola.push(raiz);
    while (!cola.empty()) {

        // Primero vemos si al alcanzar el final la solución obtenida es la mejor solución
        if (cola.size()>result.max_live_nodes) result.max_live_nodes=cola.size();
        Nodo actual = cola.top();
        cola.pop();
        if (actual.nivel == graph.num_vertices) {
            int coste = actual.max_colors+1;
            if (coste < result.colors_used) {
                result.colors_used=coste;
                result.colors=actual.colores;
                cota=coste;
            }
        }
        else {
            int nivel = actual.nivel;

            for (int color = 0; color <= actual.max_colors+1; color++) {
                result.nodes_generated++;
                if (max(actual.max_colors,color)+1 >= cota) {
                    result.nodes_pruned++;
                    continue;
                }
                if (is_color_valid(nivel, color, actual.colores, graph)) {
                    Nodo hijo;
                    hijo.colores=actual.colores;
                    hijo.colores[nivel]=color;
                    hijo.nivel=nivel+1;
                    hijo.max_colors=max(actual.max_colors,color);
                    cola.push(hijo);
                }
            }
        }
    }


    return result;
}

/**
 * @brief Imprime la solución de coloreo en la consola.
 * @param result El ColoringResult a imprimir.
 * @param caption Título para la sección de salida.
 */
void print_coloring_solution(const ColoringResult& result, const string& caption) {
    cout << "=== " << caption << " ===" << endl;
    if (!result.success) {
        cout << "No valid coloring found." << endl;
        return;
    }
    cout << "Colors used: " << result.colors_used << endl;
    cout << "Nodes generated: " << result.nodes_generated << endl;
    cout << "Nodes pruned: " << result.nodes_pruned << endl;
    cout << "Max live nodes: " << result.max_live_nodes << endl;
    cout << "Color assignment:" << endl;
    for (int i = 0; i < static_cast<int>(result.colors.size()); ++i) {
        cout << "  vertex " << i << " -> color " << result.colors[i] << endl;
    }
}

/**
 * @brief Extrae el nombre de instancia de una ruta de archivo.
 * @param path La ruta del archivo.
 * @return El nombre base del archivo sin extensión.
 */
string get_instance_name(const string& path) {
    size_t last_slash = path.find_last_of("/\\");
    string filename = (last_slash == string::npos) ? path : path.substr(last_slash + 1);
    size_t last_dot = filename.find_last_of('.');
    return (last_dot == string::npos) ? filename : filename.substr(0, last_dot);
}

