//parser
//Con esto logramos que el archivo no se incluya mas de una vez durante la compilacion
#ifndef PARSER_HPP
#define PARSER_HPP
//Bibliotecas para manejar strings y vectores 
#include <string>
#include <vector>

//Estructura para representar una solicitud tomada del dataset
struct Solicitud {
    std::string customerID; //Identificador del cliente 
    int tenure; //Prioridad de servicio por antiguedad 
    double monthlyCharges; //Valor mensual de la solicitud (cobro al cliente mensual) 
    double totalCharges; //Valor acumulado, en el modulo C lo utilizamos como base para el peso
    std::string churn; //Estado de la solicitud "No" -> activa y "Yes" -> en riesgo
};
//Leemos el archivo CSV ubicado en la ruta dada 
//Devuelve un vector de Solicitud(struct), donde cada Solicitud representa una fila del dataset con los campos requeridos 
std::vector<Solicitud> leerCSV(const std::string& ruta, int& totalChargesNulos);

#endif