#include "camino_heroe.h"
using namespace std;
// =============================================================================
//   ARCHIVO A COMPLETAR POR EL ALUMNO
// =============================================================================
//
//   1. el_camino_del_heroe(...)
//        - Rellena la matriz D con los costes minimos entre cada par de
//          localizaciones, considerando los costes G(k) de los nodos
//          intermedios.
//        - Rellena la matriz P con la informacion necesaria para reconstruir
//          despues las rutas optimas (matriz de predecesores).
//
//
// Sugerencias:
//   - El coste G(k) solo se suma cuando k es nodo INTERMEDIO,
//     nunca cuando es origen ni destino.
//   - Para los caminos directos inexistentes, usa la constante INF.
//
// =============================================================================


void el_camino_del_heroe(const vector<vector<int>>& C,
                         const vector<int>& G,
                         vector<vector<int>>& D,
                         vector<vector<int>>& P) {

    int n = C.size();

    // Inicializar D y P con sus tamaños correspondientes.
    D.assign(n, vector<int>(n, INF));
    P.assign(n, vector<int>(n, -1));

    // -------------------------------------------------------------------------
    // PASO 1: Inicializar D con los valores de C (caso base de la recurrencia)
    // -------------------------------------------------------------------------
    // >>> AQUI DEBE IR LA IMPLEMENTACION >>>
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            D[i][j] = C[i][j];
        }
    }
    // <<< FIN DEL BLOQUE <<<


    // -------------------------------------------------------------------------
    // PASO 2: Aplicar la recurrencia de programacion dinamica.
    //         Para cada vertice k que actua como intermedio, comprobar para
    //         cada par (i, j) si pasar por k mejora el coste actual de D[i][j].
    //         Recordar actualizar P[i][j] cada vez que se mejore D[i][j].
    // -------------------------------------------------------------------------
    // >>> AQUI DEBE IR LA IMPLEMENTACION >>>
    //El bucle externo es el del nodo intermedio que intentamos añadir
    for (int k = 0; k < n; k++) {
        //Bucle nodo de origen
        for (int i = 0; i < n; i++) {
            //Bucle nodo de destino
            for (int j = 0; j < n; j++) {
                //Como un nodo no puede ser intermedio de si mismo, ni k puede ser i o j
                if (i==k || j==k || i==j) continue;

                //Comprobamos ahora si pasar por 'k' mejora el coste de D[i][j]
                //teniendo en cuenta que hay que añadir el coste de vencer al guardian de G[k]
                if ((D[i][k] + G[k] + D[k][j]) < D[i][j]) {
                    D[i][j] = D[i][k] + G[k] + D[k][j];
                    P[i][j] = k;
                }
            }
        }
    }

    // <<< FIN DEL BLOQUE <<<
}


// Funcion auxiliar recursiva para reconstruir la ruta.
// Anade a ruta los nodos intermedios entre i y j (sin incluir extremos).
static void reconstruir(const vector<vector<int>>& P,
                        int i, int j,
                        vector<int>& ruta) {
    int k = P[i][j];
    if (k == -1) return;            // i->j es directo, no hay intermedios.
    reconstruir(P, i, k, ruta);
    ruta.push_back(k);
    reconstruir(P, k, j, ruta);
}

// Reconstruir la ruta optima de origen a destino usando P.
// La ruta debe incluir tanto el origen como el destino.
// Si no existe ruta posible, devolver un vector vacio.
std::vector<int> recuperar_ruta(const vector<vector<int>>& P,
                                int origen,
                                int destino) {

    vector<int> ruta;
    ruta.push_back(origen);
    reconstruir(P, origen, destino, ruta);
    ruta.push_back(destino);

    return ruta;
}
