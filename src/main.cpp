//Metodo principal
#include <iostream> //Mostrar resultados en pantalla 
#include <vector>  //Para usar vectores 
#include "parser.hpp" //Incluimos la estructura Solicitud y la funcion leerCSV 

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

    return 0;
}