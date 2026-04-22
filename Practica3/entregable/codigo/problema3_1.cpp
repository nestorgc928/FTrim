// Imagina que eres un/a arquitecto/a de redes en la nube (Cloud Arquitect). Dispones de un
// conjunto de servidores y una cola de tareas de procesamiento que debes asignar a esos servidores.
// Cada tarea debe asignarse como máximo a un servidor y a cada servidor se le puede asignar como
// máximo una tarea. Suponemos que tenemos el mismo número de servidores que de tareas.

// Cada tarea i requiere una capacidad de cálculo mínima ri para que el proceso se pueda ejecutar
// en un servidor sin agotar el tiempo de espera (time-out). Cada servidor j tiene una capacidad
// de procesamiento cj . Si cj ≥ ri, podemos asignar la tarea i al servidor j y la tarea se procesará
// correctamente. Tu objetivo es maximizar el número de tareas completadas y dar como resultado
// ese número máximo.
// Diseña un algoritmo Greedy para resolver esta tarea. Justifica por qué con este algoritmo se
// obtiene la solución óptima.

// Ejemplo 1. Para r = [4, 6, 9] , c = [3, 5, 8], el número máximo de asignaciones es 2, siendo estas
// (1, 2), (2, 3).

// Ejemplo 2. Para r = [12, 5, 8, 20, 7, 15] , c = [10, 6, 18, 14, 5, 22], el número máximo de asignacio-
// nes es 5, siendo estas (2, 5), (5, 1), (3, 4), (1, 3), (6, 6).

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int maximizar_tareas(vector<int> &tareas, vector<int> &servidores) {
    int num = 0;

    // La estrategia que planteo es ordenar los dos vectores e ir emparejando los elementos más pequeños de los mismos.
    // si un servidor no puede con el elemento más pequeño del vector de trabajos, entonces no va a poder con ninguno
    // de los demás, así que simplemente lo descartamos.

    sort(tareas.begin(), tareas.end());
    sort(servidores.begin(), servidores.end());

    int i =0, j=0;
    // como el número de servidores es igual que el número de tareas, podemos poner esto sin tener riesgo de desbordamiento
    int n = tareas.size();

    while (i < n){
        if (servidores[i] >= tareas[j]) {
            num++;
            j++;
        }
        i++;
    }
    return num;
}

int main() {

    // vector<int> tareas = {4, 6, 9};
    // vector<int> servidores = {3, 5, 8};

    vector<int> tareas = {12, 5, 8, 20, 7, 15};
    vector<int> servidores = {10, 6, 18, 14, 5, 22};

    cout << "Tareas: [";
    for (int i = 0; i < tareas.size(); i++) {
        if (i < tareas.size() - 1) {
            cout << tareas[i] << ", ";
        }
        else cout << tareas[i] << "]" << endl;
    }
    cout << "Servidores: [";
    for (int i = 0; i < servidores.size(); i++) {
        if (i < servidores.size() - 1) {
            cout << servidores[i] << ", ";
        }
        else cout << servidores[i] << "]" << endl;
    }

    cout << "Maximo numero de tareas: " << maximizar_tareas(tareas, servidores) << endl;

    return 0;
}