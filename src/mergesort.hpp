//mergesort
#ifndef MERGESORT_HPP
#define MERGESORT_HPP

#include "parser.hpp" //Incluimos Solicitud, ya que mergeSort ordena un vector de solicitudes
#include <vector> //Para usar vectores
//Ordenamos el vector de solicitudes por tenure de forma descendente 
//Recibimos el vector por referencia para modificar de una vez el original
void mergeSort(std::vector<Solicitud>& solicitudes, int izquierda, int derecha);

#endif