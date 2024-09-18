#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

#define MAX 5000

// Inicializa la matriz A y los vectores x e y con valores aleatorios
void initialize_matrix(std::vector<std::vector<double>> &A, std::vector<double> &x, std::vector<double> &y, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            A[i][j] = rand() % 10 + 1; // Valores aleatorios para A
        }
        x[i] = rand() % 10 + 1; // Valores aleatorios para x
        y[i] = 0.0;             // Inicializa y en 0
    }
}

// Primer bucle: se recorre por filas (i) y luego por columnas (j)
void first_loop(const std::vector<std::vector<double>> &A, const std::vector<double> &x, std::vector<double> &y, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            // Cada acceso A[i][j] sigue un patrón de acceso secuencial por fila,
            // lo que favorece la localización espacial en caché.
            y[i] += A[i][j] * x[j];
        }
    }
    // Comentario sobre caché:
    // En este caso, la matriz A se recorre por filas. En la mayoría de los sistemas,
    // las matrices se almacenan en memoria de manera continua por filas (orden fila-major).
    // Este patrón de acceso secuencial aprovecha la localización espacial de la caché,
    // ya que los elementos consecutivos de la fila de A están cerca en la memoria,
    // reduciendo las fallas de caché.
}

// Segundo bucle: se recorre por columnas (j) y luego por filas (i)
void second_loop(const std::vector<std::vector<double>> &A, const std::vector<double> &x, std::vector<double> &y, int size)
{
    for (int j = 0; j < size; ++j)
    {
        for (int i = 0; i < size; ++i)
        {
            // Aquí se accede a A[i][j], pero en este caso, el patrón de acceso es por columnas.
            // Esto implica saltos de memoria, lo que es menos eficiente en términos de caché.
            y[i] += A[i][j] * x[j];
        }
    }
    // Comentario sobre caché:
    // En este segundo bucle, se accede a la matriz A por columnas. Dado que la memoria se organiza por filas,
    // esto significa que los elementos que se acceden en A[i][j] no están secuencialmente en memoria.
    // Esto puede resultar en más fallos de caché, ya que al acceder a un elemento de una columna,
    // el siguiente elemento que se accede puede estar lejos en la memoria, forzando recargas en caché.
}

int main()
{
    std::ofstream csvfile;
    csvfile.open("loop_times.csv");
    csvfile << "Matrix Size,First Loop (ms),Second Loop (ms)\n";

    std::vector<int> sizes = {10, 25, 50, 75, 100, 250, 500, 750, 1000, 2500, 5000, 7500, 10000};

    for (int size : sizes)
    {
        // Crear la matriz A y los vectores x e y para el tamaño actual
        std::vector<std::vector<double>> A(size, std::vector<double>(size));
        std::vector<double> x(size), y(size);

        // Inicializa A, x e y
        initialize_matrix(A, x, y, size);

        // Medir el tiempo del primer bucle
        auto start = std::chrono::high_resolution_clock::now();
        first_loop(A, x, y, size);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> first_loop_time = end - start;

        // Re-inicializa y a 0
        std::fill(y.begin(), y.end(), 0.0);

        // Medir el tiempo del segundo bucle
        start = std::chrono::high_resolution_clock::now();
        second_loop(A, x, y, size);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> second_loop_time = end - start;

        // Guarda los tiempos en el archivo CSV
        csvfile << size << "," << first_loop_time.count() << "," << second_loop_time.count() << "\n";
        std::cout << "Size " << size << " done." << std::endl;

        // Liberar memoria explicitamente al reducir el tamaño de los vectores a 0
        A.clear();         // Libera memoria de la matriz A
        A.shrink_to_fit(); // Libera la capacidad asignada en exceso

        x.clear();         // Libera memoria del vector x
        x.shrink_to_fit(); // Libera la capacidad asignada en exceso

        y.clear();         // Libera memoria del vector y
        y.shrink_to_fit(); // Libera la capacidad asignada en exceso
    }

    csvfile.close();
    return 0;
}
