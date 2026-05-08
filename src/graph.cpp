#include "graph.hpp" //Incluimos la estructura Arista y la clase UnionFind
#include <cmath> //Nos permite usar floor para calcular el peso de cada arista

//Constructor de UnionFind
//Cada nodo empieza siendo su propio representante
UnionFind::UnionFind(int n) {
    padre.resize(n);
    rango.resize(n, 0);

    //Inicializamos cada nodo como un conjunto independiente
    for (int i = 0; i < n; i++) {
        padre[i] = i;
    }
}

//Funcion find para buscar el representante de un conjunto
//Tambien aplica compresion de caminos para acelerar futuras busquedas
int UnionFind::find(int x) {
    //Si x no es su propio padre, seguimos buscando hacia arriba
    if (padre[x] != x) {
        padre[x] = find(padre[x]);
    }

    return padre[x];
}

//Funcion unite para unir dos conjuntos diferentes
//Se usa union por rango para mantener eficiente la estructura
void UnionFind::unite(int x, int y) {
    int raizX = find(x);
    int raizY = find(y);

    //Si tienen la misma raiz, ya estan en el mismo conjunto
    if (raizX == raizY) {
        return;
    }

    //Unimos el arbol de menor rango debajo del de mayor rango
    if (rango[raizX] < rango[raizY]) {
        padre[raizX] = raizY;
    } else if (rango[raizX] > rango[raizY]) {
        padre[raizY] = raizX;
    } else {
        padre[raizY] = raizX;
        rango[raizX]++;
    }
}

//Funcion para construir el grafo completo
//Cada par de nodos u,v con u < v genera una arista
std::vector<Arista> construirGrafo(const std::vector<double>& promedios) {
    std::vector<Arista> aristas;
    int n = static_cast<int>(promedios.size());

    //Recorremos todos los pares posibles de nodos
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            //La practica define el peso como piso del promedio de u mas promedio de v
            int peso = static_cast<int>(std::floor(promedios[u] + promedios[v]));
            aristas.push_back({u, v, peso});
        }
    }

    return aristas;
}
