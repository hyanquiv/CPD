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
    // Bucle externo recorre filas de A
    for (int i = 0; i < size; ++i)
    {
        // Bucle intermedio recorre columnas de B
        for (int j = 0; j < size; ++j)
        {
            C[i][j] = 0; // Inicializamos cada entrada de C a 0
            // Bucle interno para realizar la multiplicación de la fila de A por la columna de B
            for (int k = 0; k < size; ++k)
            {
                C[i][j] += A[i][k] * B[k][j]; // Cálculo clásico de la multiplicación
            }
        }
    }
    // Nota: Este enfoque tiene problemas de caché. Los elementos de A se recorren por filas (bueno para caché),
    // pero los elementos de B se recorren por columnas, lo que causa saltos en la memoria (malo para caché),
    // resultando en posibles fallos de caché y mayor tiempo de acceso a la memoria principal.
}

// Multiplicación por bloques (seis bucles anidados)
void block_matrix_multiplication(const std::vector<std::vector<double>> &A,
                                 const std::vector<std::vector<double>> &B,
                                 std::vector<std::vector<double>> &C, int size, int block_size)
{
    // Los tres bucles externos dividen la matriz en bloques
    for (int ii = 0; ii < size; ii += block_size)
    {
        for (int jj = 0; jj < size; jj += block_size)
        {
            for (int kk = 0; kk < size; kk += block_size)
            {
                // Los tres bucles internos recorren los elementos dentro de los bloques
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
    // Este enfoque mejora el comportamiento en la caché:
    // 1. En lugar de recorrer toda la matriz B columna por columna (lo cual es malo para la caché),
    //    ahora operamos en bloques pequeños, donde los datos de A, B y C en cada bloque pueden caber
    //    en la caché del procesador.
    // 2. Al operar dentro de bloques, se reduce el número de fallos de caché, ya que se reutilizan más
    //    eficientemente los mismos datos antes de que se expulsen de la caché.
    // 3. En general, la multiplicación por bloques minimiza la necesidad de acceder a la memoria principal
    //    constantemente, ya que los datos necesarios permanecen en la caché por más tiempo.
}

int main()
{
    // Abrir archivo CSV para guardar los resultados
    std::ofstream csvfile;
    csvfile.open("matrix_multiplication_comparison.csv");
    csvfile << "Matrix Size,Classic Time (ms),Block Time (ms)\n";

    // Tamaños de matriz que vamos a probar
    std::vector<int> sizes = {10, 25, 50, 75, 100, 250, 500, 750, 1000};
    int block_size = 64; // Tamaño de bloque para la multiplicación por bloques (típico para mejorar uso de caché)

    // Bucle para cada tamaño de matriz
    for (int size : sizes)
    {
        // Crear las matrices A, B y C (A y B son las multiplicandas, C es la resultante)
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

        // --- Multiplicación por bloques ---
        start = std::chrono::high_resolution_clock::now();                  // Iniciar cronómetro
        block_matrix_multiplication(A, B, C_block, size, block_size);       // Ejecutar multiplicación por bloques
        end = std::chrono::high_resolution_clock::now();                    // Parar cronómetro
        std::chrono::duration<double, std::milli> block_time = end - start; // Tiempo en milisegundos

        // Guardar los resultados en el archivo CSV
        csvfile << size << "," << classic_time.count() << "," << block_time.count() << "\n";
        std::cout << "Matrix size " << size << " processed: Classic = " << classic_time.count()
                  << " ms, Block = " << block_time.count() << " ms." << std::endl;

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

    // Cerrar el archivo CSV
    csvfile.close();
    return 0;
}
