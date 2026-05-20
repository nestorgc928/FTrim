#include <iostream>
#include <vector>
using namespace std;

bool maximizar_cajas(vector<int> &s, vector<int> &n, int M, vector<int> &resultados) {
    int T = s.size();
    vector<vector<int>> dp(T+1, vector<int>(M+1, -1));

    //DP es la matriz que guarda los datos del dynamic programming, y dp[i][j] representa el máximo número de cajas
    // para obtener un peso de j kg, con -1 como valor imposible
    dp[0][0] = 0;

    //Se rellena la matriz con todas las combinaciones posibles
    for (int i = 1; i <= T; i++) {
        for (int j = 0; j <= M; j++) {
            dp[i][j] = -1;

            int max_cajas_posibles=min(n[i-1],j/s[i-1]);
            for (int k = 0; k<=max_cajas_posibles; k++) {
                int p_anterior=j-k*s[i-1];
                bool posible = dp[i-1][p_anterior]!=-1;
                //se ve si el nuevo número de cajas es mayor que el anterior
                if (posible && dp[i - 1][p_anterior] + k > dp[i][j]) {
                    dp[i][j] = dp[i - 1][p_anterior] + k;
                }
            }
        }
    }

    //Primero se obtiene si la suma objetivo M es posible de alcanzar
    if (dp[T][M]==-1) {
        return false;
    }

    // Ahora vemos cuál es la combinación que maximiza el número de cajas mediante backtracking
    int p_actual = M;
    for (int i = T; i > 0; i--) {
        int max_cajas_posibles=min(n[i - 1], p_actual / s[i - 1]);
        for (int j = 0; j <= max_cajas_posibles; j++) {
            int p_anterior = p_actual - j * s[i - 1];
            bool posible = dp[i-1][p_anterior]!=-1;
            if (posible && dp[i - 1][p_anterior] + j == dp[i][p_actual]) {
                resultados[i - 1] = j;
                p_actual = p_anterior;
                break;
            }
        }
    }
    return true;
}

int main() {
    vector<int> s={3,4};
    vector<int> n={3,2};
    vector<int> resultados(s.size(),0);
    int M = 14;
     if (!maximizar_cajas(s,n,M, resultados)) {
         cout << "El peso " << M << " es imposible de alcanzar" << endl;
     }
    else {
        cout << "Peso inicial: " << M << endl;
        cout << "Cajas:" << endl;

        for (int i = 0; i < resultados.size(); i++) {
            if (resultados[i] > 0) cout << "Se han usado " << resultados[i] << " cajas de " << s[i] << " kg" << endl;
        }
    }
    return 0;
}
