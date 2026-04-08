//EJERCICIO 2.2
//Sea una matriz M de enteros, de tamaño nxm (n,m>0), en la que existen al menos dos valores
//distintos. Se trata de calcular los valores enteros siguientes:
// -max1: el mayor valor de la matriz y
// -max2: el mayor valor, estrictamente menor que max1, de la matriz.

#include <iostream>
#include <vector>
#include <climits> //Lo añadimos para poder usar INT_MIN
#include <algorithm>
#include <chrono>   // Para medir el tiempo
#include <cstdlib>  // Para rand() y srand()
#include <ctime>    // Para time()

using namespace std;
using namespace std::chrono;

//ALGORITMO PARA CALCULAR MAXIMO Y SEGUNDO MAXIMO MEDIANTE FUERZA BRUTA
int CalcularMaximosFuerzaBruta(const vector<vector<int>> &v, int &max2) {
    int max1 = INT_MIN;
    max2 = INT_MIN;

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {

            //Vemos si supera al maximo absoluto
            if (v[i][j] > max1) {
                max2 = max1;       //El anterior maximo absoluto pasa a ser el segundo
                max1 = v[i][j];    //Cambiamos el maximo absoluto
            }
            //Si no supera al maximo absoluto, pero es mejor que el segundo maximo, lo cambiamos
            else if (v[i][j] < max1 && v[i][j] > max2) {
                max2 = v[i][j];    //Actualizamos solo el segundo maximo
            }

        }
    }
    return max1;
}

//ALGORITMO PARA CALCULAR MAXIMO Y SEGUNDO MAXIMO MEDIANTE DIVIDE Y VENCERAS
//Usamos un struct para almacenar las soluciones parciales
struct Maximos {
    int max1;
    int max2;
};

//Planteamos una funcion que resuelva directamente el algoritmo
Maximos ResolverDirectamente(const vector<vector<int>> &v, int f_ini, int f_fin, int c_ini, int c_fin) {
    Maximos res;
    res.max1 = INT_MIN;
    res.max2 = INT_MIN;

    for (int i = f_ini; i <= f_fin; i++) {
        for (int j = c_ini; j <= c_fin; j++) {
            if (v[i][j] > res.max1) {
                res.max2 = res.max1;
                res.max1 = v[i][j];
            }
            else if (v[i][j] < res.max1 && v[i][j] > res.max2) {
                res.max2 = v[i][j];
            }
        }
    }
    return res;
}

//Algoritmo usando divide y venceras
Maximos MaximosDyV(const vector<vector<int>> &v, int f_ini, int f_fin, int c_ini, int c_fin, int n0) {
    //En primer lugar calculamos el tamaño del problema actual 'n'
    int num_filas = f_fin - f_ini +1;
    int num_cols = c_fin - c_ini +1;
    int n = num_filas * num_cols;

    //Resolvemos el caso base
    if (n <= n0) {
        return ResolverDirectamente(v, f_ini, f_fin, c_ini, c_fin);
    }

    //1-Dividimos el problema en distintos subproblemas
    Maximos S1, S2; //Soluciones parciales

    if (num_filas >= num_cols) {
        //Dividimos por la mitad de las filas
        int f_medio = f_ini + (num_filas/2) -1;

        //2-Conquistar: Llamadas recursivas
        S1 = MaximosDyV(v, f_ini, f_medio, c_ini, c_fin, n0);
        S2 = MaximosDyV(v, f_medio+1, f_fin, c_ini, c_fin, n0);
    }
    else {
        //Dividimos por la mitad de las columnas
        int c_medio = c_ini + (num_cols/2) -1;

        //2-Conquistar: Llamadas recursivas
        S1 = MaximosDyV(v, f_ini, f_fin, c_ini, c_medio, n0);
        S2 = MaximosDyV(v, f_ini, f_fin, c_medio+1, c_fin, n0);
    }

    //3-Combinar: Construimos la solucion final
    Maximos Solucion;
    //max1 sera el maximo de todos los subproblemas
    Solucion.max1 = max(S1.max1, S2.max1);

    //Para obtener el segundo maximo, comparamos al "perdedor" del max1 con el max2 del "ganador"
    if (S1.max1 > S2.max1) {
        Solucion.max2 = max(S1.max2, S2.max1);
    }
    else if (S2.max1 > S1.max1) {
        Solucion.max2 = max(S2.max2, S1.max1);
    }
    else {
        Solucion.max2 = max(S1.max2, S2.max1);
    }

    return Solucion; //Devolvemos la combinacion de las soluciones
}


//Creamos una funcion auxiliar para generar matrices aleatorias de tamaño NxM
vector<vector<int>> GenerarMatrizAleatoria(int n, int m) {
    //Inicializamos un vector de 'n' filas, donde cada fila es un vector de 'm' columnas
    vector<vector<int>> matriz(n, vector<int>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matriz[i][j] = rand() % 100000; // Números aleatorios entre 0 y 99999
        }
    }
    return matriz;
}

