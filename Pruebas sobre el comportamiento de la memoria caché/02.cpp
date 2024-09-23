#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cmath>

// Inicializa una matriz con valores aleatorios
void initialize_matrix(std::vector<std::vector<double>> &A, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            A[i][j] = rand() % 100 + 1; // Valores aleatorios entre 1 y 100
        }
    }
}

// Multiplicación clásica de matrices (tres bucles anidados)
void matrix_multiplication(const std::vector<std::vector<double>> &A,
                           const std::vector<std::vector<double>> &B,
                           std::vector<std::vector<double>> &C, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            C[i][j] = 0;
            for (int k = 0; k < size; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Multiplicación por bloques (seis bucles anidados)
void block_matrix_multiplication(const std::vector<std::vector<double>> &A,
                                 const std::vector<std::vector<double>> &B,
                                 std::vector<std::vector<double>> &C, int size, int block_size)
{
    for (int ii = 0; ii < size; ii += block_size)
    {
        for (int jj = 0; jj < size; jj += block_size)
        {
            for (int kk = 0; kk < size; kk += block_size)
            {
                for (int i = ii; i < std::min(ii + block_size, size); ++i)
                {
                    for (int j = jj; j < std::min(jj + block_size, size); ++j)
                    {
                        for (int k = kk; k < std::min(kk + block_size, size); ++k)
                        {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    // Abrir archivo CSV para guardar los resultados
    std::ofstream csvfile;
    csvfile.open("matrix_multiplication_comparison.csv");
    csvfile << "Matrix Size,Classic Time (ms),Block Size 16 (ms),Block Size 32 (ms),Block Size 64 (ms),Block Size 128 (ms)\n";

    // Tamaños de matrices y tamaños de bloque a evaluar
    std::vector<int> sizes = {10, 25, 50, 75, 100, 250, 500, 750, 1000};
    std::vector<int> block_sizes = {16, 32, 64, 128}; // Diferentes tamaños de bloque para probar

    int num_trials = 10; // Número de veces que se ejecutarán las pruebas para sacar promedio

    // Bucle para cada tamaño de matriz
    for (int size : sizes)
    {
        double classic_avg_time = 0.0;
        std::vector<double> block_avg_times(block_sizes.size(), 0.0);

        // Ejecutar las pruebas num_trials veces
        for (int trial = 0; trial < num_trials; ++trial)
        {
            // Crear las matrices A, B y C
            std::vector<std::vector<double>> A(size, std::vector<double>(size));
            std::vector<std::vector<double>> B(size, std::vector<double>(size));
            std::vector<std::vector<double>> C(size, std::vector<double>(size, 0));
            std::vector<std::vector<double>> C_block(size, std::vector<double>(size, 0));

            // Inicializar matrices A y B con valores aleatorios
            initialize_matrix(A, size);
            initialize_matrix(B, size);

            // --- Multiplicación clásica ---
            auto start = std::chrono::high_resolution_clock::now();               // Iniciar cronómetro
            matrix_multiplication(A, B, C, size);                                 // Ejecutar multiplicación clásica
            auto end = std::chrono::high_resolution_clock::now();                 // Parar cronómetro
            std::chrono::duration<double, std::milli> classic_time = end - start; // Tiempo en milisegundos
            classic_avg_time += classic_time.count();                             // Acumular tiempo

            // --- Multiplicación por bloques con diferentes tamaños ---
            for (size_t b = 0; b < block_sizes.size(); ++b)
            {
                // Reiniciar C_block
                std::fill(C_block.begin(), C_block.end(), std::vector<double>(size, 0));

                start = std::chrono::high_resolution_clock::now();                  // Iniciar cronómetro
                block_matrix_multiplication(A, B, C_block, size, block_sizes[b]);   // Ejecutar multiplicación por bloques
                end = std::chrono::high_resolution_clock::now();                    // Parar cronómetro
                std::chrono::duration<double, std::milli> block_time = end - start; // Tiempo en milisegundos
                block_avg_times[b] += block_time.count();                           // Acumular tiempo
            }

            // Liberar memoria de las matrices
            A.clear();
            B.clear();
            C.clear();
            C_block.clear();
            A.shrink_to_fit();
            B.shrink_to_fit();
            C.shrink_to_fit();
            C_block.shrink_to_fit();
        }

        // Promediar los tiempos
        classic_avg_time /= num_trials;
        for (double &block_avg_time : block_avg_times)
        {
            block_avg_time /= num_trials;
        }

        // Guardar los resultados en el archivo CSV
        csvfile << size << "," << classic_avg_time;
        for (const double block_avg_time : block_avg_times)
        {
            csvfile << "," << block_avg_time;
        }
        csvfile << "\n";
        std::cout << "Matrix size " << size << " processed: Classic Avg = " << classic_avg_time << " ms." << std::endl;

        // Mostrar los promedios de tiempos por bloques
        for (size_t b = 0; b < block_sizes.size(); ++b)
        {
            std::cout << "Block Size " << block_sizes[b] << " Avg = " << block_avg_times[b] << " ms." << std::endl;
        }
    }

    // Cerrar el archivo CSV
    csvfile.close();
    return 0;
}
