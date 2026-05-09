#include "knapsack.hpp" //Incluimos la declaracion del modulo C
#include <fstream> //Nos permite crear y escribir archivos de salida
#include <vector> //Nos permite trabajar con vectores
#include <cmath> //Nos permite usar round para redondear valores
#include <algorithm> //Nos permite usar sort y reverse
#include <iomanip> //Nos permite controlar decimales en la salida
#include <string> //Nos permite usar string

using namespace std;

//Estructura auxiliar que representa una solicitud como item de mochila
//peso representa el ancho de banda requerido
//valor representa el ingreso asociado a la solicitud
struct Item {
    string customerID;
    int peso;
    int valor;
    double ratio;
};

//Funcion auxiliar para imprimir una lista de items
static void imprimirItems(ofstream& out, const vector<Item>& items) {
    for (const Item& it : items) {
        out << it.customerID
            << " peso=" << it.peso
            << " valor=" << it.valor
            << " ratio=" << fixed << setprecision(4) << it.ratio
            << "\n";
    }
}

//Funcion auxiliar para imprimir solo los customerID seleccionados
static void imprimirIDs(ofstream& out, const vector<Item>& items) {
    if (items.empty()) {
        out << "Ninguna";
        return;
    }

    for (const Item& it : items) {
        out << it.customerID << " ";
    }
}

