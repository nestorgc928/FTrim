#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

//ESTRUCTURAS BASE PARA TODO EL PROBLEMA

//Struct para representar cada proyecto
struct Proyecto {
    int id;
    int coste;
    int beneficio;
    double ratio; //ratio es el beneficio/coste
};

//Struct para almacenar las estadisticas de cada algoritmo
struct Resultado {
    int mejor_beneficio = 0;
    vector<int> seleccion_optima;
    int nodos_generados = 0;
    int nodos_podados = 0;

    //Guardamos todas las combinaciones validas para imprimirlas al final
    vector<vector<int>> todas_las_selecciones;
    vector<int> todos_los_beneficios;
    vector<int> todos_los_costes;
};


/***************/
//BACKTRACKING
/***************/

//Funcion auxiliar para el backtracking de las tareas (para ocultar los detal
void backtracking_recursivo(const vector<Proyecto>& proyectos, int presupuesto_maximo, int nivel,
                            int coste_acumulado, int beneficio_acumulado, vector<int>& seleccion_actual, Resultado& res) {

    res.nodos_generados++;

    //Caso base: ya evaluados todos los proyectos
    if (nivel == proyectos.size()) {
        //Guardamos la combinacion para poder imprimirlas luego
        res.todas_las_selecciones.push_back(seleccion_actual);
        res.todos_los_beneficios.push_back(beneficio_acumulado);
        res.todos_los_costes.push_back(coste_acumulado);

        if (beneficio_acumulado > res.mejor_beneficio) {
            res.mejor_beneficio = beneficio_acumulado;
            res.seleccion_optima = seleccion_actual;
        }
        return;
    }

    //Opcion 1: incluir el proyecto actual (rama de "sí")
    //Poda de factibilidad: solo entramos si no se supera el presupuesto
    if (coste_acumulado + proyectos[nivel].coste <= presupuesto_maximo) {
        seleccion_actual.push_back(proyectos[nivel].id);

        backtracking_recursivo(proyectos, presupuesto_maximo, nivel+1,
                                coste_acumulado + proyectos[nivel].coste, beneficio_acumulado + proyectos[nivel].beneficio,
                                seleccion_actual, res);

        //Backtrack (deshacer la decicision): limpiamos para probar la otra rama
        seleccion_actual.pop_back();
    }
    else {
        res.nodos_podados++; //No hemos entrado porque el coste era excesivo
    }

    //Opcion 2: No incluir el proyecto actual (rama de "no")
    //Esta opcion siempre es factible (si antes no te habias pasado, ahora tampoco)
    backtracking_recursivo(proyectos, presupuesto_maximo, nivel+1, coste_acumulado,
                 beneficio_acumulado, seleccion_actual, res);
}

//Funcion principal de interfaz para el backtracking
Resultado ejecutar_backtracking(int presupuesto_maximo, const vector<Proyecto>& proyectos) {
    Resultado res;
    vector<int> seleccion_actual;

    backtracking_recursivo(proyectos, presupuesto_maximo, 0, 0, 0, seleccion_actual, res);

    return res;
}



/***************/
//BRANCH & BOUND
/***************/
struct Nodo {
    int nivel;  //Proyecto sobre el que estamos decidiendo
    int coste;  //Coste acumulado hasta ese nodo
    int beneficio; //Beneficio acumulado hasta ese nodo
    double cota_superior; //Beneficio maximo "soñado" si seguimos por aqui
    vector<int> seleccion; //Guarda los id de los proyectos elegidos en este camino

    //Operador para que la cola con prioridad nos de siempre el de mayor cota_superior
    bool operator<(const Nodo& otro) const {
        return cota_superior < otro.cota_superior;
    }
};

