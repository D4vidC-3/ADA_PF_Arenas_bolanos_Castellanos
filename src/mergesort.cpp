//mergesort
#include "mergesort.hpp" //Incluimos la declaracion de mergeSort para poder implementar el ordenamiento

//La idea principal de este ordenamiento es divide y venceras 
//Ya que queremos dividir el vector en partes mas pequeñas y luego las unimos ordenadas 
//Para esto nos basamos en tenure, que nos representa la prioridad de servicio por antiguedad

//Funcion auxiliar merge que recibe dos partes del vector ya ordenadas y las une nuevamente al vector original 
//Recibimos el vector por referencia para modificar directamente el vector original
//izquierda nos representa la posicion inicial de la parte que vamos a organizar 
//medio nos representa el punto donde termina la parte izquierda
//derecha nos representa la posicion final de la parte que vamos a organizar 
void merge(std::vector<Solicitud>& solicitudes, int izquierda, int medio, int derecha) {
    
    //Tamaño del subarreglo izquierdo incluyendo el medio
    int n1 = medio - izquierda + 1;

    //Tamaño del subarreglo derecho
    int n2 = derecha - medio;

    //Creamos vectores temporales de tipo Solicitud para guardar las dos mitades
    std::vector<Solicitud> L(n1);//mitad izquierda
    std::vector<Solicitud> R(n2);//Mitad derecha

    //Copiamos la mitad izquierda del vector original en L
    //n1 -> tamaño del subarreglo izq
    for (int i = 0; i < n1; i++){
        L[i] = solicitudes[izquierda + i];//Usamos izquierda+i porque la parte que copiamos no siempre empieza desde la posicion 0 
    }
    //Copiamos la mitad derecha del vector original en R
    //n2 -> tamaño del subarreglo der
    for (int j = 0; j < n2; j++){
        R[j] = solicitudes[medio + 1 + j]; //Empezamos despues de medio, porque la parte derecha inicia en medio+1
    }
    //Indices para recorrer L, R y el vector original
    int i = 0; //Recorrer L
    int j = 0; //Recorrer R
    int k = izquierda; //Posicion donde escribiremos en el vector original

    //Comparamos mientras ambos subarreglos tengan elementos 
    while (i < n1 && j < n2) {

        //Ordenamos de forma descendente utilizando el campo tenure como criterio de comparacion
        //Recordemos que cada posición del vector guarda una Solicitud completa, por eso al asignar
        //solicitudes[k] = L[i] o solicitudes[k] = R[j], estamos moviendo toda la solicitud
        //Si los tenure son iguales, tomamos primero el elemento de L para conservar la estabilidad del mergeSort

        //Si la solicitud de L tiene mayor o igual tenure, la copiamos completa en la posicion k del vector original
        if (L[i].tenure >= R[j].tenure) { 
            solicitudes[k] = L[i]; 
            i++;//Avanzamos al siguiente elemento de L porque L[i] ya lo ubicamos ordenadamente
        } else {
            //Si la solicitud de R tiene mayor tenure, la copiamos completa en la posicion k del vector original
            solicitudes[k] = R[j];  
            j++; //Avanzamos al siguiente elemento de R porque R[j] ya lo ubicamos ordenadamente
        }

        k++; //Avanzamos a la siguiente posicion del vector original 
    }

    //Si sobran elementos en L, se copian directamente
    //ya que estos ya estan ordenados por las llamadas recursivas 
    while (i < n1) {
        solicitudes[k] = L[i];
        i++;//Avanzamos al siguiente elemento de L
        k++;//Avanzamos a la siguiente posicion del vector original 
    }

    //Si sobran elementos en R, se copian directamente
    //ya que estos ya estan ordenados por las llamadas recursivas
    while (j < n2) {
        solicitudes[k] = R[j];
        j++;//Avanzamos al siguiente elemento de R
        k++;//Avanzamos a la siguiente posicion del vector original 
    }
}

//Funcion recursiva para dividir al vector en mitades hasta llegar a subarreglos de un solo elemento
//Luego utilizamos merge para unir estos subarreglos ya ordenadas en orden descendente por tenure  
void mergeSort(std::vector<Solicitud>& solicitudes, int izquierda, int derecha) {
    //Caso base: si el subarreglo tiene 0 o 1 elemento, ya esta ordenado
    if (izquierda >= derecha) return;

    //Calculamos el punto medio del subarreglo
    int medio = izquierda + (derecha - izquierda) / 2;

    //Ordenamos recursivamente la mitad izquierda
    mergeSort(solicitudes, izquierda, medio);

    //Ordenamos recursivamente la mitad derecha
    mergeSort(solicitudes, medio + 1, derecha);

    //Unimos ambas mitades ya ordenadas
    merge(solicitudes, izquierda, medio, derecha);
}