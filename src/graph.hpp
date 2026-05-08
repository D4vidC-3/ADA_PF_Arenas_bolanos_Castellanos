//Con esto logramos que el archivo no se incluya mas de una vez durante la compilacion
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector> //Nos permite trabajar con vectores

//Estructura que representa una arista del grafo
//u y v son los nodos que conecta la arista
//peso es el costo de instalar esa conexion
struct Arista {
    int u;
    int v;
    int peso;
};

//Clase UnionFind para manejar conjuntos disjuntos
//La usamos en Kruskal para saber si una arista forma ciclo
class UnionFind {
private:
    std::vector<int> padre; //Guarda el representante de cada nodo
    std::vector<int> rango; //Ayuda a unir conjuntos sin hacer arboles muy altos

public:
    //Constructor que crea n conjuntos separados
    UnionFind(int n);

    //Busca el representante del conjunto donde esta x
    int find(int x);

    //Une los conjuntos donde estan x e y
    void unite(int x, int y);
};

//Construye el grafo completo a partir de los promedios de MonthlyCharges
//Devuelve todas las aristas entre los nodos del grafo
std::vector<Arista> construirGrafo(const std::vector<double>& promedios);

#endif
