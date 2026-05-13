#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;


/***************/
//BRANCH & BOUND
/***************/

//Struct para representar cada proyecto
struct Proyecto {
    int id;
    int coste;
    int beneficio;
    int ratio; //ratio es el beneficio/coste
};

//Variables
int presupuesto_maximo;
int mejor_beneficio = 0;
vector<int> seleccion_optima;
vector<int> seleccion_actual;

//Variables para las estadisticas de la memoria
int nodos_generados = 0;
int nodos_podados = 0;

//Funcion para el backtracking de las tareas
void backtracking(const vector<Proyecto>& proyectos, int nivel, int coste_acumulado, int beneficio_acumulado) {
    nodos_generados++;

    //Caso base:
    if (nivel == proyectos.size()) {
        if (beneficio_acumulado > mejor_beneficio) {
            mejor_beneficio = beneficio_acumulado;
            seleccion_optima = seleccion_actual;
        }
        return;
    }

    //Opcion 1: incluir el proyecto actual (rama de "sí")
    //Poda de factibilidad: solo entramos si no se supera el presupuesto
    if (coste_acumulado + proyectos[nivel].coste <= presupuesto_maximo) {
        seleccion_actual.push_back(proyectos[nivel].id);

        backtracking(proyectos, nivel+1, coste_acumulado + proyectos[nivel].coste, beneficio_acumulado + proyectos[nivel].beneficio);

        //Backtrack (deshacer la decicision): limpiamos para probar la otra rama
        seleccion_actual.pop_back();
    }
    else {
        nodos_podados++; //No hemos entrado porque el coste era excesivo
    }

    //Opcion 2: No incluir el proyecto actual (rama de "no")
    //Esta opcion siempre es factible (si antes no te habias pasado, ahora tampoco)
    backtracking(proyectos, nivel+1, coste_acumulado, beneficio_acumulado);
}

/***************/
//BRANCH & BOUND
/***************/
struct Nodo {
    int nivel;  //Proyecto sobre el que estamos decidiendo
    int coste;  //Coste acumulado hasta ese nodo
    int beneficio; //Beneficio acumulado hasta ese nodo
    double cota_superior; //Beneficio maximo "soñado" si seguimos por aqui

    //Operador para que la cola con prioridad nos de siempre el de mayor cota_superior
    bool operator<(const Nodo& otro) const {
        return cota_superior < otro.cota_superior;
    }
};

//Funcion auxiliar que calcula la cota (una estimacion optimista)
double calcular_cota(Nodo actual, int n, int presupuesto, const vector<Proyecto>& p) {
    if (actual.coste >= presupuesto) return 0;

    double beneficio_limite = actual.beneficio;
    int j = actual.nivel +1;
    int coste_total = actual.coste;

    //Añadimos proyectos enteros mientras quepan
    while (j < n && coste_total + p[j].coste <= presupuesto) {
        coste_total += p[j].coste;
        beneficio_limite += p[j].beneficio;
        j++;
    }

    //si todavia queda hueco, partimos el siguiente proyecto
    if (j < n) {
        beneficio_limite += (presupuesto - coste_total) * p[j].ratio;
    }

    return beneficio_limite;
}

//Funcion Branch&Bound para las tareas
void branch_and_bound(int presupuesto, vector<Proyecto>& proyectos) {
    //Antes de empezar ordenamos por ratio beneficio/coste descendente
    sort(proyectos.begin(), proyectos.end(), [](const Proyecto& a, const Proyecto& b) {
        return a.ratio > b.ratio;
    });

    priority_queue<Nodo> pq;
    int mejor_beneficio = 0;
    int n = proyector.size();

    //Nodo raiz (antes del primer proyecto)
    Nodo u, v;
    u.nivel = -1;
    u.coste = 0;
    u.beneficio = 0;
    u.cota_superior = calcular_Cota(u, n, presupuesto, proyectos);
    pq.push(u);

    while (!pq.empty()) {
        u = pq.top(); // Extraemos el nodo más prometedor
        pq.pop();

        //si el nodo actual ya no puede superar al mejor beneficio, lo ignoramos
        if (u.cotaSuperior <= mejorBeneficio) continue;

        //pasamos al siguiente nivel (siguiente proyecto)
        v.nivel = u.nivel + 1;

        //HIJO 1: INCLUIR el proyecto actual
        v.coste = u.coste + proyectos[v.nivel].coste;
        v.beneficio = u.beneficio + proyectos[v.nivel].beneficio;

        if (v.coste <= presupuesto) {
            if (v.beneficio > mejorBeneficio) {
                mejorBeneficio = v.beneficio; //Actualizamos el mejor
            }
            v.cotaSuperior = calcularCota(v, n, presupuesto, proyectos);
            if (v.cotaSuperior > mejorBeneficio) {
                pq.push(v);
            }
        }

        //HIJO 2: NO INCLUIR el proyecto actual
        v.coste = u.coste;
        v.beneficio = u.beneficio;
        v.cotaSuperior = calcularCota(v, n, presupuesto, proyectos);

        if (v.cotaSuperior > mejorBeneficio) {
            pq.push(v);
        }
    }

    return mejorBeneficio;
}