Implementación Paralela del Algoritmo de Dijkstra

1. Descripción del Proyecto

Este proyecto implementa el algoritmo de Dijkstra para encontrar la ruta más corta desde un vértice origen hacia todos los demás vértices de un grafo. 
El algoritmo ha sido paralelizado en ciertas secciones críticas, como la búsqueda del mínimo y la relajación de vecinos, para mejorar el rendimiento en arquitecturas multinúcleo.

El programa cumple con los requisitos de la tarea de Computación Paralela y Distribuida de la Universidad Tecnológica Metropolitana, Departamento de Computación e Informática, 
bajo la dirección del Profesor Sebastián Salazar Molina. Ha sido desarrollado en C++ utilizando la biblioteca POSIX Threads (`pthread`) para el paralelismo explícito, 
como se solicita para ser probado en una máquina Ubuntu 24.04 LTS.

2. Requisitos del Sistema

* **Sistema Operativo:** Ubuntu 24.04 LTS (o un sistema Linux compatible con `g++` y `pthread`).
* **Compilador:** `g++` (GNU C++ Compiler)
* **Librerías:** `pthread` (incluida con `g++` por defecto al usar el flag `-pthread`).

3. Compilación del Proyecto

Para compilar el programa, navegue hasta el directorio raíz del proyecto en su terminal y ejecute el siguiente comando para crear el ejecutable.

g++ dijkstra_parallel.cpp -o dijkstra_parallel -pthread

4. Ejecución del Programa

El programa debe ser ejecutado desde la línea de comandos con tres argumentos:

Representación del grafo: Una cadena de texto que representa la matriz de adyacencia del grafo. Las filas deben estar separadas por punto y coma (;) y los elementos dentro de cada fila 
por comas (,). El programa reconoce matrices cuadradas de tamaño (N*N).

Vértice de inicio: Un número entero que indica el vértice desde el cual se calcularán las distancias más cortas (0-indexado).

Ruta al archivo de salida: La ruta completa o relativa donde el programa guardará los resultados.

Sintaxis de Ejecución:

./dijkstra_parallel "<matriz_de_adyacencia>" <vertice_inicio> "<ruta_archivo_salida>"

Ejemplos de Ejecución:

Ejemplo 1: Grafo pequeño (del enunciado de la tarea) 

./dijkstra_parallel "0,10,0,0,5;0,0,1,0,2;0,0,0,4,0;7,8,6,0,0;0,3,9,2,0" 0 output_ejemplo1.txt

Para ver el resultado:

cat output_ejemplo1.txt

Ejemplo 2: Grafo denso de 10 vértices

./dijkstra_parallel "0,2,3,0,0,0,0,0,0,0;2,0,0,5,0,0,0,0,0,0;3,0,0,0,4,0,0,0,0,0;0,5,0,0,0,6,0,0,0,0;0,0,4,0,0,0,7,0,0,0;0,0,0,6,0,0,0,8,0,0;0,0,0,0,7,0,0,0,9,0;0,0,0,0,0,8,0,0,0,1;
0,0,0,0,0,0,9,0,0,2;0,0,0,0,0,0,0,1,2,0" 0 output_grafo_denso.txt

Para ver el resultado:

cat output_grafo_denso.txt

Ejemplo 3: Grafo aleatorio de 20 vértices 

./dijkstra_parallel "0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;2,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;0,5,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;0,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;
0,0,0,1,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0;0,0,0,0,10,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,4,0,7,0,0,0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,7,0,3,0,0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,3,0,6,
0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,6,0,9,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,9,0,2,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0,2,0,5,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0,0,5,0,8,0,0,0,0,0,0;0,
0,0,0,0,0,0,0,0,0,0,0,8,0,1,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,10,0,0,0,0;0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,4,0,0,0;0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,7,0,0;0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,7,0,3,0;0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,6;0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0" 0 output_random20.txt

Para ver el resultado:

cat output_random20.txt

5. Salida del Programa

El programa generará un archivo de texto en la ruta especificada por el tercer argumento. Este archivo contendrá las distancias más cortas desde el vértice de origen a cada uno de los 
demás vértices del grafo. El formato de salida será una tabla con dos columnas: "Vértice" y "Distancia desde el origen". Los vértices inalcanzables se indicarán con "INF".

Para cualquier consulta sobre el proyecto, por favor contactar a:

Cristobal Perez
cperezb@utem.cl
