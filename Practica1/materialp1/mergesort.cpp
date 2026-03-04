#include <cstdlib> // Para usar srand y rand
#include <chrono>
#include <cstring> // Para usar memcpy
#include <iostream>
using namespace std;


void fusionaMS(int *v, int posIni, int centro, int posFin, int *vaux) {

    int i= posIni;
    int j= centro;
    int k= 0;

    while (i<centro && j<=posFin) {
        if (v[i]<=v[j]) {
            vaux[k]= v[i];
            i++;
        } else {
            vaux[k]= v[j];
            j++;
        }
        k++;
    }

    while (i<centro) {
        vaux[k]= v[i];
        i++, k++;
    }
    while (j<=posFin) {
        vaux[k]= v[j];
        j++, k++;
    }

    memcpy(v+posIni, vaux, k*sizeof(int)); //pasa los elementos copiados al vector original
}
//Tiene una eficiencia de O(n) el fusionams

void MergeSort(int *v, int posIni, int posFin, int *vaux) {

    if (posIni>=posFin) return; //T(1)

    int centro= (posIni+posFin)/2; //T(1)

    MergeSort(v, posIni, centro, vaux);         //T(n/2)
    MergeSort(v, centro+1, posFin, vaux);       //T(n/2)
    fusionaMS(v, posIni, centro+1, posFin, vaux); //T(n)
}




int main(int argc, char *argv[]) {
	
	int *v;
    int *vaux;
	int n, i;
    chrono::time_point<std::chrono::high_resolution_clock> t0, tf; // Para medir el tiempo de ejecución
	unsigned long int semilla;
	
	if (argc != 3) {
		cerr << "Uso: " << argv[0] << " semilla tam\n";
		return -1;
	}
	
	semilla = atoi(argv[1]);
	srand(semilla);	// Inicializamos generador de no. aleatorios

	n= atoi(argv[2]);
	v= new int[n];
	vaux= new int[n];
	
	// Generamos vector aleatorio de prueba, con componentes entre 0 y n-1
	for (i= 0; i<n; i++)
		v[i]= rand()%n;
			
	t0= std::chrono::high_resolution_clock::now(); // Cogemos el tiempo en que comienza la ejecuciÛn del algoritmo
	MergeSort(v, 0, n-1, vaux); 
	tf= std::chrono::high_resolution_clock::now(); // Cogemos el tiempo en que finaliza la ejecuciÛn del algoritmo
	
	unsigned long tejecucion= std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();
	
	cout <<n<<" "<<tejecucion<<endl;
	
	delete [] v;
	delete [] vaux;
	
	return 0;
}