//Funcion auxiliar que calcula la cota superior (una estimacion optimista)
double calcular_cota_superior(Nodo actual, int n, int presupuesto, const vector<Proyecto>& p) {
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

//Funcion auxiliar que calcula la cota inferior inicial
//Usamos una estrategia greedy para obtener un beneficio inicial garantizado
int calcular_cota_inferior_inicial(int presupuesto, const vector<Proyecto>& p) {
    int beneficio_inicial = 0;
    int coste_actual = 0;

    //Asumimos que el vector p esta ordenado por ratio
    for (int i = 0; i < p.size(); ++i) {
        if (coste_actual + p[i].coste <= presupuesto) {
            coste_actual += p[i].coste;
            beneficio_inicial += p[i].beneficio;
        }
    }
    return beneficio_inicial;
}

//Funcion Branch&Bound para las tareas
Resultado ejecutar_branch_and_bound(int presupuesto_maximo, vector<Proyecto> proyectos) {

    Resultado res;

    //Antes de empezar ordenamos por ratio beneficio/coste descendente (usamos una funcion lambda)
    sort(proyectos.begin(), proyectos.end(), [](const Proyecto& a, const Proyecto& b) {
        return a.ratio > b.ratio;
    });

    //Calculamos la cota inferior inicial
    res.mejor_beneficio = calcular_cota_inferior_inicial(presupuesto_maximo, proyectos);

    priority_queue<Nodo> pq;
    int n = proyectos.size();

    //Nodo raiz (antes del primer proyecto)
    Nodo u;
    u.nivel = -1;
    u.coste = 0;
    u.beneficio = 0;
    u.cota_superior = calcular_cota_superior(u, n, presupuesto_maximo, proyectos);
    pq.push(u);

    while (!pq.empty()) {
        u = pq.top(); // Extraemos el nodo más prometedor
        pq.pop();
        res.nodos_generados++;

        //Condicion principal: solo generamos hijos si la cota superior supera a nuestro mejor beneficio actual
        if (u.cota_superior > res.mejor_beneficio) {
            Nodo v_si, v_no;
            v_si.nivel = u.nivel+1;
            v_no.nivel = u.nivel+1;

            //Vemos que no nos salimos de los limites del vector
            if (v_si.nivel < n) {
                //HIJO SI
                v_si.coste = u.coste + proyectos[v_si.nivel].coste;
                v_si.beneficio = u.beneficio + proyectos[v_si.nivel].beneficio;
                v_si.seleccion = u.seleccion;
                v_si.seleccion.push_back(proyectos[v_si.nivel].id);

                //Vemos si es factible el hijo si
                if (v_si.coste <= presupuesto_maximo) {
                    //vemos si es la mejor hasta ahora
                    if (v_si.beneficio > res.mejor_beneficio) {
                        res.mejor_beneficio = v_si.beneficio;
                        res.seleccion_optima = v_si.seleccion;
                    }

                    //Calculamos su potencial y vemos si lo metemos en la cola
                    v_si.cota_superior = calcular_cota_superior(v_si, n, presupuesto_maximo, proyectos);
                    if (v_si.cota_superior > res.mejor_beneficio) {
                        pq.push(v_si);
                    }
                    else {
                        res.nodos_podados++; //Poda por cota
                    }
                }
                else {
                    res.nodos_podados++; //poda por factibilidad (se pasa del coste)
                }

                //HIJO NO
                v_no.coste = u.coste;
                v_no.beneficio = u.beneficio;
                v_no.seleccion = u.seleccion;

                //Calculamos su potencial
                v_no.cota_superior = calcular_cota_superior(v_no, n, presupuesto_maximo, proyectos);
                if (v_no.cota_superior > res.mejor_beneficio) {
                    pq.push(v_no);
                }
                else {
                    res.nodos_podados++; //poda por cota
                }
            } //fin de verificacion del nivel
        }
        else {
            //si el nodo original que sacamos de la cola no es prometedor lo podamos y no generamos sus hijos
            res.nodos_podados++;
        }
    }

    return res;
}

/***********************/
//FUNCION MAIN DE PRUEBA
/***********************/
int main() {
    int P = 8;
    vector<Proyecto> lista = {
        {1, 2, 4, 4.0/2.0},
        {2, 3, 5, 5.0/3.0},
        {3, 5, 6, 6.0/5.0}
    };

    //Ejecutamos ambos de forma independiente
    Resultado res_bt = ejecutar_backtracking(P, lista);
    Resultado res_bb = ejecutar_branch_and_bound(P, lista);

    cout << "TODAS LAS COMBINACIONES VALIDAS (BACKTRACKING)" << endl;

    for (int i = 0; i < res_bt.todas_las_selecciones.size(); i++) {
        cout << "\tCombinacion " << i + 1 << ": { ";

        //Bucle para recorrer cada combinación individual
        for (int j = 0; j < res_bt.todas_las_selecciones[i].size(); j++) {
            cout << res_bt.todas_las_selecciones[i][j] << " ";
        }

        cout << "} | Beneficio: " << res_bt.todos_los_beneficios[i];
        cout << " | Coste: " << res_bt.todos_los_costes[i] << endl;
    }

    cout << endl;
    cout << "RESULTADOS BACKTRACKING" << endl;
    cout << "\tMejor beneficio: " << res_bt.mejor_beneficio << endl;
    cout << "\tProyectos elegidos: ";
    for (int i = 0; i < res_bt.seleccion_optima.size(); i++) {
        cout << res_bt.seleccion_optima[i] << " ";
    }
    cout << endl;
    cout << "\tNodos generados: " << res_bt.nodos_generados << endl;
    cout << "\tNodos podados: " << res_bt.nodos_podados << endl;

    cout << "-----------------------------------------" << endl;
    cout << "RESULTADOS BRANCH & BOUND" << endl;
    cout << "\tMejor beneficio: " << res_bb.mejor_beneficio << endl;
    cout << "\tProyectos elegidos: ";
    for (int i = 0; i < res_bb.seleccion_optima.size(); i++) {
        cout << res_bb.seleccion_optima[i] << " ";
    }
    cout << endl;
    cout << "\tNodos generados: " << res_bb.nodos_generados << endl;
    cout << "\tNodos podados: " << res_bb.nodos_podados << endl;


    return 0;
}