//parser
#include "parser.hpp" //Incluimos la estructura Solicitud y la funcion leerCSV 
#include <fstream> //Abrir y leer archivos
#include <sstream> //Para separar los campos del CSV usando un flujo de texto
#include <iostream> //Para mostrar mensajes de error      

//Esta funcion recibe una linea del CSV y la divide en campos 
//Pasamos como parametro una "linea" que corresponde a una fila del CSV
std::vector<std::string> dividirCSV(const std::string& linea) {
    //Creamos un vector llamado campos, donde guardaremos los datos separados de la linea del CSV
    std::vector<std::string> campos;
    //Variable temporal para guardar cada campo que vayamos leyendo 
    std::string campo;

    //Convertimos la linea en un flujo de lectura para poder recorrerla por partes
    //ss es una variable de tipo std::stringstream que nos permite leer la linea como si fuera una entrada de datos
    std::stringstream ss(linea); 

    //Bucle para leer desde el flujo ss hasta encontrar una coma
    //Cada parte leida, es separada por comas y la guardamos temporalmente en la variable campo
    while (std::getline(ss, campo, ',')) {
        campos.push_back(campo); //Agregamos el campo leido al vector campos 
    }

    return campos; //Retornamos todos los campos separados de la linea 
}

//Funcion que nos sirve para leer el archivo CSV desde la ruta indicada
//Retornamos un vector de Solicitud con los campos requeridos del dataset
//Pasamos totalChargesNulos por referencia para contar cuantos registros tiene el campo TotalCharges vacio
std::vector<Solicitud> leerCSV(const std::string& ruta, int& totalChargesNulos) {
    //Creamos un vector donde guardamos todas las solicitudes cargadas
    std::vector<Solicitud> solicitudes;
    //Abrimos el archivo por la ruta recibida 
    std::ifstream archivo(ruta);

    //Verificamos si el archivo no se abrio correctamente 
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo CSV.\n";
        return solicitudes; //Si no pudimos abrir el archivo retornamos el vector vacio
    }

    //Creamos una variable donde guardaremos cada linea completa del archivo
    std::string linea;
    //Leemos la primera linea del archivo, y la ignoramos ya que es la cabecera
    std::getline(archivo, linea); 
    
    //Inicializamos la variable totalChargesNulos, ya que por el momento no hemos encontrado ningun TotalCharges vacio
    totalChargesNulos = 0;

    //Leemos el archivo linea por linea hasta llegar al final 
    while (std::getline(archivo, linea)) {
        //Tomamos la linea completa y la separamos en campos utilizando la funcion dividirCSV
        std::vector<std::string> campos = dividirCSV(linea);

        //Verificamos que la fila tenga al menos 21 campos
        //porque el ultimo campo que necesitamos es Churn y esta en el indice 20
        if (campos.size() < 21) continue; //Si tiene menos campos, ignoramos la linea

        //Creamos una variable temporal de tipo Solicitud para guardar los datos de esta fila 
        Solicitud s;

        //Asignamos a la solicitud los campos que necesitamos del CSV
        s.customerID = campos[0]; //ID del cliente
        s.tenure = std::stoi(campos[5]); //Convertimos de texto a entero
        s.monthlyCharges = std::stod(campos[18]); //Convertimos de texto a decimal
        s.churn = campos[20]; //Estado del cliente

        //Verificamos si el campo TotalCharges viene vacio o con un espacio
        //Si llega a ser así le asignamos 0.0 y aumentamos el contador totalChargesNulos
        if (campos[19] == " " || campos[19].empty()) {
            s.totalCharges = 0.0;
            totalChargesNulos++;
        } else {
            s.totalCharges = std::stod(campos[19]); //Si no esta vacio, lo convertimos de texto a decimal
        }

        //Agregamos la solicitud ya procesada al vector solicitudes       
        solicitudes.push_back(s);
    }

    archivo.close(); //Cerramos el archivo
    return solicitudes; //Retornamos el vector con todas las solicitudes cargadas 
}