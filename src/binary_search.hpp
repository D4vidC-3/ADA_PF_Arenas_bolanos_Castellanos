//binary_search
//Evitamos que el archivo .hpp se incluya mas de una vez durante la compilacion
#ifndef BINARY_SEARCH_HPP
#define BINARY_SEARCH_HPP

#include <vector> //Para usar vectores
#include "parser.hpp" //Incluimos la estructura Solicitud para poder buscar dentro del vector

//Funcion que nos retorna un entero
//La funcion recibe el vector ya ordenado como const ya que solo queremos buscar dentro de este
//Los limites izquierda, derecha, y el tenure k que queremos buscar
//Si encontramos una solicitud con tenure == k retornamos la posicion dentro del vector
//Si no encontramos ninguna solicitud con ese tenure retornamos -1
int busquedaBinariaRecursivaExacta(
    const std::vector<Solicitud>& solicitudes,
    int izquierda,
    int derecha,
    int k
);

#endif