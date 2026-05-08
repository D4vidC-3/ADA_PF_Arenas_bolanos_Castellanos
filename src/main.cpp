#include <iostream> //Mostrar resultados en pantalla 
#include <vector>  //Para usar vectores 
#include "parser.hpp" //Incluimos la estructura Solicitud y la funcion leerCSV 
#include "mergesort.hpp" //Incluimos la funcion mergeSort para ordenar las solicitudes por tenure
#include "binary_search.hpp" //Incluimos la busqueda binaria recursiva exacta por tenure

//El programa recibe argumentos desde la terminal
//argv[0] guarda el nombre del ejecutable que sera: ./ada_pf
//argv[1] debe guardar la ruta del CSV que es: data/WA_Fn-UseC_-Telco-Customer-Churn.csv
int main(int argc, char* argv[]) {
    //Verificamos que el usuario haya pasado la ruta del CSV como argumento
    //Si no se pasa, no existe argv[1] por eso mostramos un mensaje de uso 
    //Esto ayuda a que el usuario sepa como ejecutar correctamente el programa
    if (argc < 2) {
        std::cout << "Uso: ./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv\n";
        return 1;
    }

    //Guardamos en rutaCSV la ruta que el usuario paso por la terminal
    //Ya que esta ruta se pasa a leerCSV para abrir y leer el archivo
    std::string rutaCSV = argv[1];

    //Variable para contar cuantos registros tienen TotalCharges vacio
    int totalChargesNulos = 0;

    //Leemos el archivo CSV y guardamos las solicitudes cargadas
    std::vector<Solicitud> solicitudes = leerCSV(rutaCSV, totalChargesNulos);

    //Mostramos los resultados principales del parseo
    std::cout << "Registros cargados: " << solicitudes.size() << std::endl;
    std::cout << "Registros con TotalCharges vacio: " << totalChargesNulos << std::endl;

    //Probamos con los primeros registros
    std::cout << "\nPrimeras solicitudes cargadas:\n";

    //solicitudes.size() nos devuelve un size_t(tipo de tamaño de vectores), por eso lo convertimos a int con static_cast
    //Lo convertimos a int para evitar compararlo con size_t
    for (int i = 0; i < 5 && i < static_cast<int>(solicitudes.size()); i++) {
        std::cout << "CustomerID: " << solicitudes[i].customerID
                  << " || Tenure: " << solicitudes[i].tenure
                  << " || MonthlyCharges: " << solicitudes[i].monthlyCharges
                  << " || TotalCharges: " << solicitudes[i].totalCharges
                  << " || Churn: " << solicitudes[i].churn
                  << std::endl;
    }

    //Verificamos que existan solicitudes antes de llamar a mergeSort
    if (solicitudes.empty()) {
        std::cout << "\nNo hay solicitudes para ordenar.\n"; 
        return 1;//Si no existen solicitudes detenemos el programa
    }

    //Aplicamos MergeSort sobre el vector de solicitudes
    //El ordenamiento lo hacemos por tenure de forma descendente
    //Como solicitudes se pasa por referencia ordenamos directamente el vector original
    mergeSort(solicitudes, 0, static_cast<int>(solicitudes.size()) - 1);

    //Primeras solicitudes ya ordenadas por tenure
    std::cout << "\nPrimeras solicitudes despues de ordenar por tenure descendente:\n";

    //Volvemos a utilizar el mismo ciclo para verificar que ordenamos de forma correcta
    for (int i = 0; i < 5 && i < static_cast<int>(solicitudes.size()); i++) {
        std::cout << "CustomerID: " << solicitudes[i].customerID
                  << " || Tenure: " << solicitudes[i].tenure
                  << " || MonthlyCharges: " << solicitudes[i].monthlyCharges
                  << " || TotalCharges: " << solicitudes[i].totalCharges
                  << " || Churn: " << solicitudes[i].churn
                  << std::endl;
    }

    //Consultas del modulo A
    //Ahora la busqueda debe encontrar tenure exactamente igual a k
    std::vector<int> consultas = {72, 60, 45, 30, 12};

    //Recorremos cada valor de k para probar la busqueda binaria
    std::cout << "\nResultados de busqueda binaria exacta por tenure:\n";

    for (int k : consultas) {
        //Buscamos una solicitud cuyo tenure sea exactamente igual a k
        //Si hay varias solicitudes con el mismo tenure, la funcion nos puede retornar cualquiera
        int posicion = busquedaBinariaRecursivaExacta(
            solicitudes,
            0,
            static_cast<int>(solicitudes.size()) - 1,
            k
        );

        //Mostramos el resultado de la consulta actual
        std::cout << "k = " << k << " -> ";

        //Si posicion es diferente de -1 significa que encontramos una solicitud valida
        if (posicion != -1) {
            std::cout << "CustomerID: " << solicitudes[posicion].customerID
                      << " || Tenure: " << solicitudes[posicion].tenure
                      << std::endl;
        } else {
            std::cout << "No encontrado" << std::endl; //Si la posicion es -1 entonces no encontramos nigun tenure == k
        }
    }


    return 0;
}