/*
// PROGRAMA PRINCIPAL (ESTUDIO EXPERIMENTAL DEL UMBRAL ÓPTIMO)
//EJECUTAR ESTE MAIN CON ./problema2_2 > ../tiempos_y_graficas/datos/estudio_umbral.csv
int main() {
    // Inicializamos la semilla
    srand(time(NULL));

    // 1. Fijamos una matriz GIGANTE y estática para notar bien las diferencias
    int N = 5000;
    int M = 5000;

    cout << "Generando matriz de prueba de " << N << "x" << M << "..." << endl;

    // Generamos la matriz UNA SOLA VEZ fuera del bucle.
    // Así nos aseguramos de que todos los umbrales compiten en igualdad de condiciones
    // y el tiempo de crear la matriz no ensucia las métricas.
    vector<vector<int>> matriz_prueba = GenerarMatrizAleatoria(N, M);

    cout << "Matriz generada. Arrancando baterias de pruebas...\n" << endl;

    // Imprimimos la cabecera CSV
    cout << "Umbral_n0;Tiempo_DyV_ms" << endl;

    // 2. Probamos distintos tamaños de umbral.
    //Iremos probando con distintas potencias de base 2
    for (int umbral = 2; umbral <= 4096; umbral *= 2) {

        //Cronometramos SOLO la ejecución de Divide y Vencerás con el umbral actual
        auto inicio = high_resolution_clock::now();

        MaximosDyV(matriz_prueba, 0, N - 1, 0, M - 1, umbral);

        auto fin = high_resolution_clock::now();
        auto duracion = duration_cast<milliseconds>(fin - inicio).count();

        // Imprimimos el resultado de esta iteración
        cout << umbral << ";" << duracion << endl;
    }

    cout << "\nExperimento terminado." << endl;
    return 0;
}
*/

/*
//EJECUTAR ESTE MAIN CON ./problema2_2 > ../tiempos_y_graficas/datos/tiempos_generales.csv
int main() {
    // Inicializamos la semilla de números aleatorios
    srand(time(NULL));

    // Configuraciones de nuestra prueba
    int umbral_n0 = 4096; //Umbral optimo calculado
    int salto = 500;    //Incremento del tamaño de la matriz en cada iteración
    int max_N = 10000;   //Límite de filas a probar

    // Imprimimos la cabecera en formato CSV (separador: punto y coma)
    cout << "Filas_N;Columnas_M;Tiempo_FB_ms;Tiempo_DyV_ms" << endl;

    // Bucle para probar tamaños de problema crecientes
    for (int N = 500; N <= max_N; N += salto) {

        // Hacemos que M sea distinto de N para garantizar una matriz rectangular nxm
        int M = N + 250;

        // Generamos la matriz de prueba para la iteración actual
        vector<vector<int>> matriz_prueba = GenerarMatrizAleatoria(N, M);
        int max2_fb;

        //Cronometramos la Fuerza Bruta
        auto inicio_fb = high_resolution_clock::now();

        CalcularMaximosFuerzaBruta(matriz_prueba, max2_fb);

        auto fin_fb = high_resolution_clock::now();
        auto duracion_fb = duration_cast<milliseconds>(fin_fb - inicio_fb).count();

        //Cronometramos Divide y Vencerás
        auto inicio_dyv = high_resolution_clock::now();

        //Pasamos (N - 1) y (M - 1) como los índices máximos
        MaximosDyV(matriz_prueba, 0, N - 1, 0, M - 1, umbral_n0);

        auto fin_dyv = high_resolution_clock::now();
        auto duracion_dyv = duration_cast<milliseconds>(fin_dyv - inicio_dyv).count();

        // Salida de resultados
        cout << N << ";" << M << ";" << duracion_fb << ";" << duracion_dyv << endl;
    }

    return 0;
}
*/

int main() {
    //Definimos una matriz de prueba
    vector<vector<int>> matriz_prueba = {
        {12, 45,  2,  8},
        {99, 33, 56,  1},
        { 4, 88, 72, 99},
        {10, 25, 40, 60}
    };

    cout << "PRUEBA DEL PROBLEMA 2.2: MAXIMO Y SEGUNDO MAXIMO\n";

    //Prueba de Fuerza Bruta
    int max2_fb;
    int max1_fb = CalcularMaximosFuerzaBruta(matriz_prueba, max2_fb);

    cout << "\nResultados mediante Fuerza Bruta:" << endl;
    cout << "-> Primer Maximo (max1): " << max1_fb << endl;
    cout << "-> Segundo Maximo (max2): " << max2_fb << endl;

    //Prueba de Divide y Vencerás
    int filas = matriz_prueba.size();
    int columnas = matriz_prueba[0].size();

    //El umbral que calculamos como óptimo en la gráfica
    int umbral_n0 = 4096;

    Maximos resultados_dyv = MaximosDyV(matriz_prueba, 0, filas - 1, 0, columnas - 1, umbral_n0);

    cout << "\nResultados mediante Divide y Venceras (Umbral = " << umbral_n0 << "):" << endl;
    cout << "-> Primer Maximo (max1): " << resultados_dyv.max1 << endl;
    cout << "-> Segundo Maximo (max2): " << resultados_dyv.max2 << endl;

    return 0;
}

