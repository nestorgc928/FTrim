PRÁCTICA 3: ALGORITMOS GREEDY

Este paquete contiene la resolución de los dos problemas de la práctica 3.

1. COMPILACIÓN
Para compilar todos los programas a la vez, ejecuta:
    make

Se generarán los ejecutables: 'problema3_1', 'nearest_neighbor' y 'nearest_insertion'.

2. EJECUCIÓN

PROBLEMA 1 (Servidores y Tareas):
    ./problema3_1

PROBLEMA 2 (Viajante de Comercio - TSP):
Para ejecutar las heurísticas sobre todas las instancias y generar las gráficas .png:

A) Vecino más cercano:
    ./nearest_neighbor ./data/a280.tsp
    ./nearest_neighbor ./data/berlin52.tsp
    ./nearest_neighbor ./data/ch150.tsp
    ./nearest_neighbor ./data/kroA100.tsp
    ./nearest_neighbor ./data/st70.tsp

B) Inserción más económica:
    ./nearest_insertion ./data/a280.tsp
    ./nearest_insertion ./data/berlin52.tsp
    ./nearest_insertion ./data/ch150.tsp
    ./nearest_insertion ./data/kroA100.tsp
    ./nearest_insertion ./data/st70.tsp

Cada ejecución mostrará el coste calculado y generará automáticamente la imagen
del tour (ej: berlin52_nearest_neighbor_tour.png).

3. LIMPIEZA
Para borrar los ejecutables y todas las imágenes generadas, ejecuta:
    make clean
