PRÁCTICA 4: ALGORITMOS DE EXPLORACIÓN DE GRAFOS

1- COMPILACIÓN:
Para compilar todos los programas a la vez, ejecuta: 
	make
Se generarán los ejecutables: 'seleccion_proyectos' y 'coloreo_grafos'.

2- EJECUCIÓN: 
	i) EJERCICIO 2.1 (Selección de Proyectos): 
		./seleccion_proyectos
		
	ii) EJERCICIO 2.2 (Coloreo de Grafos): 
		El programa requiere la ruta de un archivo de grafo (.txt) 
		y opcional el numero de colores para Backtracking
		
		Ejecución básica: 
			./graph_coloring <ruta_fichero_grafo>
		Ejecución con límite de colores para Backtracking:
			./graph_coloring <ruta_fichero_grafo> <n_colores>
			
		Por ejemplo: 
			./graph_coloring data/map_coloring_example.txt 3

3- LIMPIEZA: 
Para borrar los ejecutables: 
	make clean
