#include "kruskal.hpp" //Incluimos la declaracion de Kruskal
#include <algorithm> //Nos permite ordenar las aristas por peso

//Funcion principal del algoritmo de Kruskal
//Selecciona aristas de menor peso siempre que no formen ciclo
std::vector<Arista> kruskal(int n, std::vector<Arista>& aristas, int& pesoTotal) {
    std::vector<Arista> mst; //Aqui guardamos las aristas seleccionadas

    //Ordenamos las aristas de menor a mayor peso
    std::sort(aristas.begin(), aristas.end(), [](const Arista& a, const Arista& b) {
        return a.peso < b.peso;
    });

    //Creamos UnionFind para controlar los componentes conectados
    UnionFind uf(n);
    pesoTotal = 0;

    //Recorremos las aristas ordenadas
    for (const Arista& a : aristas) {
        //Si los extremos estan en conjuntos diferentes, la arista no forma ciclo
        if (uf.find(a.u) != uf.find(a.v)) {
            uf.unite(a.u, a.v);
            mst.push_back(a);
            pesoTotal += a.peso;
        }
    }

    return mst;
}