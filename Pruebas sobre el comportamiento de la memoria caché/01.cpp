#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <numeric> // Para std::accumulate

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
            y[i] += A[i][j] * x[j];
        }
    }
}

// Segundo bucle: se recorre por columnas (j) y luego por filas (i)
void second_loop(const std::vector<std::vector<double>> &A, const std::vector<double> &x, std::vector<double> &y, int size)
{
    for (int j = 0; j < size; ++j)
    {
        for (int i = 0; i < size; ++i)
        {
            y[i] += A[i][j] * x[j];
        }
    }
}

int main()
{
    std::ofstream csvfile;
    csvfile.open("loop_times.csv");              
    csvfile << "Matrix Size,First Loop Avg Time (ms),Second Loop Avg Time (ms)\n";

    std::vector<int> sizes = {10, 25, 50, 75, 100, 250, 500, 750, 1000, 2500, 5000, 7500, 10000};
    int num_trials = 50; // Número de pruebas para promediar

    for (int size : sizes)
    {
        // Vectores para almacenar tiempos
        std::vector<double> first_loop_times;
        std::vector<double> second_loop_times;

        for (int trial = 0; trial < num_trials; ++trial)
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
            first_loop_times.push_back(first_loop_time.count()); // Almacena el tiempo

            // Re-inicializa y a 0
            std::fill(y.begin(), y.end(), 0.0);

            // Medir el tiempo del segundo bucle
            start = std::chrono::high_resolution_clock::now();
            second_loop(A, x, y, size);
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> second_loop_time = end - start;
            second_loop_times.push_back(second_loop_time.count()); // Almacena el tiempo

            // Liberar memoria explicitamente
            A.clear();         // Libera memoria de la matriz A
            A.shrink_to_fit(); // Libera la capacidad asignada en exceso

            x.clear();         // Libera memoria del vector x
            x.shrink_to_fit(); // Libera la capacidad asignada en exceso

            y.clear();         // Libera memoria del vector y
            y.shrink_to_fit(); // Libera la capacidad asignada en exceso
        }

        // Calcular los promedios de tiempo
        double first_loop_avg = std::accumulate(first_loop_times.begin(), first_loop_times.end(), 0.0) / num_trials;
        double second_loop_avg = std::accumulate(second_loop_times.begin(), second_loop_times.end(), 0.0) / num_trials;

        // Guarda los promedios en el archivo CSV
        csvfile << size << "," << first_loop_avg << "," << second_loop_avg << "\n";
        std::cout << "Matrix size " << size << " done. First Loop Avg: " << first_loop_avg << " ms, Second Loop Avg: " << second_loop_avg << " ms." << std::endl;
    }

    csvfile.close();
    return 0;
}
