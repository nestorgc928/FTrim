// Sea un vector v de enteros, de tamaño n>0, se define una inversión como un par de índices (i,j)
// tales que:
// ● i<j
// ● v[i] > v[j]
// Se trata de calcular el número total de inversiones existentes en el vector.

#include <iostream>
#include <vector>

using namespace std;

//Algoritmo para calcular el número de inversiones de un vector por la fuerza bruta

int calcular_n_inversiones_fbruta(vector<int> &v, int n){
    int n_inversiones = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (v[i] > v[j]) {
                n_inversiones++;
            }
        }
    }
    return n_inversiones;
}

//Algoritmo para calcular el número de inversiones de un vector mediande DyV

int contar_dividir(vector<int> &v, int inicio, int mitad, int fin) {
    int n_inversiones=0;
    vector<int> v1, v2;

    for (int i = inicio; i <= mitad; i++) {
        v1.push_back(v[i]);
    }
    for (int i = mitad + 1; i <= fin; i++) {
        v2.push_back(v[i]);
    }

    for (int i = 0; i < v1.size(); i++) {
        for (int j = 0; j < v2.size(); j++) {
            if (v1[i] > v2[j]) {
                n_inversiones++;
            }
        }
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

int main () {
    vector<int>v ={0,5,3,2,4};
    int n=v.size();

    cout << calcular_n_inversiones_fbruta(v, n) << endl;
    cout << calcular_n_inversiones_dyv(v, 0, n-1) << endl;

    return 0;
}