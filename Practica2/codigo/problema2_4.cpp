// Sea un vector v de enteros, de tamaño n>0, se define una inversión como un par de índices (i,j)
// tales que:
// ● i<j
// ● v[i] > v[j]
// Se trata de calcular el número total de inversiones existentes en el vector.

#include <iostream>
#include <vector>
#include <climits> //Lo añadimos para poder usar INT_MIN
#include <algorithm>
#include <chrono>   // Para medir el tiempo
#include <cstdlib>  // Para rand() y srand()
#include <ctime>    // Para time()

using namespace std;
using namespace std::chrono;

//Algoritmo para calcular el número de inversiones de un vector por la fuerza bruta

int calcular_n_inversiones_fbruta(vector<int> &v){
    int n_inversiones = 0;
    int n = v.size();

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (v[i] > v[j]) {
                n_inversiones++;
            }
        }
    }
    return n_inversiones;
}

//Algoritmo para calcular el número de inversiones de un vector mediande DyV. Para que el algoritmo sea eficiente tenemos
// que ordenar el vector en la función

int contar_dividir(vector<int> &v, int inicio, int mitad, int fin) {
    int n_inversiones=0;
    vector<int> vaux;
    int i = inicio;
    int j = mitad+1;
    while (i<=mitad && j<=fin) {
        if (v[i] <= v[j]) {
            vaux.push_back(v[i]);
            i++;
        } else {
            vaux.push_back(v[j]);
            // como v[j] es menor que todos los elementos que hay en v[1] entonces sumamos todas las inversiones de golpe
            n_inversiones += mitad - i + 1;
            j++;
        }

    }

    while (i<=mitad) {
        vaux.push_back(v[i]);
        i++;
    }
    while (j<=fin) {
        vaux.push_back(v[j]);
        j++;
    }

    for (int k = inicio; k <= fin; k++) {
        v[k] = vaux[k - inicio];
    }
    return n_inversiones;
}

int calcular_n_inversiones_dyv(vector<int> &v, int inicio, int fin) {
    int n_inversiones = 0;

    if (inicio < fin) {
        int mitad=inicio+(fin - inicio)/2;
        n_inversiones+=calcular_n_inversiones_dyv(v,inicio,mitad);
        n_inversiones+=calcular_n_inversiones_dyv(v,mitad+1,fin);
        n_inversiones+=contar_dividir(v,inicio,mitad,fin);
    }

    return n_inversiones;
}

//Función para generar vectores aleatorios de n elementos
vector<int> GenerarVectorAleatorio(int n) {
    //Inicializamos un vector de 'n' elementos
    vector<int> v(n);

    for (int i = 0; i < n; i++) {
        v[i] = rand() % 100000; // Números aleatorios entre 0 y 99999
    }
    return v;
}

//EJECUTAR ESTE MAIN CON ./problema2_4 > ../tiempos_y_graficas/datos/tiempos_generales2_4.csv

int main () {
    // vector<int>v ={0,5,3,2,4};
    // int n=v.size();
    //
    // cout << calcular_n_inversiones_fbruta(v) << endl;
    // cout << calcular_n_inversiones_dyv(v, 0, n-1) << endl;


    srand(time(NULL));

    // Configuraciones de nuestra prueba
    int salto = 500;    //Incremento del tamaño del vector en cada iteración
    int max_N = 20000;   //Límite de elementos a probar

    // Imprimimos la cabecera en formato CSV (separador: punto y coma)
    cout << "Elementos_N;Tiempo_FB_ms;Tiempo_DyV_ms" << endl;

    // Bucle para probar tamaños de problema crecientes
    for (int N = 500; N <= max_N; N += salto) {
        // Generamos el vector de prueba para la iteración actual
        vector<int> v_prueba = GenerarVectorAleatorio(N);

        //Cronometramos la Fuerza Bruta
        auto inicio_fb = high_resolution_clock::now();

        int resultado = calcular_n_inversiones_fbruta(v_prueba);

        auto fin_fb = high_resolution_clock::now();
        auto duracion_fb = duration_cast<milliseconds>(fin_fb - inicio_fb).count();

        //Cronometramos Divide y Vencerás
        auto inicio_dyv = high_resolution_clock::now();
        calcular_n_inversiones_dyv(v_prueba, 0, v_prueba.size()-1);

        auto fin_dyv = high_resolution_clock::now();
        auto duracion_dyv = duration_cast<milliseconds>(fin_dyv - inicio_dyv).count();

        // Salida de resultados
        cout << N << ";" << duracion_fb << ";" << duracion_dyv << endl;
    }

    return 0;
}