//Ejecuta el modulo C de Programacion Dinamica
//Recibe las solicitudes ya ordenadas desde el modulo A
void ejecutarModuloC(const vector<Solicitud>& solicitudesOrdenadas) {
    const int W = 500; //Capacidad maxima de ancho de banda
    vector<Item> items; //Aqui guardamos las primeras 50 solicitudes activas

    //Recorremos las solicitudes ordenadas
    for (const Solicitud& s : solicitudesOrdenadas) {
        //Solo se toman solicitudes activas, es decir Churn = No
        if (s.churn == "No") {
    
            int peso = static_cast<int>(round(s.totalCharges / 10.0));   // escalado para caber en W=500
            int valor = static_cast<int>(round(s.monthlyCharges * 10.0));
            
            double ratio = 0.0;
            if (peso > 0) {
                ratio = static_cast<double>(valor) / peso;
            }

            items.push_back({s.customerID, peso, valor, ratio});

            //Cuando ya tenemos 50 solicitudes activas detenemos el recorrido
            if (items.size() == 50) {
                break;
            }
        }
    }

    int n = static_cast<int>(items.size());

    //Creamos la tabla dp de tamaño (n+1) x (W+1)
    //guarda el mejor valor usando las primeras i solicitudes con capacidad w
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    //Llenamos la tabla por tabulacion
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            //Primero copiamos el caso donde no tomamos la solicitud actual
            dp[i][w] = dp[i - 1][w];

            //Si el peso cabe, comparamos contra tomar la solicitud actual
            if (items[i - 1].peso <= w) {
                dp[i][w] = max(
                    dp[i][w],
                    dp[i - 1][w - items[i - 1].peso] + items[i - 1].valor
                );
            }
        }
    }

    //Reconstruimos la solucion optima desde dp[n][W]
    vector<Item> seleccionados;
    int w = W;

    for (int i = n; i >= 1; i--) {
        //Si cambia frente a la fila anterior, significa que ese item fue tomado
        if (dp[i][w] != dp[i - 1][w]) {
            seleccionados.push_back(items[i - 1]);
            w -= items[i - 1].peso;
        }
    }

    //Invertimos porque reconstruimos desde el final hacia el inicio
    reverse(seleccionados.begin(), seleccionados.end());

    //Buscamos un contraejemplo con exactamente 3 solicitudes del conjunto usado
    bool encontrado = false;
    int capacidadContraejemplo = 0;
    int valorGreedy = 0;
    int valorOptimo = 0;
    vector<Item> trioContraejemplo;
    vector<Item> seleccionGreedy;
    vector<Item> seleccionOptima;

    for (int a = 0; a < n && !encontrado; a++) {
        for (int b = a + 1; b < n && !encontrado; b++) {
            for (int c = b + 1; c < n && !encontrado; c++) {
                vector<Item> trio = {items[a], items[b], items[c]};

                //Probamos capacidades formadas con pesos individuales y parejas
                vector<int> capacidades = {
                    trio[0].peso,
                    trio[1].peso,
                    trio[2].peso,
                    trio[0].peso + trio[1].peso,
                    trio[0].peso + trio[2].peso,
                    trio[1].peso + trio[2].peso
                };

                for (int cap : capacidades) {
                    if (cap <= 0) {
                        continue;
                    }

                    //Aplicamos codicioso por mayor ratio valor/peso
                    vector<Item> greedy = trio;
                    sort(greedy.begin(), greedy.end(), [](const Item& x, const Item& y) {
                        return x.ratio > y.ratio;
                    });

                    int pesoG = 0;
                    int valorG = 0;
                    vector<Item> selG;

                    for (const Item& it : greedy) {
                        if (pesoG + it.peso <= cap) {
                            pesoG += it.peso;
                            valorG += it.valor;
                            selG.push_back(it);
                        }
                    }

                    //Calculamos el optimo real probando las 8 combinaciones posibles
                    int mejorValor = 0;
                    vector<Item> mejorSel;

                    for (int mask = 0; mask < 8; mask++) {
                        int pesoActual = 0;
                        int valorActual = 0;
                        vector<Item> actual;

                        for (int j = 0; j < 3; j++) {
                            if (mask & (1 << j)) {
                                pesoActual += trio[j].peso;
                                valorActual += trio[j].valor;
                                actual.push_back(trio[j]);
                            }
                        }

                        if (pesoActual <= cap && valorActual > mejorValor) {
                            mejorValor = valorActual;
                            mejorSel = actual;
                        }
                    }

                    //Si el codicioso da menor valor, encontramos el contraejemplo
                    if (valorG < mejorValor) {
                        encontrado = true;
                        capacidadContraejemplo = cap;
                        valorGreedy = valorG;
                        valorOptimo = mejorValor;
                        trioContraejemplo = trio;
                        seleccionGreedy = selG;
                        seleccionOptima = mejorSel;
                        break;
                    }
                }
            }
        }
    }

    //Creamos el archivo de salida del modulo C
    ofstream out("results/asignacion_bw.txt");

    out << "MODULO C - ASIGNACION DE ANCHO DE BANDA\n\n";
    out << "Capacidad W: " << W << "\n";
    out << "Solicitudes consideradas: " << n << "\n";
    out << "Dimensiones tabla dp: " << n + 1 << " x " << W + 1 << "\n\n";

    out << "Solucion optima por Programacion Dinamica:\n";
    out << "Valor optimo total: " << dp[n][W] << "\n";
    out << "Numero de solicitudes seleccionadas: " << seleccionados.size() << "\n";
    out << "Solicitudes seleccionadas:\n";
    imprimirItems(out, seleccionados);

    out << "\nPrimeras 50 solicitudes activas usadas:\n";
    out << "customerID,peso,valor,ratio\n";
    for (const Item& it : items) {
        out << it.customerID << ","
            << it.peso << ","
            << it.valor << ","
            << fixed << setprecision(4) << it.ratio << "\n";
    }

    out << "\nContraejemplo codicioso con 3 solicitudes:\n";
    if (encontrado) {
        out << "Capacidad usada: " << capacidadContraejemplo << "\n";
        out << "Solicitudes del contraejemplo:\n";
        imprimirItems(out, trioContraejemplo);
        out << "\nComparacion:\n";
        out << "Enfoque | Solicitudes seleccionadas | Valor total | Optimo\n";
        out << "Codicioso ratio v/w | ";
        imprimirIDs(out, seleccionGreedy);
        out << "| " << valorGreedy << " | No\n";
        out << "Programacion Dinamica | ";
        imprimirIDs(out, seleccionOptima);
        out << "| " << valorOptimo << " | Si\n";
    } else {
        out << "No se encontro contraejemplo dentro de las primeras 50 solicitudes activas.\n";
    }

    out.close();
}
