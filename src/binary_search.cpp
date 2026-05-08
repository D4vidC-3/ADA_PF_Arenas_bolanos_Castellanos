//binary_search
#include "binary_search.hpp" //Incluimos la declaracion de la busquedaBinariaRecursivaExacta

int busquedaBinariaRecursivaExacta(const std::vector<Solicitud>& solicitudes,int izquierda,int derecha,int k){
    //Caso base:
    //Si izquierda es mayor a derecha significa que ya no queda rango donde buscar
    //Retornamos -1 si no encontramos nada 
    if (izquierda > derecha) {
        return -1;
    }

    //Calculamos la mitad del rango actual
    //Evitamos problemas que podrian aparecer si izquierda + derecha fuera muy grande
    int medio = izquierda + (derecha - izquierda) / 2; //Nos queremos ubicar en el medio ya que de eso se trata la busqueda binaria

    //Si el tenure de la mitad es exactamente igual a k, ya encontramos una solicitud valida
    //Si existen varias solicitudes con este tenure, retornamos cualquiera
    if (solicitudes[medio].tenure == k) {
        return medio;
    }

    //El vector esta ordenado de forma descendente por tenure

    //Si el tenure del medio es menor que k, entonces el valor que buscamos debe estar a la izquierda
    //Ya que a la izquierda estan los tenure mas grandes
    if (solicitudes[medio].tenure < k) {
        return busquedaBinariaRecursivaExacta(solicitudes, izquierda, medio - 1, k);
    }

    //Si el tenure del medio es mayor que k, entonces el valor buscado debe estar a la derecha
    //Ya que a la derecha estan los tenure mas pequeños
    return busquedaBinariaRecursivaExacta(solicitudes, medio + 1, derecha, k);
}