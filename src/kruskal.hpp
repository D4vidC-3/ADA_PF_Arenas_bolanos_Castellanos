//Con esto logramos que el archivo no se incluya mas de una vez durante la compilacion
#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include "graph.hpp" //Incluimos Arista y UnionFind
#include <vector> //Nos permite trabajar con vectores

//Aplica Kruskal sobre un grafo con n nodos y un vector de aristas
//Devuelve las aristas que forman el arbol de expansion minima
//pesoTotal se pasa por referencia para guardar el peso total del MST
std::vector<Arista> kruskal(int n, std::vector<Arista>& aristas, int& pesoTotal);

#endif
