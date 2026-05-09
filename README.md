
# ADA — Práctica Final Integradora 2026/01
 
**Universidad EAFIT — Análisis de Algoritmos**
Optimización de rutas y planificación de recursos en redes de telecomunicaciones — Algoritmos Codiciosos, Divide y Vencerás, y Programación Dinámica sobre datos reales.
 
## Integrantes
 
- Cristian Bolaños
- David Castellanos
- Juan Hernandez
## Descripción
 
Pipeline en C++ que resuelve tres subproblemas reales del despliegue de fibra óptica sobre el dataset **Telco Customer Churn** (Kaggle, 7,043 registros):
 
| Módulo | Paradigma | Algoritmo | Salida |
|--------|-----------|-----------|--------|
| **A** | Divide y Vencerás | MergeSort + Búsqueda Binaria Recursiva | `solicitudes_ordenadas.csv`, `busquedas_A.txt` |
| **B** | Codicioso | Kruskal con Union-Find (rango + compresión de caminos) | `mst_red.txt` |
| **C** | Programación Dinámica | Mochila 0-1 por tabulación + backtracking | `asignacion_bw.txt` |
 
Los tres módulos se ejecutan en una sola invocación. La salida del Módulo A (vector ordenado por `tenure` descendente) alimenta al Módulo C (selección de las 50 solicitudes activas con mayor antigüedad).
 
## Requisitos
 
- **Compilador:** g++ 9 o superior con soporte C++17
- **Sistema operativo:** Linux, macOS o Windows con MinGW/WSL
- **Dataset:** `WA_Fn-UseC_-Telco-Customer-Churn.csv` (incluido en `data/`)
## Estructura del repositorio
 
```
ADA_PF_Apellido1_Apellido2_Apellido3/
├── README.md
├── data/
│   └── WA_Fn-UseC_-Telco-Customer-Churn.csv
├── src/
│   ├── parser.cpp         parser.hpp
│   ├── mergesort.cpp      mergesort.hpp
│   ├── binary_search.cpp  binary_search.hpp
│   ├── graph.cpp          graph.hpp
│   ├── kruskal.cpp        kruskal.hpp
│   ├── knapsack.cpp       knapsack.hpp
│   └── main.cpp
├── results/                  (generado en cada ejecución)
│   ├── solicitudes_ordenadas.csv
│   ├── busquedas_A.txt
│   ├── mst_red.txt
│   └── asignacion_bw.txt
└── report/
    └── Informe.pdf
```
 
## Compilación
 
Desde la raíz del repositorio:
 
```bash
g++ -std=c++17 -O2 -o ada_pf src/main.cpp src/*.cpp
```
 
> El flag `-O2` es necesario para que los tiempos empíricos de MergeSort sean representativos. Sin optimización los tiempos crecen significativamente, aunque la complejidad asintótica O(n log n) sigue verificándose.
 
## Ejecución
 
```bash
./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```
 
El programa crea automáticamente la carpeta `results/` si no existe y genera **los cuatro archivos de salida en una sola corrida**.
 
### Salida esperada en consola
 
```
Registros cargados: 7043
Registros con TotalCharges vacio: 11
Modulo A completado correctamente
Archivos generados:
 - results/solicitudes_ordenadas.csv
 - results/busquedas_A.txt
 - results/mst_red.txt
 - results/asignacion_bw.txt
```
 
## Detalles por módulo
 
### Módulo A — Divide y Vencerás
 
- **MergeSort** ordena las solicitudes de forma **descendente** por `tenure` (el de mayor antigüedad primero) y es **estable**: ante empates conserva el orden original del CSV.
- **Búsqueda binaria recursiva exacta** sobre el arreglo ya ordenado descendente: encuentra una posición cuyo `tenure` sea exactamente igual a `k`. Como hay miles de empates, retorna cualquiera de las posiciones válidas (la primera que encuentre la recursión).
- **Análisis empírico** mide tiempos para n = 1,000, 3,500 y 7,043 con `std::chrono::high_resolution_clock` y los compara contra `n · log₂ n`.
- **Consultas fijas** ejecutadas: k = 72, 60, 45, 30, 12.
### Módulo B — Codicioso (Kruskal)
 
- Los 7,043 registros se agrupan en 20 grupos por `i mod 20`.
- Para cada grupo se calcula el promedio de `MonthlyCharges` (redondeado a 2 decimales).
- Para cada par `(u, v)` con `u < v` se crea una arista de peso `⌊M_u + M_v⌋`. Resulta un grafo completo: **20 nodos, 190 aristas**.
- Kruskal selecciona las 19 aristas de menor peso que no formen ciclo, usando Union-Find con **unión por rango** y **compresión de caminos**.
### Módulo C — Programación Dinámica (Mochila 0-1)
 
- Toma las **primeras 50 solicitudes activas** (`Churn = "No"`) del arreglo ordenado del Módulo A.
- Capacidad: `W = 500`.
- Escalado:
  - `peso = round(TotalCharges / 10)`
  - `valor = round(MonthlyCharges × 10)`
- Tabla `dp` de dimensiones `(51 × 501)`.
- Backtracking sobre `dp` reconstruye el conjunto exacto de solicitudes seleccionadas.
- Búsqueda exhaustiva de un **contraejemplo codicioso** sobre tríos de las 50 solicitudes: encuentra una capacidad parcial donde el codicioso por ratio `v/w` produce un valor estrictamente menor que la PD.
## Verificación de resultados
 
| Propiedad | Esperado | Obtenido |
|-----------|----------|----------|
| Registros cargados | 7,043 | ✔ |
| Registros con `TotalCharges` nulo | 11 | ✔ |
| `tenure` máx / mín | 72 / 0 | ✔ |
| Aristas del grafo (Módulo B) | 190 | ✔ |
| Aristas del MST | 19 | ✔ |
| Tabla dp (Módulo C) | 51 × 501 | ✔ |
 
Como el procedimiento de construcción del grafo (Sección 9 del enunciado) es determinista, **todos los equipos deben obtener exactamente el mismo MST y el mismo valor óptimo de la mochila**, lo que permite verificación cruzada.
 
## Limpieza
 
```bash
rm -rf results/ ada_pf
```
 
## Herramientas utilizadas
 
- `g++` (GCC) con C++17 y `-O2`
- Visual Studio Code
- Git / GitHub
- Asistente de IA generativa (ChatGPT / Claude): consultado para aclarar conceptos teóricos (Lema del Ciclo, pseudopolinomialidad) y revisar redacción.
