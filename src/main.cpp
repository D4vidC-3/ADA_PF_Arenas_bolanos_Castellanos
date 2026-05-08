#include <iostream> //Mostrar resultados en pantalla
#include <fstream> //Crear y escribir archivos de salida
#include <vector>  //Para usar vectores
#include <chrono> //Medir tiempos de ejecucion
#include <iomanip> //Mostrar tiempos con dos decimales
#include <filesystem> //Crear la carpeta results si no existe
#include <cmath> //Usar log2 para calcular n log n
#include "parser.hpp" //Incluimos la estructura Solicitud y la funcion leerCSV
#include "mergesort.hpp" //Incluimos la funcion mergeSort para ordenar las solicitudes por tenure
#include "binary_search.hpp" //Incluimos la busqueda binaria recursiva exacta por tenure

//Funcion para guardar las solicitudes ordenadas en el archivo solicitado
//El archivo queda en la carpeta results con el nombre solicitudes_ordenadas.csv
void guardarSolicitudesOrdenadas(const std::vector<Solicitud>& solicitudes) {
    std::ofstream salida("results/solicitudes_ordenadas.csv");

    salida << "customerID,tenure,MonthlyCharges,TotalCharges,Churn\n";

    for (const Solicitud& s : solicitudes) {
        salida << s.customerID << ","
               << s.tenure << ","
               << std::fixed << std::setprecision(2) << s.monthlyCharges << ","
               << std::fixed << std::setprecision(2) << s.totalCharges << ","
               << s.churn << "\n";
    }

    salida.close();
}

//Funcion para medir el tiempo de ejecucion de MergeSort
//Recibe una copia del vector para no modificar las solicitudes originales
double medirTiempoMergeSort(std::vector<Solicitud> datos) {
    auto inicio = std::chrono::high_resolution_clock::now();

    if (!datos.empty()) {
        mergeSort(datos, 0, static_cast<int>(datos.size()) - 1);
    }

    auto fin = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duracion = fin - inicio;
    return duracion.count();
}

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

    //Creamos la carpeta results si no existe
    //En esta carpeta se guardan las salidas pedidas del modulo A
    std::filesystem::create_directory("results");

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

    //Verificamos que existan solicitudes antes de llamar a mergeSort
    if (solicitudes.empty()) {
        std::cout << "\nNo hay solicitudes para ordenar.\n";
        return 1; //Si no existen solicitudes detenemos el programa
    }

    //Abrimos el archivo donde guardaremos los resultados de las busquedas y los tiempos
    std::ofstream busquedas("results/busquedas_A.txt");

    busquedas << "MODULO A\n\n";
    busquedas << "Registros cargados: " << solicitudes.size() << "\n";
    busquedas << "Registros con TotalCharges nulo: " << totalChargesNulos << "\n\n";

    //Tamaños pedidos para el analisis empirico del ordenamiento
    std::vector<int> tamanos = {
        1000,
        3500,
        static_cast<int>(solicitudes.size())
    };

    busquedas << "ANALISIS EMPIRICO DE MERGESORT\n";
    busquedas << "n,tiempo_ms,nlogn_aprox\n";

    //Medimos el tiempo de MergeSort para cada tamaño solicitado
    for (int n : tamanos) {
        //Creamos una copia con los primeros n registros para medir el tiempo de ordenamiento
        std::vector<Solicitud> muestra(solicitudes.begin(), solicitudes.begin() + n);

        //Medimos cuanto tarda MergeSort con esta muestra
        double tiempo = medirTiempoMergeSort(muestra);

        //Guardamos el tamaño, el tiempo y n log n como referencia para la tabla
        busquedas << n << ","
                  << std::fixed << std::setprecision(2) << tiempo << ","
                  << std::fixed << std::setprecision(2) << n * std::log2(n)
                  << "\n";
    }

    //Aplicamos MergeSort sobre el vector completo de solicitudes
    //El ordenamiento lo hacemos por tenure de forma descendente
    //Como solicitudes se pasa por referencia ordenamos directamente el vector original
    mergeSort(solicitudes, 0, static_cast<int>(solicitudes.size()) - 1);

    //Guardamos las solicitudes ya ordenadas en el archivo
    guardarSolicitudesOrdenadas(solicitudes);

    //Consultas del modulo A
    //Ahora la busqueda debe encontrar tenure exactamente igual a k
    std::vector<std::pair<std::string, int>> consultas = {
        {"Q_A01", 72},
        {"Q_A02", 60},
        {"Q_A03", 45},
        {"Q_A04", 30},
        {"Q_A05", 12}
    };

    busquedas << "\nRESULTADOS DE BUSQUEDA BINARIA EXACTA POR TENURE\n";

    //Recorremos cada consulta guardada en el vector consultas
    //Cada consulta tiene un nombre 
    //Por ejemplo Q_A01, y un valor k que sera el tenure a buscar
    for (const auto& consulta : consultas) {
        std::string nombre = consulta.first; //Nombre de la consulta actual
        int k = consulta.second; //Valor de tenure que vamos a buscar

        //Buscamos una solicitud cuyo tenure sea exactamente igual a k
        //Si hay varias solicitudes con el mismo tenure, la funcion nos puede retornar cualquiera
        int posicion = busquedaBinariaRecursivaExacta(
            solicitudes,
            0,
            static_cast<int>(solicitudes.size()) - 1,
            k
        );

        //Guardamos el resultado de la consulta actual
        busquedas << nombre << " k=" << k << " -> ";

        //Si posicion es diferente de -1 significa que encontramos una solicitud valida
        if (posicion != -1) {
            busquedas << "CustomerID: " << solicitudes[posicion].customerID
                      << " || Tenure: " << solicitudes[posicion].tenure
                      << "\n";
        } else {
            busquedas << "No encontrado\n"; //Si la posicion es -1 entonces no encontramos ningun tenure == k
        }
    }

    busquedas.close();

    std::cout << "Modulo A completado correctamente\n";
    std::cout << "Archivos generados:\n";
    std::cout << " - results/solicitudes_ordenadas.csv\n";
    std::cout << " - results/busquedas_A.txt\n";

    return 0;
}