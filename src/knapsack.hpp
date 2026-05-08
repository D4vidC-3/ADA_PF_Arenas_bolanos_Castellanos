//Con esto logramos que el archivo no se incluya mas de una vez durante la compilacion
#ifndef KNAPSACK_HPP
#define KNAPSACK_HPP

#include "parser.hpp" //Incluimos la estructura Solicitud
#include <vector> //Nos permite trabajar con vectores

//Ejecuta el modulo C con las solicitudes ordenadas por tenure descendente
//Toma las primeras 50 solicitudes activas y aplica mochila 0-1 por tabulacion
void ejecutarModuloC(const std::vector<Solicitud>& solicitudesOrdenadas);

#endif
